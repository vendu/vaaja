#ifndef __ZEN_BITOP_H__
#define __ZEN_BITOP_H__

#include <stdint.h>
#include <mach/param.h>

/*
 * these bitcnt-routines are from http://bisqwit.iki.fi/source/misc/bitcounting/
 */

#if defined(__GNUC__) || defined(__clang__)

#define clz32(u32)      __builtin_clz(u32)
#define clz64(u64)      __builtin_clzll(u64)
#define ctz32(u32)      __builtin_ctz(u32)
#define ctz64(u64)      __builtin_ctzll(u64)
#define ham32(u32)      __builtin_popcount(u32)
#define ham64(u64)      __builtin_popcountll(u64)
#define par32(u32)      __builtin_parity(u32)
#define par64(u32)      __builtin_parityll(u64)
#define bswap16(u16)    __builtin_bswap16(u16)
#define bswap32(u32)    __builtin_bswap32(u32)
#define bswap64(u64)    __builtin_bswap64(u64)

#else

static __inline__ int
clz32(uint32_t u32)
{
    int         ret = 0;
    uint32_t    mask = 0xffff0000;

    if (u32) {
        if (!(u32 & mask)) {
            ret += 16;
            u32 <<= 16;
        }
        mask <<= 8;
        if (!(u32 & mask)) {
            ret += 8;
            u32 <<= 8;
        }
        mask <<= 4;
        if (!(u32 & mask)) {
            ret += 4;
            u32 <<= 4;
        }
        mask <<= 2;
        if (!(u32 & mask)) {
            ret += 2;
            u32 <<= 2;
        }
        mask <<= 1;
        if (!(u32 & mask)) {
            ret++;
        }
    } else {
        ret = 32;
    }

    return ret;
}

static__inline__
clz64(uint64_t u64)
{
    int         ret;
    uint64_t    mask = UINT64_C(0xffffffff00000000);

    if (u64) {
        if (!(u64 & mask)) {
            ret += 32;
            u64 <<= 32;
        }
        mask <<= 16;
        if (!(u64 & mask)) {
            ret += 16;
            u64 <<= 16;
        }
        mask <<= 8;
        if (!(u64 & mask)) {
            ret += 8;
            u64 <<= 8;
        }
        mask <<= 4;
        if (!(u64 & mask)) {
            ret += 4;
            u64 <<= 4;
        }
        mask <<= 2;
        if (!(u64 & mask)) {
            ret += 2;
            u64 <<= 2;
        }
        mask <<= 1;
        if (!(u64 & mask)) {
            ret++;
        }
    } else {
        ret = 32;
    }

    return ret;
}

static __inline__ uint32_t
ham32mul(uint32_t a)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;
    uint32_t d255 = 0x01010101;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a = (a * d255) >> 24;

    return a;
}

static __inline__ uint32_t
ham32nomul(uint32_t a)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a += a >> 8;
    a += a >> 16;
    a &= 0x7f;

    return a;
}

static __inline__ uint32_t
ham64mul(uint32_t a)
{
    /* dX == (~0) / X */
    uint64_t d3 = 0x5555555555555555;
    uint64_t d5 = 0x3333333333333333;
    uint64_t d17 = 0x0f0f0f0f0f0f0f0f;
    uint64_t d255 = 0x0101010101010101;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a = (a * d255) >> 56;

    return a;
}

static __inline__ uint32_t
ham64nomul(uint64_t a)
{
    /* dX == (~0) / X */
    uint64_t d3 = 0x5555555555555555;
    uint64_t d5 = 0x3333333333333333;
    uint64_t d17 = 0x0f0f0f0f0f0f0f0f;

    a -= (a >> 1) & d3;
    a = (a & d5) + ((a >> 2) & d5);
    a = (a + (a >> 4)) & d17;
    a += a >> 8;
    a += a >> 16;
    a += a >> 32;
    a &= 0x7f;

    return a;
}

#if (MACH_WORD_SIZE == 8)
#define m_ham ham64nomul
#else
#define m_ham ham32nomul
#endif

#endif

#if (MACH_WORD_SIZE == 8)
#define m_clz clz64
#define m_ctz ctz64
#define m_ham ham64
#else
#define m_clz clz32
#define m_ctz ctz32
#define m_ham ham32
#endif

#endif /* __ZEN_BITOP_H__ */

