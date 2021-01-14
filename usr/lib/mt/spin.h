#ifndef __MT_SPIN_H__
#define __MT_SPIN_H__

#include <mt/conf.h>

#if defined(MT_SPINLOCKS)

#include <mt/conf.h>

#include <mt/mt.h>

#if defined(MT_SPINLOCKS)

#if !defined(MTNEWSPIN)
#define MTNEWSPIN               1
#endif

typedef volatile m_atomic_t     mtspin;

#define MT_SPININITVAL          0
#define MT_SPINLKVAL            1

#define spininit(sp)            (*(sp) = MT_SPININITVAL)

/*
 * try to lock spin-lock
 * - return non-zero on success, zero if already locked
 */
static __inline__ long
mttryspin(volatile m_atomic_t *sp)
{
    m_atomic_t                  res = 0;

    if (*sp == MT_SPININITVAL) {
        res = m_cmpswap(sp, MT_SPININITVAL, MT_SPINLKVAL);
    }

    return res;
}

/*
 * acquire spin-lock
 */
static __inline__ long
mtlkspin(volatile m_atomic_t *sp)
{
    m_atomic_t                  res = *sp;

    do {
        while (*sp == MT_SPINLKVAL) {
            m_waitspin();
        }
        res = m_cmpswap(sp, MT_SPININITVAL, MT_SPINLKVAL);
    } while (res);

    return res;
}

/*
 * release spin-lock
 */
static __inline__ void
mtunlkspin(volatile m_atomic_t *sp)
{
    m_membar();
    *sp = MT_SPININITVAL;
    m_endspin();

    return;
}

#endif /* defined(MT_SPINLOCKS) */

#endif /* defined(MT_SPINLOCKS) */

#endif /* __MT_SPIN_H__ */

