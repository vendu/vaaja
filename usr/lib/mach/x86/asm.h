#ifndef __MACH_X86_ASM_H__
#define __MACH_X86_ASM_H__

/* libmach header file for 32- and 64-bit x86 architectures */

//#define frameisusr(tcb) ((tcb)->frame.cs == UTEXTSEL)

/* memory barrier inline-assembly macros */
/* full memory barrier */
#define m_membar()   __asm__ __volatile__ ("mfence\n" : : : "memory")
/* memory read barrier */
#define m_memrdbar() __asm__ __volatile__ ("lfence\n" : : : "memory")
/* memory write barrier */
#define m_memwrbar() __asm__ __volatile__ ("sfence\n" : : : "memory")
/* delay exection; light-weight sleep */
#define m_waitspin() __asm__ __volatile__ ("pause\n"  : : : "memory")
#define m_endspin()
/* wait for an interrupt */
#define m_waitint()  __asm__ __volatile__ ("hlt\n"  : : : "memory")

#include <stdint.h>
#include <zero/cdefs.h>
#if defined(__x86_64__) || defined(__amd64__)
#include <mach/x86-64/asm.h>
#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#include <mach/ia32/asm.h>
#endif

/* atomic fetch and add, 16-bit version; return earlier value */
#define m_fetchadd16(p, val)       m_xadd16(p, val)
#define m_fetchaddu16(p, val)      m_xaddu16(p, val)
/* atomic fetch and add, 32-bit version; return earlier value */
#define m_fetchadd32(p, val)       m_xadd32(p, val)
#define m_fetchaddu32(p, val)      m_xaddu32(p, val)
/* atomic compare and swap byte *lp with val iff *lp == want */
#define m_cmpswapb(p, want, val)   (m_cmpxchg8(p, want, val) == want)
#define m_cmpswap32(p, want, val)  (m_cmpxchg32(p, want, val) == want)
#define m_cmpswapu32(p, want, val) (m_cmpxchgu32(p, want, val) == want)

/* atomic increment operation */
static __inline__ void
m_atominc32(volatile m_atomic32_t *lp)
{
    __asm__ __volatile__ ("lock incl %0\n"
                          : "+m" (*(lp))
                          :
                          : "memory");

    return;
}

/* atomic decrement operation */
static __inline__ void
m_atomdec32(volatile m_atomic32_t *lp)
{
    __asm__ __volatile__ ("lock decl %0\n"
                          : "+m" (*(lp))
                          :
                          : "memory");

    return;
}

/* atomic exchange operation */
static __inline__ int32_t
m_xchg32(volatile m_atomic32_t *lp,
         int32_t val)
{
    int32_t res;

    __asm__ __volatile__ ("lock xchgl %0, %2\n"
                          : "+m" (*lp), "=a" (res)
                          : "r" (val)
                          : "cc", "memory");

    return res;
}

/*
 * atomic add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomic16_t
m_add16(volatile m_atomic16_t *lp,
         m_atomic16_t val)
{
    __asm__ __volatile__ ("lock addw %1, %w0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomicu16_t
m_addu16(volatile m_atomicu16_t *lp,
          m_atomicu16_t val)
{
    __asm__ __volatile__ ("lock addw %1, %w0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ int32_t
m_add32(volatile m_atomic32_t *lp,
         int32_t val)
{
    __asm__ __volatile__ ("lock addl %1, %0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomicu32_t
m_addu32(volatile m_atomicu32_t *lp,
          m_atomicu32_t val)
{
    __asm__ __volatile__ ("lock addl %1, %0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic fetch and add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomic16_t
m_xadd16(volatile m_atomic16_t *lp,
         m_atomic16_t val)
{
    __asm__ __volatile__ ("lock xaddw %1, %w0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic fetch and add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomicu16_t
m_xaddu16(volatile m_atomicu16_t *lp,
          m_atomicu16_t val)
{
    __asm__ __volatile__ ("lock xaddw %1, %w0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic fetch and add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ int32_t
m_xadd32(volatile m_atomic32_t *lp,
         int32_t val)
{
    __asm__ __volatile__ ("lock xaddl %1, %0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic fetch and add
 * - let *lp = *lp + val
 * - return original *lp
 */
static __inline__ m_atomicu32_t
m_xaddu32(volatile m_atomicu32_t *lp,
          m_atomicu32_t val)
{
    __asm__ __volatile__ ("lock xaddl %1, %0\n"
                          : "+m" (*(lp)), "=a" (val)
                          :
                          : "cc", "memory");

    return val;
}

/*
 * atomic compare and exchange longword
 * - if *lp == want, let *lp = val
 * - return nonzero on success, zero on failure
 */
static __inline__ int32_t
m_cmpxchg32(volatile m_atomic32_t *lp,
            int32_t want,
            int32_t val)
{
    int32_t res;

    __asm__ __volatile__("lock cmpxchgl %1, %2\n"
                         : "=a" (res)
                         : "q" (val), "m" (*(lp)), "0" (want)
                         : "memory");

    return res;
}

/*
 * atomic compare and exchange unsigned longword
 * - if *lp == want, let *lp = val
 * - return nonzero on success, zero on failure
 */
static __inline__ m_atomicu32_t
m_cmpxchgu32(volatile m_atomicu32_t *lp,
             m_atomicu32_t want,
             m_atomicu32_t val)
{
    m_atomicu32_t res;

    __asm__ __volatile__ ("lock cmpxchgl %1, %2\n"
                          : "=a" (res)
                          : "q" (val), "m" (*(lp)), "0" (want)
                          : "memory");

    return res;
}

