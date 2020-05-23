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
irpguess(double den)
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

/* this division routine should work fine :) */
int32_t
irpidiv(int32_t num, int32_t den)
{
    double  dnum = num;
    double  dval = den;
    double  drp;
    int32_t res;

    drp = irpguess(dval);
    drp = nrpiter2(drp, dval);
    drp = nrpiter2(drp, dval);
    drp = nrpiter2(drp, dval);
    drp = nrpiter2(drp, dval);
    dnum *= drp;
    res = (int32_t)dnum;

    return res;
}

#if defined(TEST_IRP)
int
main(int argc, char *argv[])
{
    fprintf(stderr, "%e steps\n", log2(33.0 / log2(17)));
    fprintf(stderr, "%d\n", irpidiv(55, 55));
    fprintf(stderr, "%d\n", irpidiv(220, 55));

    exit(0);
}
#endif

