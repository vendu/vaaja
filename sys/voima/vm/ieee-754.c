#include <stdlib.h>
#include <stdio.h>
#include <voima/vm/fp.h>

#if defined(VOIMA_IEEE_754_FP)

#define powerof2(x) (!((x) & ((x) - 1)))

/* TESTED OK */
float
fltnormalizei32(int32_t i32)
{
    float               flt;
    int32_t             fsig = i32 & VOIMA_IEEE_754_FLT_SIGN_BIT;
    int32_t             fval = i32; // = i32 & VOIMA_IEEE_754_FLT_MANT_MASK;
    int                 e;
    int32_t             fexp;

    if (fsig) {
        fval = ~fval + 1;
        fval &= VOIMA_IEEE_754_FLT_MANT_MASK;
    }
    e = VOIMA_IEEE_754_FLT_BITS - __builtin_clz(fval);
    fprintf(stderr, "E = %d\n", e);
    if (powerof2(fval)) {
        fval = 0;
    } else {
        fval <<= VOIMA_IEEE_754_FLT_MANT_BITS - e + 1;
    }
    fval &= ~(INT32_C(1) << VOIMA_IEEE_754_FLT_MANT_BITS);
    fexp = VOIMA_IEEE_754_FLT_EXP_BIAS + e - 1;
    fval |= fsig;
    fexp <<= VOIMA_IEEE_754_FLT_MANT_BITS;
    fval |= fexp;
    flt = *(float *)&fval;

    return flt;
}

void
fltprint(float flt, int hex)
{
    uint32_t            u32;
    uint32_t            fsig;
    uint32_t            fval;
    uint32_t            fexp;

    u32 = *(uint32_t *)&flt;
    fsig = fgetsign(u32) >> 31;
    fval = fgetmant(u32);
    fexp = (fgetexp(u32) >> 23) & 0xff;
    if (hex) {
        fprintf(stderr, "%e\t%c0x1.%xe%x\n",
                flt, (fsig) ? '-' : '+', fval,
                fexp - VOIMA_IEEE_754_FLT_EXP_BIAS);
    } else {
        fprintf(stderr, "%e\t%c1.%de%d\n",
                flt, (fsig) ? '-' : '+', fval,
                fexp - VOIMA_IEEE_754_FLT_EXP_BIAS);
    }

    exit(0);
}

#endif /* defined(VOIMA_IEEE_754_FP) */

