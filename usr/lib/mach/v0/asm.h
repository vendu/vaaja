#ifndef __MACH_V0_ASM_H__
#define __MACH_V0_ASM_H__

#include <stdint.h>
#include <zero/cdefs.h>

/* DUMMY NON-ATOMIC ROUTINES - FIXME */

/* full memory barrier */
#define m_membar()   __asm__ __volatile__ ("": : : "memory")
/* memory read barrier */
#define m_memrdbar() __asm__ __volatile__ ("" : : : "memory")
/* memory write barrier */
#define m_memwrbar() __asm__ __volatile__ ("" : : : "memory")
/* delay exection; light-weight sleep */
#define m_waitspin() __asm__ __volatile__ ("" : : : "memory")
#define m_endspin()
/* wait for an interrupt */
#define m_waitint()  __asm__ __volatile__ ("" : : : "memory")

/* compare-and-swap */
#define m_cmpswap(lp, want, need)                                     \
    (*(lp) == (want) ? *(lp) = need, 1 : 0)
#define m_cmpswapu(up, want, need)                                  \
    (*(up) == (want) ? *(up) = need, 1 : 0)

/* fetch-and-add */

static C_INLINE uint16_t
m_fetchadd(volatile m_atomic_t *lp, m_atomic_t delta)
{
    m_atomic_t  val = *lp;
    m_atomic_t  sum = val + delta;

    *lp = sum;

    return val;
}

static C_INLINE uint16_t
m_fetchaddu16(volatile m_atomicu16_t *u16p, uint16_t u16)
{
    m_atomicu16_t   val = *u16p;
    m_atomicu16_t   sum = val + u16;

    *u16p = sum;

    return val;
}

static C_INLINE m_atomic_t
m_cmpsetbit(volatile m_atomic_t *lp, int32_t ndx)
{
    m_atomic_t  val = *lp;
    m_atomic_t  bit = INT32_C(1) << ndx;

    while (val & bit) {
        if (!(val & bit)) {
            val |= bit;
            *lp = val;

            break;
        }
    }

    return val;
}

static C_INLINE void
m_clrbit(volatile m_atomic_t *lp, int32_t ndx)
{
    m_atomic_t  val = *lp;
    m_atomic_t  bit = INT32_C(1) << ndx;

    val &= ~bit;
    *lp = val;

    return;
}

#endif /* __MACH_V0_ASM_H__ */

