#ifndef __MT_STKPOOL_H__
#define __MT_STKPOOL_H__

#if !defined(STKPOOL_LOCKFREE)
#define STKPOOL_LOCKFREE 0
#endif
#if !defined(STKPOOL_ITEM_T)
#define STKPOOL_ITEM_T struct stkitem
#endif
#if !defined(STKPOOL_CHAIN_T)
#if defined(STKPOOL_LOCKFREE) && (STKPOOL_LOCKFREE)
#define STKPOOL_CHAIN_T volatile STKPOOL_ITEM_T
#else
#define STKPOOL_CHAIN_T STKPOOL_ITEM_T
#endif
#endif /* !defined(STKPOOL_CHAIN_T) */

#define LKPOOL_DATA_T struct stkitem

struct stkitem {
    STKPOOL_CHAIN_T  *chain;
    void             *ptr;
    void            (*recl)(void *);
};

#include <mach/asm.h>
#include <mt/lkpool.h>

#if defined(STKPOOL_LOCKFREE)

/* push traverses the pool locks backwards */
static __inline__ void
stkpoolpush(struct lkpool *pool, STKPOOL_CHAIN_T *item)
{
    long             ofs = qrand32();
    long             n = LKPOOL_ITEMS;
    STKPOOL_ITEM_T  *data;
    STKPOOL_CHAIN_T *chain;

    do {
        do {
            ofs &= LKPOOL_ITEMS - 1;
            data = &pool->tab[ofs].data;
            chain = data->chain;
            item->chain = chain;
            if (m_cmpswapptr((m_atomicptr_t *)&data->chain,
                             (m_atomicptr_t)chain,
                             (m_atomicptr_t)item)) {

                return;
            }
            ofs--;
            m_waitspin();
        } while (n--);
        ofs = qrand32();
    } while (1);

    return;
}

#define stkpoolgetitem()   malloc(sizeof(STKPOOL_CHAIN_T))
//#define stkpoolputitem(dp) ((dp)->recl((dp)->ptr))
#define stkpoolputitem(dp) /* FIXME: reclaim memory */

static __inline__ void *
stkpoolpushptr(struct lkpool *pool, void *ptr)
{
    STKPOOL_ITEM_T *item = stkpoolgetitem();

    if (item) {
        item->ptr = ptr;
        stkpoolpush(pool, item);
    }

    return item;
}

/* push traverses the pool locks forward */
static __inline__ void *
stkpoolpop(struct lkpool *pool)
{
    long             ofs = qrand32();
    long             n = LKPOOL_ITEMS;
    STKPOOL_ITEM_T  *data;
    void            *ptr;
    STKPOOL_CHAIN_T *chain;

    do {
        do {
            ofs &= LKPOOL_ITEMS - 1;
            data = &pool->tab[ofs].data;
            chain = data->chain;
            if ((chain)
                && m_cmpswapptr((m_atomicptr_t *)&data->chain,
                                (m_atomicptr_t)chain,
                                (m_atomicptr_t)chain->chain)) {
                ptr = chain->ptr;
                if (chain) {
                    stkpoolputitem(data);
                }

                return ptr;
            }
            ofs++;
            m_waitspin();
        } while (n--);
    } while (1);

    return NULL;
}

#else

static __inline__ void
stkpoolpush(struct lkpool *pool, STKPOOL_CHAIN_T *item)
{
    long ofs = lkpoollk(pool);

    item->chain = pool->tab[ofs].data.chain;
    pool[ofs].tab->chain = item;
    lkpoolunlk(pool, ofs);

    return;
}

static __inline__ void *
stkpoolpop(struct lkpool *pool)
{
    long             ofs = qrand32();
    long             n = LKPOOL_ITEMS;
    void           *ptr;
    struct stkitem *item;

    while (n--) {
        ofs &= LKPOOL_ITEMS - 1;
        if (m_cmpswap(&pool->tab[ofs].lk, FMTXINITVAL, FMTXLKVAL)) {
            item = pool[ofs].tab->chain;
            if (item) {
                pool->tab[ofs].data.chain = item->chain;
                lkpoolunlk(pool, ofs);
                ptr = item->ptr;

                return ptr;
            }
        }
        ofs++;
    }

    return NULL;
}

#endif /* STKPOOL_LOCKFREE */

#endif /* __MT_STKPOOL_H__ */

