#ifndef DECK_LIB_ASM_IA32_H
#define DECK_LIB_ASM_IA32_H

typedef int                         m_atomic_t;
typedef int                         m_atomic32_t;

#define m_atomread(src)             (*(volatile m_atomic_t *)(src))
#define m_atomwrite(dest, src)      (*(dest) = *(volatile m_atomic_t *)(src))

/* memory barrier inline-assembly macros */
/* full memory barrier */
#define m_membar()                  __asm__ __volatile__ ("mfence\n"    \
                                                          :             \
                                                          :             \
                                                          : "memory")
/* memory read barrier */
#define m_memrdbar()                __asm__ __volatile__ ("lfence\n"    \
                                                          :             \
                                                          :             \
                                                          : "memory")
/* memory write barrier */
#define m_memwrbar()                __asm__ __volatile__ ("sfence\n"    \
                                                          :             \
                                                          :             \
                                                          : "memory")

/* delay exection; light-weight sleep */
#define m_waitspin()                __asm__ __volatile__ ("pause\n"     \
                                                          :             \
                                                          :             \
                                                          : "memory")
#define m_endspin()

#define m_setbit(p, ndx)            m_setbit32(p, ndx)
#define m_clrbit(p, ndx)            m_clrbit32(p, ndx)
#define m_atominc(p)                m_atominc32(p)
#define m_atomdec(p)                m_atomdec32(p)
#define m_atomswap(p, val)          m_xchg32(p, val)
#define m_fetchadd(p, val)          m_xadd32(p, val)
#define m_fetchswap(p, want, val)   m_cmpxchg32(p, want, val)
#define m_cmpswap(p, want, val)     (m_cmpxchg32(p, want, val) == want)
#define m_cmpswapdbl(p, want, val)  (m_cmpxchg64(p, val))
#define m_cmpsetbit(p, ndx)         m_cmpsetbit32(p, ndx)
#define m_cmpclrbit(p, ndx)         m_cmpclrbit32(p, ndx)

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
 * atomic fetch and subtract
 * - let *lp = *lp - val
 * - return original *lp
 */
static __inline__ int32_t
m_xsub32(volatile m_atomic32_t *lp,
         int32_t val)
{
    __asm__ __volatile__ ("lock xsubl %1, %0\n"
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

/* atomic set and test bit operation; returns the old value */
static __inline__ int32_t
m_cmpsetbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t val = 0;

    if (C_IMMEDIATE(ndx)) {
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

/* atomic set and test bit operation; returns the old value */
static __inline__ int32_t
m_cmpclrbit32(volatile m_atomic32_t *lp, int32_t ndx)
{
    int32_t val = 0;

    if (C_IMMEDIATE(ndx)) {
        __asm__ __volatile__ ("lock btcl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "i" (ndx)
                              : "cc", "memory");
    } else {
        __asm__ __volatile__ ("lock btcl %2, %0\n"
                              "jnc 1f\n"
                              "incl %1\n"
                              "1:\n"
                              : "+m" (*(lp)), "=r" (val)
                              : "r" (ndx)
                              : "cc", "memory");
    }

    return val;
}

#endif /* DECK_LIB_ASM_IA32_H */

