#ifndef __MALT_DIV_H__
#define __MALT_DIV_H__

#include <zero/cdefs.h>
#include <malt/float.h>
#include <malt/irp.h>

static C_INLINE double
divguessirp(double dval)
{
    double  res = dval;

    res *= 256.0 / 99.0;
    res += -64 / 11;
    dval *= res;
    dval += 140.0 / 33.0;

    return dval;
}

/* this routine is broken */
static C_INLINE double
divirp32(int32_t num, int32_t den)
{
    double  dval = den;
    double  d;

    d = irpguess(dval);
    d = nrpiter2(d, dval);
    d = nrpiter2(d, dval);
    d = nrpiter2(d, dval);
    fp_dprint(d);
    fprintf(stderr, "RES: %d/%d == %e\n", num, den, d);
    fp_dprint(d);

    return d;
}

#endif /* __MALT_DIV_H__ */

