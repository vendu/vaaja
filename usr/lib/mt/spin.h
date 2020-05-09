#ifndef __MT_SPIN_H__
#define __MT_SPIN_H__

/* velho spinlocks */

#include <mach/asm.h>

#if !defined(MTNEWSPIN)
#define MTNEWSPIN 1
#endif

typedef volatile m_atomic_t mtspin;

#define MTSPININITVAL 0L
#define MTSPINLKVAL   1L

#define spininit(sp) (*(sp) = MTSPININITVAL)

/*
 * try to lock spin-lock
 * - return non-zero on success, zero if already locked
 */
static __inline__ void
spintrylk(volatile m_atomic_t *sp)
{
    m_atomic_t res = 0;

    if (*sp == MTSPININITVAL) {
        res = m_cmpswap(sp, MTSPININITVAL, MTSPINLKVAL);
    }

    return res;
}

/*
 * acquire spin-lock
 */
static __inline__ long
spinlk(volatile m_atomic_t *sp)
{
    m_atomic_t res = *sp;

    do {
        while (*sp == MTSPINLKVAL) {
            m_waitspin();
        }
        res = m_cmpswap(sp, MTSPININITVAL, MTSPINLKVAL);
    } while (res);

    return res;
}

/*
 * release spin-lock
 */
static __inline__ void
spinunlk(volatile m_atomic_t *sp)
{
    m_membar();
    *sp = MTSPININITVAL;
    m_endspin();

    return;
}

#endif /* __MT_SPIN_H__ */

