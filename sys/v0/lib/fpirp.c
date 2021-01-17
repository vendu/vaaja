#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <env/cdefs.h>
#include <v0/lib/fpirp.h>
#include <v0/lib/float.h>

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

    return x;
}

/* Another Newton-Raphson iterator variant */
/*
 * e = 1 - dx;
 * y = x * e;
 * x = x + y + y * e;
 */
static C_INLINE double
nrpiter2(double x, double d)
{
    double  e = 1.0 - d * x;
    double  y = x * e;

    x += y + y * e;

    return x;
}

C_INLINE C_CONST double
fpirp64(double x)
{
    double              firp64;
    uint64_t            uirp64 = IRP_MAGIC_64;
    union irp64         irp64;

    irp64.dbl = x;          // store floating-point value in the union
    uirp64 -= irp64.u64;    // compute uirp64 - irp64;
    uirp64 >>= 1;           // shift to acquire (uirp64 - irp64) >> 1;
    irp64.u64 = uirp64;     // irp64 = pow(x, -0.5);
    firp64 = irp64.dbl;     // read current result back as a double
    firp64 *= firp64;       // pow(pow(x, -0.5), 2) = pow(x, -1) = 1.0/X;

    return firp64;
}

C_INLINE C_CONST float
fpirp32(float x)
{
    float               firp32;
    uint32_t            uirp32 = IRP_MAGIC_32;
    union irp32         irp32;

    irp32.flt = x;
    uirp32 -= irp32.u32;
    uirp32 >>= 1;
    irp32.u32 = uirp32;
    firp32 = irp32.flt;
    firp32 *= firp32;

    return firp32;
}

float
firp32fast(float x)
{
    union {
        float           flt;
        int32_t         i32;
    } v;
    float               w;
    float               sx;
    int32_t             m;

    sx = (x < 0) ? -1 : 1;
    x = sx * x;
    v.i32 = FIRP32_FAST_MAGIC - *(uint32_t *)&x;
    w = x * v.flt;
    /* iterative approximation improvement in horner polynomial form */
    v.flt = v.flt * (2 - w);
    sx *= v.flt;

    return sx;
}

int
main(int argc, char *argv)
{
    double              x1 = 528803.0;
    double              x2 = 2097151;
    double              fpi;

    fpi = fpirp64(x2);
    fpi = nrpiter2(fpi, x2);
    fpi = nrpiter2(fpi, x2);
    fpi = nrpiter2(fpi, x2);
    fprintf(stderr, "%le / %le = %le (%le)\n", x1, x2, x1 / x2, fpi * x1);
    fpi = firp32fast(x1);
    fprintf(stderr, "%le / %le = %le (%le)\n", x2, x1, x2 / x1, fpi * x2);

    exit(0);
}

