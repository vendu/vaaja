#ifndef __SYS_V0_SHIFT_H__
#define __SYS_V0_SHIFT_H__

#include <v0/v0.h>

static __inline__ long
v0shl(long cnt, long word)
{
    word <<= cnt;

    return word;
}

static __inline__ long
v0shr(long cnt, long word)
{
    long        mask = 0xffffffff;

    word >>= cnt;
    mask >>= cnt;
    word &= mask;

    return word;
}

static __inline__ long
v0sar(long cnt, long word)
{
    long        sign = word & 0x80000000;
    long        mask = 0xffffffff;

    word >>= cnt;
    if (sign) {
        mask <<= 32 - cnt;
        word |= mask;
    } else {
        mask >> cnt;
        word &= mask;
    }

    return word;
}

static __inline__ long
v0rol(long cnt, long word)
{
    long        mask = 0xffffffff;

    mask >>= 32 - cnt;
    word <<= cnt;
    word |= mask;

    return word;
}

static __inline__ long
v0ror(long cnt, long word)
{
    long        mask = 0xffffffff;

    mask <<= 32 - cnt;
    word >>= cnt;
    word |= mask;

    return word;
}

#endif /* __SYS_V0_SHIFT_H__ */

