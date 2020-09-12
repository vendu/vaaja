#ifndef __SYS_VOIMA_NUM_FXP_OP_H__
#define __SYS_VOIMA_NUM_FXP_OP_H__

#include <signal.h>

/* TESTED OK */
/* multiply two fixed-point values */
static C_INLINE FXP_TYPE
fxpmul(FXP_TYPE a, FXP_TYPE b)
{
    FXP_TYPE            afrac = a & FXP_FRAC_MASK;
    FXP_TYPE            asig = a & FXP_SIGN_BIT;
    FXP_TYPE            bsig = a & FXP_SIGN_BIT;

    if (bsig) {
        b = ~b + 1;
    }
    if (asig) {
        a = ~a + 1;
    }
    b >>= FXP_FRAC_BITS;
    a *= b;
    asig ^= bsig;
    afrac *= b;
    a |= asig;
    a |= afrac;
    fprintf(stderr, "MUL: %llx\n", a);

    return a;
}

/* TESTED OK */
/* perform [truncating] integer-part division of a / b */
static C_INLINE FXP_TYPE
fxpidiv(FXP_TYPE a, FXP_TYPE b)
{
    if (!b) {

        raise(SIGFPE);
    }
    a /= b;
    a <<= FXP_FRAC_BITS;

    return a;
}

/* TESTED OK */
/* perform division of a / b */
static C_INLINE FXP_TYPE
fxpdiv(FXP_TYPE a, FXP_TYPE b)
{
    FXP_TYPE            rem = a;
    FXP_TYPE            den = b;
    FXP_TYPE            q = 0;
    FXP_TYPE            d;
    int                 i = FXP_INT_BITS + 1;

    if (!b) {

        raise(SIGFPE);
    }
    while (!((den) & 0xf) && (i >= 4)) {
        den >>= 4;
        i -= 4;
    }
    while ((rem) && (i >= 0)) {
        int             n = __builtin_clzll(rem);

        if (n > i) {
            n = i;
        }
        rem <<= n;
        i -= n;
        d = rem / den;
        rem %= den;
        q += d << i;
        rem <<= 1;
        i--;
        a = q >> 1;
    }

    return a;
}

#endif /* __SYS_VOIMA_NUM_FXP_OP_H__ */

