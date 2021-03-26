#ifndef SYS_ZEN_INTERNAL_MACH_IA32_MTX_H
#define SYS_ZEN_INTERNAL_MACH_IA32_MTX_H

#include "asm.h"

/*
 * try to acquire a mutex-lock without blocking
 * - return non-zero on success, 0 on failure
*/
static C_INLINE int
m_trymtx(m_atomic_t *volatile vap, int32_t val)
{
    int                         res = __sync_bool_compare_and_swap(vap,
                                                                   0,
                                                                   val);

    return res;
}

static C_INLINE int
m_lkmtx(m_atomic_t *volatile vap, m_atomic_t val)
{
    int res = 1;

    do {
        while (*vap) {
            m_waitspin();
        }
        res = m_trymtx(vap, 1);
    } while (!res);

    return res;
}

#define m_unlkmtx(vap)          __sync_lock_release(vap)

#endif /* SYS_ZEN_INTERNAL_MACH_IA32_MTX_H */

