#ifndef DECK_LIB_LK_H
#define DECK_LIB_LK_H

/* deck library lock routines */

#include <stdint.h>
#include <deck/lib/asm.h>

#define M_ADR_LK_BIT          ((uintptr_t)1 << M_ADR_LK_BIT_OFS)
#define M_ADR_LK_BIT_OFS      0

static C_INLINE long
m_trylkbit(volatile m_atomic_t *lp, long pos)
{
    m_atomic_t res = 0;

    res = !m_cmpsetbit(lp, pos);

    return res;
}

static C_INLINE long
m_lkbit(volatile m_atomic_t *lp, long pos)
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
 * unlock bit-lock
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
m_unlkbit(volatile m_atomic_t *lp, long pos)
{
    m_membar();         // full memory barrier
    m_clrbit(lp, pos);  // clear bit
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* DECK_LIB_LK_H */

