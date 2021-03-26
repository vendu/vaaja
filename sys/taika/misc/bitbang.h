#ifndef ZEN_BITBANG_H
#define ZEN_BITBANG_H

#define ZEN_BITBANG_DEBRUIJN    1
#if defined(ZEN_BITBANG_DEBRUIJN)
#define ZEN_DEBRUIJN_CLZ        1
#define ZEN_DEBRUIJN_CTZ        1
#endif

#if (defined(__GNUC__) || defined(__clang__))

#undef ZEN_NEED_CLZ
#undef ZEN_NEED_CTZ
#undef ZEN_NEED_HAM

#define m_clz32(x)              ((x) ? __builtin_clz(x) : 32)
#define m_clz64(x)              ((x) ? __builtin_clzll(x) : 64)
#define m_ctz32(x)              ((x) ? __builtin_ctz(x) : 32)
#define m_ctz64(x)              ((x) ? __builtin_ctzll(x) : 64)
#define m_ham32(x)              __builtin_popcount(x)
#define m_ham64(x)              __builtin_popcountll(x)
#define bswap16(x)              __builtin_bswap16(x)
#define bswap32(x)              __builtin_bswap32(x)
#define bswap64(x)              __builtin_bswap64(x)

#else // ((defined(__GNUC__ || defined(__clang__))) && !defined(ZEN_BITBANG_DEBRUIJN))

#define ZEN_NEED_CLZ            1
#define ZEN_NEED_CTZ            1
#define ZEN_NEED_HAM            1

#define bswap16(u)                                                      \
    ((((u) & 0xff00U) >> 8)                                             \
     | (((u) & 0x00ffU) << 8))
#define bswap32(u)                                                      \
    ((((u) & UINT32_C(0xff000000U)) >> 24)                              \
     | (((u) & UINT32_C(0x00ff0000U)) >> 8)                             \
     | (((u) & UINT32_C(0x0000ff00U)) << 8)                             \
     | (((u) & UINT32_C(0x000000ffU)) << 24))
#define bswap64(u)                                                      \
    ((((u) & UINT64_C(0xff00000000000000)) >> 56)                       \
     | (((u) & UINT64_C(0x00ff000000000000)) >> 40)                     \
     | (((u) & UINT64_C(0x0000ff0000000000)) >> 24)                     \
     | (((u) & UINT64_C(0x000000ff00000000)) >> 8)                      \
     | (((u) & UINT64_C(0x00000000ff000000)) << 8)                      \
     | (((u) & UINT64_C(0x0000000000ff0000)) << 24)                     \
     | (((u) & UINT64_C(0x000000000000ff00)) << 40)                     \
     | (((u) & UINT64_C(0x00000000000000ff)) << 56))

#endif /* defined(__GNUC__) || defined(__clang__) */

/* get the lowest 1-bit in a */
#define lo1bit(a)           ((a) & -(a))
/* get n lowest and highest bits of i */
#define lobits(i, n)        ((i) & ((1U << (n)) - 0x01))
#define hibits(i, n)        ((i) & ~((1U << (sizeof(i) * CHAR_BIT - (n))) - 0x01))
/* get n bits starting from index j */
#define getbits(i, j, n)    (lobits((i) >> (j), (n)))
/* set n bits starting from index j to value b */
#define setbits(i, j, n, b) ((i) |= (((b) << (j)) & ~(((1UL << (n)) << (j)) - 0x01)))
#define bitset(p, b)        (((uint8_t *)(p))[(b) >> 3] & (1UL << ((b) & 0x07)))
/* set bit # b in *p */
#if !defined(setbit)
#define setbit(p, b)        (((uint8_t *)(p))[(b) >> 3] |= (1UL << ((b) & 0x07)))
#endif
/* clear bit # b in *p */
#if !defined(clrbit)
#define clrbit(p, b)        (((uint8_t *)(p))[(b) >> 3] &= ~(1UL << ((b) & 0x07)))
#endif
/* m - mask of bits to be taken from b. */
#define mergebits(a, b, m)  ((a) ^ (((a) ^ (b)) & (m)))
/* m - mask of bits to be copied from a. 1 -> copy, 0 -> leave alone. */
#define copybits(a, b, m) (((a) | (m)) | ((b) & ~(m)))

