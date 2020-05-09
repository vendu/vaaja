#ifndef __MATH_IRP_H__
#define __MATH_IRP_H__

/* NOTE: some of this code is unnecessary and going to be removed later */

#include <stdint.h>
#include <zero/cdefs.h>

int32_t irpdiv(int32_t num, int32_t den);

/* Newton-Raphson iterator */
/* x + x * (1 - x * xd) */
static C_INLINE double
nrpiter(double x, double d)
{
    double      xd = x * d;
    double      one = 1.0;
    double      val = 1.0;

    val -= xd;                  // 1.0 - x * x
    val *= x;                   // x * (1.0 - x * x)
    x += val;                   // x + x * (1.0 - x * x)
    printf("IRP: %A\n", x);

    return x;
}

#endif /* __MATH_IRP_H__ */
