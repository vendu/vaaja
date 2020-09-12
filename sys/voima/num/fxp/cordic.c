#include <fxp/fxpcordic.h>

voimafxpcordic_t
fxp2cordic(voimafxp_t fxp)
{
    fxp >>= VOIMA_FXP_INT_BITS;
    if (fxp & VOIMA_FXP_INT_MASK) {

        return VOIMA_FXPCORDIC_SIGN_BIT;
    }
}