/* atomic set bit operation */
static C_INLINE void
m_setbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    m_membar();
    __asm__ __volatile__ ("lock btsl %1, %0\n"
                          : "=m" (*(lp))
                          : "Ir" (ndx)
                          : "memory");

    return;
}

/* atomic reset/clear bit operation */
static C_INLINE void
m_clrbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t mask = ~(INT32_C(1) << ndx);

    m_membar();
    __asm__ __volatile__ ("lock andl %1, %0\n"
                          : "=m" (*((uint8_t *)(lp) + (ndx >> 3)))
                          : "Ir" (mask));

    return;
}

/* atomic flip/toggle bit operation */
static C_INLINE void
m_flipbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t bit = INT32_C(1) << ndx;

    m_membar();
    __asm__ __volatile__ ("lock orl %1, %0\n"
                          : "=m" (*((uint8_t *)(lp) + (ndx >> 3)))
                          : "Ir" (bit));

    return;
}

/* atomic set and test bit operation; returns the old value */
static __inline__ int32_t
m_cmpsetbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t val = 0;

    if (IMMEDIATE(ndx)) {
        __asm__ __volatile__ ("lock btsl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "i" (ndx)
                              : "cc", "memory");
    } else {
        __asm__ __volatile__ ("lock btsl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "r" (ndx)
                              : "cc", "memory");
    }

    return val;
}

/* atomic clear bit operation */
static __inline__ int32_t
m_cmpclrbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t val = 0;

    if (IMMEDIATE(ndx)) {
        __asm__ __volatile__ ("lock btrl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "i" (ndx)
                              : "cc", "memory");
    } else {
        __asm__ __volatile__ ("lock btrl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "r" (ndx)
                              : "cc", "memory");
    }

    return val;
}

#if defined(__GNUC__)
#define m_atomor(p, val)  __sync_or_and_fetch((p), (val))
#define m_atomand(p, val) __sync_and_and_fetch((p), (val))
#endif

/*
 * atomic compare and exchange byte
 * - if *lp == want, let *lp = val
 * - return original *lp
 */
static __inline__ int8_t
m_cmpxchg8(volatile m_atomic64_t *lp,
           int8_t want,
           int8_t val)
{
    long res;

    __asm__ __volatile__ ("lock cmpxchgb %b1, %2\n"
                          : "=a" (res)
                          : "q" (val), "m" (*(lp)), "0" (want)
                          : "memory");

    return res;
}

static __inline__ int32_t
m_bsf32(m_atomicu32_t val)
{
    int32_t ret = ~0;

    __asm__ __volatile__ ("bsfl %1, %0\n" : "=r" (ret) : "rm" (val));

    return ret;
}

static __inline__ int32_t
m_bsr32(m_atomicu32_t val)
{
    int32_t ret = ~0;

    __asm__ __volatile__ ("bsrl %1, %0\n" : "=r" (ret) : "rm" (val));

    return ret;
}

/*
 * atomic compare and exchange pointer
 * - if *lp == want, let *lp = val
 * return nonzero on success, zero on failure
 */
static __inline__ void *
m_cmpxchg32ptr(m_atomic32_t **lp,
               m_atomic32_t *want,
               m_atomicptr_t *val)
{
    void *res;

    __asm__ __volatile__("lock cmpxchgl %1, %2\n"
                         : "=a" (res)
                         : "q" (val), "m" (*(lp)), "0" (want)
                         : "memory");

    return res;
}

#if !defined(__x86_64__) && !defined(__amd64__)

#if defined(__GNUC__) && 0

/*
 * atomic 64-bit compare and swap
 * - if *lp == want, let *lp = val
 * - return original nonzero on success, zero on failure
 */
static __inline__ long
m_cmpxchg64(int64_t *lp64,
            int64_t *want,
            int64_t *val)
{
    return __sync_bool_compare_and_swap(p64, want, val);
}

#elif defined(_MSC_VER)

static __inline__ long
m_cmpxchg64(int64_t *lp64,
            int64_t *want,
            int64_t *val)
{
    long long ask = *want;
    long long say = *val;
    long long res;

    res = InterlockedCompareExchange64(p64, say, ask);

    return res;
}

#else

/*
 * atomic 64-bit compare and swap
 * - if *lp == want, let *lp = val + set ZF
 *   - else let EDX:EAX = *lp + clear ZF
 * - return 0 on failur, 1 on success

 */
static __inline__ long
m_cmpxchg64(int64_t *ptr,
            int64_t want,
            int64_t val)
{
    long     res = 0;
    register int32_t ebx __asm__ ("ebx") = want & 0xffffffff;

    __asm__ __volatile (
        "movl %%edi, %%ebx\n" // load EBX
        "lock cmpxchg8b (%%esi)\n"
        "setz %%al\n" // EAX may change
        "movzx %%al, %1\n" // res = ZF
        : "+S" (ptr), "=a" (res)
        : "0" (ptr),
          "d" ((uint32_t)(want >> 32)),
          "a" ((uint32_t)(want & 0xffffffff)),
          "c" ((uint32_t)(val >> 32)),
          "D" ((uint32_t)(val & 0xffffffff))
        : "flags", "memory", "eax", "edx");

    return res;
}

#endif

#endif /* !64-bit */

#endif /* __MACH_X86_ASM_H__ */

