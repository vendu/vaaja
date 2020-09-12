#ifndef __SYS_VOIMA_NUM_DOUBLE2FXP_H__
#define __SYS_VOIMA_NUM_DOUBLE2FXP_H__

#include <zero/cdefs.h>
#include <voima/num/npx/fp.h>
#include <voima/num/fxp/fxp.h>

#if 0
static C_INLINE FXP_TYPE
double2fxpbig(double x)
{
    FXP_TYPE            fxp = *(FXP_TYPE *)&x;
    FXP_TYPE            dsig = fxp & VOIMA_IEEE_754_DBL_SIGN_BIT;
    FXP_TYPE            dmant = dgetmant(fxp);
    FXP_TYPE            dexp = dgetexp(fxp & ~dsig)
                               >> VOIMA_IEEE_754_DBL_FRAC_BITS;
    int                 cnt = __builtin_clzll(dmant);

    fxp = dmant;
    fprintf(stderr, "EXP: %lld, CNT: %lld\n", dexp, cnt);
    if (dexp > VOIMA_IEEE_754_DBL_EXP_BIAS) {
        dexp -= VOIMA_IEEE_754_DBL_EXP_BIAS;
        fprintf(stderr, "DEXP: %lld\n", dexp);
        fxp <<= dexp + VOIMA_IEEE_754_DBL_EXP_BIAS - dexp + VOIMA_IEEE_754_DBL_MANT_BITS - FXP_FRAC_BITS + cnt;
    } else if (dexp < VOIMA_IEEE_754_DBL_EXP_BIAS) {
        dexp = VOIMA_IEEE_754_DBL_EXP_BIAS - dexp;
        fprintf(stderr, "DEXP: %lld\n", dexp);
        fxp >>= dexp + VOIMA_IEEE_754_DBL_EXP_BIAS - dexp + VOIMA_IEEE_754_DBL_MANT_BITS - FXP_FRAC_BITS - cnt;
    }

    return fxp;
}
#endif

#if 0
/* FIXME: some conversion precision loss in the fraction */
/* convert double values to our fixed-point format */
static C_INLINE FXP_TYPE
double2fxpbig(double x)
{
    FXP_TYPE            fxp;
    double              d = fabs(x);
    double              dstep;
    double              dlim;
    FXP_TYPE            lim;
    UFXP_TYPE           bit;
    int                 ndx;
    int                 dsig = signbit(x);

    /* initialize fxp to integer part */
    if (!dsig) {
        dlim = floor(x);
        lim = (FXP_TYPE)dlim;
        d -= dlim;
        fxp = lim << FXP_FRAC_BITS;
    } else {
        dlim = ceil(x);
        lim = (FXP_TYPE)dlim;
        d = dlim - d;
        fxp = lim << FXP_FRAC_BITS;
    }
    fprintf(stderr, "D: %e (DLIM: %e)\n", d, dlim);
    dstep = 0.5;
    bit = UINT64_C(1) << (FXP_FRAC_BITS - 1);
    ndx = FXP_FRAC_BITS;
    while (ndx--) {
        if (d >= dstep) {
            d -= dstep;
            fxp |= bit;
        }
        //        dstep *= 0.5;
        dstep /= 2.0;
        bit >>= 1;
    }
    return fxp;
}
#endif

static C_INLINE FXP_TYPE
double2fxpbig(double x)
{
    FXP_TYPE            fxp = *(FXP_TYPE *)&x;
    double              d = fabs(x);
    double              dlim;
    FXP_TYPE            lim;
    UFXP_TYPE           dbit;
    UFXP_TYPE           bit;
    int                 ndx;
    int                 dsig = signbit(x);
    FXP_TYPE            dfrac = dgetfrac(fxp);

    /* initialize fxp to integer part */
    if (!dsig) {
        dlim = floor(x);
        lim = (FXP_TYPE)dlim;
        if (dfrac) {
            dfrac -= lim;
        }
        fxp = lim << FXP_FRAC_BITS;
    } else {
        dlim = ceil(x);
        lim = (FXP_TYPE)dlim;
        lim = ~lim + 1;
        if (dfrac) {
            dfrac = lim - dfrac;
        }
        fxp = lim << FXP_FRAC_BITS;
    }
    fprintf(stderr, "LIM: %lld (DFRAC: %lld)\n", lim, dfrac);
    if (dfrac) {
        dbit = UINT64_C(1) << (VOIMA_IEEE_754_DBL_FRAC_BITS - 1);
        ndx = FXP_FRAC_BITS;
        bit = UINT64_C(1) << (FXP_FRAC_BITS - 1);
        while (ndx--) {
            if (dfrac & dbit) {
                dfrac &= ~dbit;
                fxp |= bit;
            }
            dbit >>= 1;
            bit >>= 1;
        }
    }
    if (dsig) {
        fxp |= FXP_SIGN_BIT;
    }
    return fxp;
}

#endif /* __SYS_VOIMA_NUM_DOUBLE2FXP_H__ */

