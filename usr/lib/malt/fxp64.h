#ifndef __MALT_FXP64_H__
#define __MALT_FXP64_H__

#include <stdint.h>
#include <stdio.h>

#define FXP64_BITS              64
#define FXP64_SIGN_BIT          (INT64_C(1) << (FXP64_BITS - 1))
#define FXP64_INT_BITS          (FXP64_BITS - 1 - FXP64_FRAC_BITS)
#define FXP64_FRAC_BITS         32
#define FXP64_FRAC_MASK         ((INT64_C(1) << 32) - 1)

typedef int64_t                 fxp64_t;
typedef uint64_t                ufxp64_t;

#define fxp64sign(a)            ((a) & FXP64_SIGN_BIT)
#define fxp64add(a, b)      	((a) + (b))
#define fxp64sub(a, b)      	((a) - (b))
#define fxp64imul(a, b)      	(((a) * (b)) >> FXP64_FRAC_BITS)

static C_INLINE fxp64_t
fxp64lomul(fxp64_t x, fxp64_t y)
{
    fxp64_t res = 0;
    fxp64_t fxi = x >> FXP64_FRAC_BITS;
    fxp64_t fyi = y >> FXP64_FRAC_BITS;
    fxp64_t fxf = x & FXP64_FRAC_MASK;
    fxp64_t fyf = x & FXP64_FRAC_MASK;

    res += (fxi * fyi) << FXP64_FRAC_BITS;
    res += fxi * fyf;
    res += fxf * fyf;
    res += ((fxf * fyf) >> FXP64_FRAC_BITS) & FXP64_FRAC_BITS;

    return res;
}

static C_INLINE fxp64_t
fxp64mul(fxp64_t x, fxp64_t y)
{
    fxp64_t res = 0;
    fxp64_t fyi = y >> FXP64_FRAC_BITS;
    fxp64_t fxf = x & FXP64_FRAC_MASK;
    fxp64_t fyf = x & FXP64_FRAC_MASK;

    res = (x * fyi);
    res += x * fyf;

    return res;
}

static C_INLINE fxp64_t
fxp64idiv(int64_t x, int64_t y)
{
    int64_t sx = fxp64sign(x);
    int64_t sy = fxp64sign(y);
    int64_t ux = x & ~sx;
    int64_t uy = y & ~sy;

    sx ^= sy;
    ux /= uy;
    sx >>= FXP64_BITS - FXP64_INT_BITS;
    ux |= sx;
    
    return ux;
}

static C_INLINE fxp64_t
fxp64div(fxp64_t x, fxp64_t y)
{
    fxp64_t res = 0;
    fxp64_t p2 = INT64_C(1) << (FXP64_BITS - 2);
    fxp64_t bit;
    int     cnt;

    fprintf(stderr, "X / Y => %llx / %llx\n", (long long)x, (long long)y);
    for (cnt = 1 ; cnt < FXP64_BITS - 1 ; cnt++) {
        bit = (y & p2) >> cnt;
        if (x > 0 && (bit)) {
            x ^= p2;
            res |= p2;
        } else if (bit) {
            x ^= p2;
            res |= p2;
        }
        fprintf(stderr, "%d: bit == %llx, p2 == %llx, x == %llx, res == %llx\n",
                cnt, (long long)bit, (long long)p2,
                (long long)x, (long long)res);
        p2 >>= 1;
    }

    return res;
}

#if 0
static C_INLINE fxp64_t
fxp64div(int64_t x, int64_t y)
{
    int64_t     sign = FXP64_SIGN_BIT;
    int64_t     quot = 0;
    int64_t     rem = x & ~sign;
    int64_t     den = y & ~sign;
    int64_t     tmp;
    int         i;

    i = 0;
    for (i = 0 ; i < FXP64_BITS - 1 ; i++) {
        fprintf(stderr, "R = %llx, D = %llx, Q == %llx\n", rem, den, quot);
        tmp = rem - den;
        quot <<= 1;
        if (tmp > 0) {
            rem = tmp;
            quot |= 1;
        }
    }

    return quot;
}
#endif

#endif /* __MALT_FXP64_H__ */

