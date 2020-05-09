#ifndef __MT_SPINWT_H__
#define __MT_SPINWT_H__

/* velho spin-wait locks */

/*
 * NOTES
 * -----
 * - initialise spin-wait locks with spininit() from <mt/spin.h>
 */

#include <mach/atomice.h>
#include <mach/asm.h>
#include <mt/spin.h>

/*
 * try to acquire spin-wait lock
 * - return non-zero on success, zero otherwise
 */
static __inline__ long
spinwttrylk(volatile m_atomic_t *sp, m_atomic_t val, long niter)
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
spinwtlk(volatile m_atomic_t *sp, m_atomic_t val, long niter)
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
spinwtunlk(volatile m_atomic_t *sp)
{
    m_membar();
    *sp = MTSPININITVAL;
    m_endspin();

    return;
}

#endif /* __MT_SPINWT_H__ */

