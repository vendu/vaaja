#ifndef MT_SPINWT_H
#define MT_SPINWT_H

/* velho spin-wait locks */

#include <mt/mt.h>

/*
 * NOTES
 * -----
 * - initialise spin-wait locks with spininit() from <mt/spin.h>
 */

/*
 * try to acquire spin-wait lock
 * - return non-zero on success, zero otherwise
 */
static __inline__ long
mttryspinwt(volatile m_atomic_t *sp, m_atomic_t val, long niter)
{
    volatile m_atomic_t res = 0;

    do {
        while (*sp != MTSPININITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(sp, MTSPININITVAL, val);
    } while (--niter && !res);

    return res;
}

/*
 * lock spin-wait lock
 */
static __inline__ void
mtlkspinwt(volatile m_atomic_t *sp, m_atomic_t val, long niter)
{
    volatile m_atomic_t res = 0;

    do {
        while (*sp != MTSPININITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(sp, MTSPININITVAL, val);
    } while (--niter && !res);
    if (!niter) {
        mtmtxlk(sp, val);
    }

    return;
}

/*
 * release spin-wait lock
 */
static __inline__ void
mtunlkspinwt(volatile m_atomic_t *sp)
{
    m_membar();
    *sp = MTSPININITVAL;
    m_endspin();

    return;
}

#endif /* MT_SPINWT_H */

