#ifndef __MACH_IA32_ASM_H__
#define __MACH_IA32_ASM_H__

#include <stdint.h>
#include <zero/cdefs.h>

#define __EIPFRAMEOFS                4

#define m_atominc(p)                 m_atominc32(p)
#define m_atomdec(p)                 m_atomdec32(p)
#define m_atomswap(p, val)           m_xchg32(p, val)
#define m_fetchadd(p, val)           m_xadd32(p, val)
#define m_fetchswap(p, want, val)    m_cmpxchg32(p, want, val)
#define m_fetchswapu(p, want, val)   m_cmpxchgu32(p, want, val)
#define m_fetchswapptr(p, want, val) m_cmpxchg32ptr(p, want, val)
#define m_cmpswap(p, want, val)      (m_cmpxchg32(p, want, val) == want)
#define m_cmpswapu(p, want, val)     (m_cmpxchgu32(p, want, val) == want)
#define m_cmpswapptr(p, want, val)   (m_cmpxchg32ptr(p, want, val) == want)
#define m_cmpswapdbl(p, want, val)   (m_cmpxchg64(p, val))
#define m_setbit(p, ndx)             m_setbit32(p, ndx)
#define m_clrbit(p, ndx)             m_clrbit32(p, ndx)
#define m_flipbit(p, ndx)            m_flipbit32(p, ndx)
#define m_cmpsetbit(p, ndx)          m_cmpsetbit32(p, ndx)
#define m_cmpclrbit(p, ndx)          m_cmpclrbit32(p, ndx)
#define m_scanlo1bit(l)              m_bsf32(l)
#define m_scanhi1bit(l)              m_bsr32(l)

static INLINE void *
m_getretadr(void)
{
    void *ptr;

    __asm__ __volatile__ ("movl %c1(%%ebp), %0\n"
                          : "=r" (ptr)
                          : "i" (__EIPFRAMEOFS));
    return ptr;
}

static INLINE void *
m_getfrmadr(void)
{
    void *ptr;

    __asm__ __volatile__ ("movl %%ebp, %0\n" : "=r" (ptr));

    return ptr;
}

static INLINE void *
m_getfrmadr1(void **fp)
{
    void *ptr;

    if (fp) {
        ptr = *fp;
    } else {
        ptr = m_getfrmadr();
    }

    return ptr;
}

static INLINE void *
m_getretfrm(void)
{
    void *ptr;

    __asm__ __volatile__ ("movl *%%ebp, %0\n" : "=r" (ptr));

    return ptr;
}

static INLINE void *
m_loadretadr(void *frm)
{
    void *ptr;

    __asm__ __volatile__ ("movl %c1(%2), %0\n"
                          : "=r" (ptr)
                          : "i" (__EIPFRAMEOFS), "r" (frm));

    return ptr;
}

#endif /* __MACH_IA32_ASM_H__ */

