/* REFERENCE: https://stackoverflow.com/questions/9939322/fast-1-x-division-reciprocal  */

/*
 * Utilising Newton-Raphson method to do 32-bit integer division with 64-bit
 * double-precision floating-point multiplication using Black Magic... =)
 */

#include <stdint.h>
//#include <malt/util.h>
#include <malt/float.h>
#include <malt/irp.h>
#if defined(TEST_IRP)
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#if 0
float
irpguessf(float f)
{
    union _flt32        u;
    uint32_t            m = UINT32_C(0xbe6eb3be);
    float               flt;

    u.flt = f;
    u.u32 = (m - u.u32) >> 1;
    u.flt *= u.flt;
    flt = u.flt;

    return flt;
}
#endif

union _dbl64 {
    uint64_t    u64;
    double      dbl;
};

double
irpguess0(double den)
{
    union _dbl64        u;
    uint64_t            m = UINT64_C(0xbfcdd6a18f6a6f52);
    double              dbl;

    u.dbl = den;
    u.u64 = (m - u.u64) >> 1;
    u.dbl *= u.dbl;
    dbl = u.dbl;

    return dbl;
}

double
irpguess(double den)
{
    double  d1 = 140.0 / 33.0;
    double  d2 = -64.0 / 11.0;
    double  d3 = 256.0 / 99.0;
    double  res = den;

    res *= d3;          // den * 256/99
    res += d2;          // den += -64/11
    res *= den;         // den * (-64/11 + d * 256/99)
    res += d1;

    return res;
}

double
gsdiv(int32_t num, int32_t den)
{
    int     sft = DOUBLE_MANTISSA_BITS - __builtin_clz(den);
    double  n;
    double  d = den << sft;
    double  f;

    if (den == 0) {
        abort();
    } else if (den > num) {

        return 0;
    } else if (den == num) {

        return 1;
    }
    fp_dsetexp(n, sft);
    fp_dsetexp(d, -sft);
    f = 2.0 - d;
    d = 2.0 - f;
    f = 2.0 - d;
    d = 2.0 - f;
    f = 2.0 - d;
    num *= d;

    return num;
}

int32_t
irpidiv(int32_t num, int32_t den)
{
    double  dnum = num;
    double  dval = den;
    double  drp;
    int32_t res;

    if (den == 0) {
        abort();
    } else if (den > num) {

        return 0;
    } else if (den == num) {

        return 1;
    } else if (den != 1) {
        drp = irpguess2(dval);
        drp = nrpiter2(drp, dval);
        drp = nrpiter2(drp, dval);
        drp = nrpiter2(drp, dval);
        drp = nrpiter2(drp, dval);
        dnum *= drp;
    }
    res = (int32_t)dnum;

    return res;
}

#if defined(TEST_IRP)
int
main(int argc, char *argv[])
{
    int32_t res;
    int     i;
    int     j;

    fprintf(stderr, "%e steps\n", log2(33.0 / log2(17)));
    fprintf(stderr, "%d\n", irpidiv(55, 55));
    fprintf(stderr, "%d\n", irpidiv(220, 55));
    fprintf(stderr, "%d\n", irpidiv(5, 5));
    for (i = 0 ; i <= 0xffff ; i++) {
        fprintf(stderr, "I: %x\n", i);
        for (j = 1 ; j <= 0xffff ; j++) {
            res = irpidiv(i, j);
            if (res != i / j) {
                fprintf(stderr, "%d/%d != %d", i, j, res);
                abort();
            }
        }
    }

    exit(0);
}
#endif

