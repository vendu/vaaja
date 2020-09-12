#ifndef __SYS_VOIMA_NUM_FXP_UTIL_H__
#define __SYS_VOIMA_NUM_FXP_UTIL_H__

#include <stdio.h>
#include <zero/cdefs.h>

/* print fixed-point value in decimal or hex */
static C_INLINE void
printfxpbig(FXP_TYPE fxp, int hex)
{
    FXP_TYPE            fxsig = fxp & FXP_SIGN_BIT;
    FXP_TYPE            fx = fxp & ~fxsig;
    FXP_TYPE            fxint;
    FXP_TYPE            fxfrac;

    fxint = fx >> FXP_FRAC_BITS;
    fxfrac = fx & FXP_FRAC_MASK;
    if (hex) {
        fprintf(stderr, "(%c)%x.%x",
                (fxsig) ? '-' : '+',
                fxint, fxfrac);
    } else {
        fprintf(stderr, "(%c)%u.%u",
                (fxsig) ? '-' : '+',
                fxint, fxfrac);
    }
}

#endif /* __SYS_VOIMA_NUM_FXP_UTIL_H__ */

