#ifndef __MATH_IRP_H__
#define __MATH_IRP_H__

/* NOTE: some of this code is unnecessary and going to be removed later */

#include <stdint.h>
#include <sys/cdefs.h>
#if defined(TEST_DIV) || defined(TEST_IRP)
#include <stdio.h>
#endif

#define MALT_IRP_GUESS_CONST    (48.0 / 17.0)
#define MALT_IRP_GUESS_FACTOR   (32 / 17.0)

int32_t irpidiv(int32_t num, int32_t den);

/* Newton-Raphson iterator */
/* x + x * (1 - x * xd) */
static __inline__ double
nrpiter(double x, double d)
{
    double      xd = x * d;
    double      val = 1.0;

    val -= xd;                  // 1.0 - x * d
    val *= x;                   // x * (1.0 - x * d)
    x += val;                   // x + x * (1.0 - x * d)

    return x;
}

/* Another Newton-Raphson iterator variant */
/*
 * e = 1 - dx;
 * y = x * e;
 * x = x + y + y * e;
 */
static __inline__ double
nrpiter2(double x, double d)
{
    double  e = 1.0 - d * x;
    double  y = x * e;

    x += y + y * e;
#if (TEST_DIV) || (TEST_IRP)
    fprintf(stderr, "X == %e\n", x);
#endif

    return x;
}

#endif /* __MATH_IRP_H__ */

