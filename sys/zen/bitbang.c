#include <stdint.h>
#include <sys/zen/bitbang.h>

#if defined(ZEN_NEED_CLZ)

static __inline__ int
m_clz32(uint32_t u32)
{
    int         ret = 0;
    uint32_t    mask = 0xffff0000;

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

static__inline__ int
m_clz64(uint64_t u64)
{
    int         ret = 0;
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

#endif /* ZEN_NEED_CLZ */

#if defined(ZEN_NEED_CTZ)

/* count number of trailing (low) zero-bits in u32 */
static __inline__ int
m_ctz32(uint32_t u32)
{
    int                 ret = 0;
    uint32_t            tmp;
    uint32_t            mask;

    if (!u32) {
        ret = 32;
    } else {
        ret = 0;
        tmp = u32;
        mask = 0x01;
        if (!(tmp & mask)) {
            mask = 0xffff;
            if (!(tmp & mask)) {
                tmp >>= 16;
                ret += 16;
            }
            mask >>= 8;
            if (!(tmp & mask)) {
                tmp >>= 8;
                ret += 8;
            }
            mask >>= 4;
            if (!(tmp & mask)) {
                tmp >>= 4;
                ret += 4;
            }
            mask >>= 2;
            if (!(tmp & mask)) {
                tmp >>= 2;
                ret += 2;
            }
            mask >>= 1;
            if (!(tmp & mask)) {
                ret++;
            }
        }
    }

    return ret;
}

/* count number of trailing (low) zero bits in u64 */
static __inline__ int
m_ctz64(uint64_t u64)
{
    int                 ret = 0;
    uint64_t            tmp;
    uint64_t            mask;

    if (!u64) {
        ret = 64;
    } else {
        ret = 0;
        tmp = u64;
        mask = 0x01;
        if (!(tmp & mask)) {
            mask = 0xffffffff;
            if (!(tmp & mask)) {
                tmp >>= 64;
                ret += 64;
            }
            mask = 0xffff;
            if (!(tmp & mask)) {
                tmp >>= 16;
                ret += 16;
            }
            mask >>= 8;
            if (!(tmp & mask)) {
                tmp >>= 8;
                ret += 8;
            }
            mask >>= 4;
            if (!(tmp & mask)) {
                tmp >>= 4;
                ret += 4;
            }
            mask >>= 2;
            if (!(tmp & mask)) {
                tmp >>= 2;
                ret += 2;
            }
            mask >>= 1;
            if (!(tmp & mask)) {
                ret++;
            }
        }
    }

    return ret;
}

#endif /* ZEN_NEED_CTZ */

#if defined(ZEN_NEED_HAM)

static __inline__ uint32_t
m_ham32(uint32_t u32)
{
    /* dX == (~0) / X */
    uint32_t d3 = 0x55555555;
    uint32_t d5 = 0x33333333;
    uint32_t d17 = 0x0f0f0f0f;

    u32 -= (u32 >> 1) & d3;
    u32 = (u32 & d5) + ((u32 >> 2) & d5);
    u32 = (u32 + (u32 >> 4)) & d17;
    u32 += u32 >> 8;
    u32 += u32 >> 16;
    u32 &= 0x7f;

    return u32;
}

static __inline__ uint32_t
m_ham64(uint64_t a)
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

#endif /* ZEN_NEED_HAM */

