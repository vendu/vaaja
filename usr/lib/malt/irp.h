#ifndef __MATH_IRP_H__
#define __MATH_IRP_H__

/* NOTE: some of this code is unnecessary and going to be removed later */

#include <stdint.h>
#include <zero/cdefs.h>
#include <malt/fxpc32.h>
#if defined(TEST_IRP)
#include <stdio.h>
#endif

#define MALT_IRP_GUESS_CONST    (48.0 / 17.0)
#define MALT_IRP_GUESS_FACTOR   (32 / 17.0)

int32_t irpdiv(int32_t num, int32_t den);

/* Newton-Raphson iterator */
/* x + x * (1 - x * xd) */
static C_INLINE double
nrpiter(double x, double d)
{
    double      xd = x * d;
    double      val = 1.0;

    val -= xd;                  // 1.0 - x * d
    val *= x;                   // x * (1.0 - x * d)
    x += val;                   // x + x * (1.0 - x * d)
#if defined(TEST_IRP)
    printf("IRP: %A\n", x);
#endif

    return x;
}

static C_INLINE double
nrpiter2(double x, double d)
{
    double  e = 1 - d * x;
    double  y = x * e;

    x += y + y * e;

    return x;
}

#endif /* __MATH_IRP_H__ */
