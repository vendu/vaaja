#ifndef __MT_LK_H__
#define __MT_LK_H__

/* velho lock routines */

#include <stdint.h>
#include <mach/asm.h>

#define MT_MEM_LK_BIT          ((uintptr_t)1 << MT_MEM_LK_BIT_OFS)
#define MT_MEM_LK_BIT_OFS      1

static INLINE long
mttrylkbit(volatile m_atomic_t *lp, long pos)
{
    m_atomic_t res = 0;

    res = !m_cmpsetbit(lp, pos);

    return res;
}

static INLINE long
mtlkbit(volatile m_atomic_t *lp, long pos)
{
    m_atomic_t  res = 0;
    long        bit = 1L << pos;

    do {
        while (*lp & bit) {
            m_waitspin();
        }
        res = !m_cmpsetbit(lp, pos);
        if (!res) {
            m_waitspin();
        }
    } while (!res);

    return res;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static INLINE void
mtunlkbit(volatile m_atomic_t *lp, long pos)
{
    m_membar();         // full memory barrier
    m_clrbit(lp, pos);  // clear bit
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* __MT_LK_H__ */