/* FIXME: test min2() and max2() */

#define min(a, b)  ((a) <= (b) ? (a) : (b))
#define max(a, b)  ((a) >= (b) ? (a) : (b))

#define min2(a, b) ((b) ^ (((a) ^ (b)) & -((a) < (b))))
#define max2(a, b) ((a) ^ (((a) ^ (b)) & -((a) < (b))))

/* compute minimum and maximum of a and b without branching */
#define min3(a, b)                              \
    ((b) + (((a) - (b)) & -((a) < (b))))
#define max3(a, b)                              \
    ((a) - (((a) - (b)) & -((a) < (b))))

#define sign(x, nb)                                     \
    ((x) = (x) & ((1U < (nb)) - 1),                     \
     ((x) ^ (1U << ((nb) - 1))) - (1U << ((nb) - 1)))

/* compare with power-of-two p2 */
#define gtpow2(u, p2)  /* true if u > p2 */     \
    ((u) & ~(p2))
#define gtepow2(u, p2) /* true if u >= p2 */    \
    ((u) & -(p2))

/* true if x is a power of two */
#if !defined(powerof2)
#define powerof2(x)     (!((x) & ((x) - 1)))
#endif
/* align a to boundary of (the power of two) b2. */
#define align1(a, b2)   ((a) & ~((b2) - 1))
#define align2(a, b2)   ((a) & -(b2))
#define modpow2(a, b2)  ((a) & ((b2) - 1))

/* round a up to the next multiple of (the power of two) b2. */
//#define rounduppow2(a, b2) (((a) + ((b2) - 0x01)) & ~((b2) + 0x01))
#define roundup2(a, b2) (((a) + ((b2) - 0x01)) & -(b2))
/* round down to the previous multiple of (the power of two) b2 */
#define rounddown2(a, b2) ((a) & ~((b2) - 0x01))

#if defined(__GNUC__)
#define _roundup2(a, b)                         \
    ((__builtin_constant_p(b) && powerof2(b))   \
     ? rounduppow2(a, b)                        \
     : ((((a) + ((b) - 1)) / (b)) * b))
#elif 0
#define roundup2b(a, b)                         \
    ((((a) + ((b) - 1)) / (b)) * (b))
#endif /* !defined(__GNUC__) */

/* compute the average of a and b without division */
#define uavg(a, b)      (((a) & (b)) + (((a) ^ (b)) >> 1))

#define divceil(a, b)   (((a) + (b) - 1) / (b))
#define divround(a, b)  (((a) + ((b) / 2)) / (b))

#define haszero32(a)    (((a) - 0x01010101) & ~(a) & 0x80808080)

/* division and modulus for a few chosen values */

static __inline__ unsigned long
divu10(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0xcd);
        unsigned long long res = uval;
        unsigned long      cnt = 3;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu10(uval)            (uval - uval * divu10(uval)))

unsigned long
divu12(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0xab);
        unsigned long long res = uval;
        unsigned long      cnt = 3;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu12                  (uval - uval * divu12(uval))

static __inline__ unsigned long
divu30(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0x89);
        unsigned long long res = uval;
        unsigned long      cnt = 4;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu30(uval)            (uval - uval * divu30(uval)))

static __inline__ unsigned long
divu60(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0x89);
        unsigned long long res = uval;
        unsigned long      cnt = 5;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu60(uval)            (uval - uval * divu60(uval)))

static __inline__ unsigned long
divu100(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0x29);
        unsigned long long res = uval;
        unsigned long      cnt = 4;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu100(uval)           (uval - uval * divu100(uval)))

unsigned long
divu400(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0x29);
        unsigned long long res = uval;
        unsigned long      cnt = 6;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}
#define modu400(uval)           (uval - uval * divu400(uval)))

#endif /* ZEN_BITBANG_H */

