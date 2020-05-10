#include <stdint.h>
#include <zero/cdefs.h>

#define V0_BIT_MAGIC_CTZ32 0x0ef96a62

static const uint8_t    _bitclz32tab16[16]
= {
    4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint8_t    _bitctz32tab32[32] = {
    31, 0, 27, 1, 28, 13, 23, 2, 29, 21, 19, 14, 24, 16, 3, 7,
    30, 26, 12, 22, 20, 18, 15, 6, 25, 11, 17, 5, 10, 4, 9, 8
};

static INLINE long
m_clz32(uint32_t u32)
{
    uint32_t    cnt = 32;

    if (u32) {
        cnt = 0;
        if (!(u32 & 0xffff0000)) {
            cnt += 16;
            u32 <<= 16;
        }
        if (!(u32 & 0xff000000)) {
            cnt += 8;
            u32 <<= 8;
        }
        if (!(u32 & 0xf0000000)) {
            cnt += 4;
        }
        u32 >>= 28;
        cnt += _bitclz32tab16[u32];
    }

    return cnt;
}

static INLINE long
m_ctz32(uint32_t u32)
{
    uint32_t val = u32 & (-u32);

    val *= V0_BIT_MAGIC_CTZ32;
    val >>= 27;
    val = _bitctz32tab32[val];

    return val;
}

static INLINE long
m_bytepar(uint8_t b)
{
    uint32_t    ret = 0x6996;
    uint32_t    val1;
    uint32_t    val2;

    val2 = b;
    val1 = b;
    val2 >>= 4;
    val1 ^= val2;
    val1 &= 0x0f;
    ret >>= val1;
    ret &= 0x01;

    return ret;
}

