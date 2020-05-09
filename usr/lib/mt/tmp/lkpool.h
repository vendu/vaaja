#ifndef __MT_LKPOOL_H__
#define __MT_LKPOOL_H__

#include <string.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/atomic.h>
#include <mach/asm.h>
#include <mt/mtx.h>
#include <zero/trix.h>
#include <zero/hash.h>
#include <zero/qrand.h>

#define LKPOOL_ITEMS 8

/* make mutex size equal cacheline */
struct lkpoolitem {
    m_atomic_t     lk;
    void          *tab;
    LKPOOL_DATA_T  data;
    int8_t         _pad[CLSIZE
                        - sizeof(m_atomic_t)
                        - sizeof(void *)
                        - sizeof(LKPOOL_DATA_T)];
};

/*
 * distribute load somewhat-randomly over LKPOOL_ITEMS mutexes instead of a
 * single one
 */
struct lkpool {
    struct lkpoolitem tab[LKPOOL_ITEMS];;
};

static __inline__ void
lkpoolinit(struct lkpool *pool)
{
    memset(pool, 0, sizeof(struct lkpool));

    return;
}

/* scan pool for an unacquired lock, start at random offset into tab */
static INLINE long
lkpooltrylk(struct lkpool *pool)
{
    struct lkpoolitem *lpmtx = &pool->tab[0];
    long                n = LKPOOL_ITEMS;
    long                ofs = qrand32();

    while (n--) {
        ofs &= LKPOOL_ITEMS - 1;
        if (lpmtx[ofs].lk == FMTXINITVAL) {
            if (fmtxtrylk(&lpmtx[ofs].lk)) {

                return ofs;
            }
        }
        m_waitspin();
    }

    return -1;
}

/* acquire lock from pool */
static __inline__ long
lkpoollk(struct lkpool *pool)
{
    long res = -1;

    do {
        res = lkpooltrylk(pool);
    } while (res < 0);

    return res;
}

/* unlock lock in pool */
#define lkpoolunlk(pp, ofs) fmtxunlk(&(pp)->tab[(ofs)].lk)

#endif /* __MT_LKPOOL_H__ */

