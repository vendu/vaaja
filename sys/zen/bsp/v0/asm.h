#ifndef __MACH_V0_ASM_H__
#define __MACH_V0_ASM_H__

#include <stdint.h>
#include <zen/cdefs.h>

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

static C_INLINE int32_t
m_atomread(const m_atomic32_t *ap32)
{
    m_atomic32_t                res = (volatile m_atomic32_t *)ap32;

    return res;
}

static C_INLINE void
m_atomwrite(const m_atomic32_t *ap32, int32_t i32)
{
    (volatile m_atomic32_t *)ap32 = i32;

    return;
}

static C_INLINE void
m_atomadd(const m_atomic32_t *ap32, int32_t i32)
{
    __asm__ __volatile__ ("xadd %1, %0\n"
                          : "m+" (*(ap32))
                            "ri" (i));

    return;
}

static C_INLINE void
m_atomsub(const m_atomic32_t *ap32, int32_t i32)
{
    __asm__ __volatile__ ("xsub %1, %0\n"
                          : "m+" (*(ap32))
                            "ri" (i));

    return;
}

static C_INLINE void
m_atominc(const m_atomic32_t *ap32)
{
    __asm__ __volatile__ ("xinc %0\n"
                          : "m+" (*(ap32)));

    return;
}

static C_INLINE void
m_atomdec(const m_atomic32_t *ap32)
{
    __asm__ __volatile__ ("xinc %0\n"
                          : "m+" (*(ap32)));

    return;
}

static C_INLINE int32_t
m_atomchkaddneg(const m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xadd %1, %0, %2\n"
                          : "m+" (*(ap32))
                          : "ri" (i)
                          : "r" (val));

    return (val < 0);
}

static C_INLINE int32_t
m_atomchksub(const m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xsub %1, %0, %2\n"
                          : "m+" (*(ap32))
                          : "ri" (i)
                          : "r" (val));

    return !0;
}

static C_INLINE long
m_atomchkinc(const m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xinc %0, %1\n"
                          : "m+" (*(ap32))
                          : "r" (val));

    return !0;
}

static C_INLINE long
m_atomaddret(m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xadd %1, %0, %2\n"
                          : "m+" (*(ap32))
                          : "ri" (i)
                          : "r" (val));

    return val;
}

static C_INLINE long
m_atomsubret(m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xsub %1, %0, %2\n"
                          : "m+" (*(ap32))
                          : "ri" (i)
                          : "r" (val));

    return val;
}

## long m_atomtryadd(m_atomic_t *ap, int i, int v);
- atomically add a to *ap iff *ap != v. return old *ap.

m_atomchkdec(const m_atomic32_t *ap32, int32_t i)
{
    int32_t val;

    __asm__ __volatile__ ("xdec %0, %1\n"
                          : "m+" (*(ap32))
                          : "r" (val));

    return !0;
}

/* compare-and-swap */
#define m_cmpswap(a32p, want, need)                                     \
    (*(a32p) == (want) ? *(a32p) = need, 1 : 0)
#define m_cmpswapu32(au32p, want, need)                                 \
    (*(au32p) == (want) ? *(au32p) = need, 1 : 0)

/* fetch-and-add */

static C_INLINE uint16_t
m_fetchadd(volatile m_atomic32_t *a32p, m_atomic32_t delta)
{
    m_atomic32_t        val = *a32p;
    m_atomic32_t        sum = val + delta;

    *a32p = sum;

    return val;
}

static C_INLINE uint16_t
m_fetchaddu16(volatile m_atomicu16_t *au16p, uint16_t u16)
{
    m_atomicu16_t       val = *au16p;
    m_atomicu16_t       sum = val + u16;

    *au16p = sum;

    return val;
}

static C_INLINE m_atomic32_t
m_cmpsetbit(volatile m_atomic32_t *a32p, int32_t ndx)
{
    m_atomic32_t        val = *a32p;
    m_atomic32_t        bit = INT32_C(1) << ndx;

    while (val & bit) {
        if (!(val & bit)) {
            val |= bit;
            *a32p = val;

            break;
        }
    }

    return val;
}

static C_INLINE void
m_clrbit(volatile m_atomic32_t *a32p, int32_t ndx)
{
    m_atomic32_t        val = *a32p;
    m_atomic32_t        bit = INT32_C(1) << ndx;

    val &= ~bit;

    *a32p = val;

    return;
}

#endif /* __MACH_V0_ASM_H__ */

