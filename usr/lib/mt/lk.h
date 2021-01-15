#ifndef MT_LK_H
#define MT_LK_H

#include <env/cdefs.h>
#include <mach/asm.h>

/* velho lock routines */

#define MT_ADR_LK_BIT           ((uintptr_t)1 << MT_ADR_LK_BIT_OFS)
#define MT_ADR_LK_BIT_OFS       0

static C_INLINE long
mttrylkbit(volatile m_atomic_t *lp, long pos)
{
    m_atomic_t                  res = 0;

    res = !m_cmpsetbit(lp, pos);

    return res;
}

static C_INLINE long
mtlkbit(volatile m_atomic_t *lp, long pos)
{
    m_atomic_t                  res = 0;
    long                        bit = 1L << pos;

    do {
        while (*lp & bit) {
            m_waitspin();
        }
        res = !m_cmpsetbit(lp, pos);
    } while (!res);

    return res;
}

/*
 * unlock bit-lock
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
mtunlkbit(volatile m_atomic_t *lp, long pos)
{
    m_membar();         // full memory barrier
    m_clrbit(lp, pos);  // clear bit
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* MT_LK_H */

