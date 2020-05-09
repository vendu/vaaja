#ifndef __MT_MTDEQ_H__
#define __MT_MTDEQ_H__

#include <zero/param.h>
#include <zero/cdefs.h>
#include <zero/asm.h>

#define MTDEQNLKSPIN  8
#define MTDEQLKBITPOS 0

#define MTDEQBKTSLOTS (CLSIZE / sizeof(void *))
struct mtdeqbkt {
    void *slots[MTDEQBKTSLOTS];
};

static __inline__ void
mtdeqputitem(struct mtxeqbkt *bkt, struct mtxdeqitem *item)
{
    void      **ptr;
    uintptr_t   val = (uintptr_t)item >> 17;
    long        lock = 0;
    long        ndx = val;
    long        cnt = -1;

    do {
        ndx &= (MTDEQBKTSLOTS - 1);
        do {
            ptr = &bkt->slots[ndx];
            val += 37;
            lock = m_cmpswapbit((m_atomic_t *)ptr, MTDEQLKBITPOS);
            ndx++;
            if (lock) {
                ptr = (void *)((uintptr_t)*ptr & (1UL << MTDEQLKBITPOS));
                item->next = (void *)ptr;
                m_atomwrite((m_atomic_t *)&bkt->slots[ndx],
                            (m_atomic_t)item);

                return;
            }
            ndx &= MTDEQBKTSLOTS - 1;
        } while (cnt < MTDEQBKTSLOTS - 1);
        ndx = (uintptr_t)item;
        m_waitspin();
        ndx -= val;
        cnt = -1;
    } while (1);

    /* NOTREACHED */
    return;
}

static __inline__ struct mtdeqitem *
mtdeqgetitem(struct mtxeqbkt *bkt)
{
    struct mtdeqitem  *item;
    void             **ptr;
    uintptr_t          val = (uintptr_t)bkt >> 13;
    long               lock = 0;
    long               ndx = val;
    long               cnt = -1;

    do {
        ndx &= MEMDEQBKTSLOTS - 1;
        do {
            ptr = &bkt->slots[ndx];
            val += 17;
            lock = m_cmpswapbit((m_atomic_t *)ptr, MTDEQLKBITPOS);
            ndx++;
            if (lock) {
                ptr = (void *)((uintptr_t)*ptr & (1UL << MTDEQLKBITPOS));
                if (!ptr) {
                    m_atomwrite((m_atomic_t *)&bkt->slots[ndx],
                                (m_atomic_t)ptr);
                } else {
                    item = ptr;
                    m_atomwrite((m_atomic_t *)&bkt->slots[ndx],
                                (m_atomic_t)item->next);

                    return item;
                }
            }
            ndx &= MTDEQBKTSLOTS - 1;
        } while (cnt < MTDEQBKTSLOTS - 1);
        ndx = (uintptr_t)bkt;
        m_waitspin();
        ndx -= val;
        cnt = -1;
    } while (1);

    /* NOTREACHED */
    return NULL;
}

#endif /* __MT_MTDEQ_H__ */

