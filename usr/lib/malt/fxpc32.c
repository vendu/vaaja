#include <stdint.h>
#include <math.h>
#include <zero/cdefs.h>
#include <malt/fxpc32.h>

#if defined(TEST_FXPC32)
#include <stdlib.h>
#include <stdio.h>
#endif

#if 0
#define FXPC32_FRAC_BITS    32
#define FXPC32_INT_BITS     31
#define FXPC32_SIGN_BIT     (UINT64_C(1) << (FXPC32_BITS - 1))
#define FXPC32_BITS         64
#endif

#define DBL_E               2.7182818284590452354   /* e */
#define DBL_LOG2E           1.4426950408889634074   /* log_2 e */
#define DBL_LOG10E          0.43429448190325182765  /* log_10 e */
#define DBL_LN2             0.69314718055994530942  /* log_e 2 */
#define DBL_LN10            2.30258509299404568402  /* log_e 10 */
#define DBL_PI              3.14159265358979323846  /* pi */
#define DBL_PI_2            1.57079632679489661923  /* pi/2 */
#define DBL_PI_4            0.78539816339744830962  /* pi/4 */
#define DBL_1_PI            0.31830988618379067154  /* 1/pi */
#define DBL_2_PI            0.63661977236758134308  /* 2/pi */
#define DBL_2_SQRTPI        1.12837916709551257390  /* 2/sqrt(pi) */
#define DBL_SQRT2           1.41421356237309504880  /* sqrt(2) */
#define DBL_SQRT1_2         0.70710678118654752440  /* 1/sqrt(2) */

struct g_fxpc32 {
    fxpc32_t   one;
    fxpc32_t   halfpi;
    fxpc32_t   reg1;
    fxpc32_t   reg2;
    fxpc32_t   reg3;
};

struct g_fxpc32         g_fxpc32;

#if 0
fxpc32_t
float2fxpc32(float x)
{
    fxpc32_t    fxp = 0; // fixed-point value
    fxpc32_t    lim;
    ufxpc32_t   bit;
    int         i;
    float       f;

    /* initialize fxp to integer part */
    if (x > 0.0) {
        lim = (fxpc32_t)floor(x);
        fxp = lim << FXPC32_FRAC_BITS;
        fprintf(stderr, "FLOOR: %x (%x)\n", fxp, lim);
    } else if (x < 0.0) {
        lim = (fxpc32_t)ceil(x);
        fxp = lim << FXPC32_FRAC_BITS;
    } else {

        return 0;
    }
    x -= lim;
    f = 0.5;
    bit = UINT32_C(1) << (FXPC32_FRAC_BITS - 1);
    for (i = 0 ; i < FXPC32_FRAC_BITS ; i++) {
        if (x - f > 0) {
            fprintf(stderr, "i == %d (%lx)\n", i, bit);
            x -= f;
            fxp |= bit;
        }
        bit >>= 1;
        f *= 0.5;
    }

    return fxp;
}
#endif

fxpc32_t
double2fxpc32(double x)
{
    fxpc32_t    fxp = 0; // fixed-point value
    fxpc32_t    lim;
    ufxpc32_t   bit;
    int         i;
    double      d;

    /* initialize fxp to integer part */
    if (x > 0.0) {
        lim = (fxpc32_t)floor(x);
        fxp = lim << FXPC32_FRAC_BITS;
        fprintf(stderr, "FLOOR: %x (%x)\n", fxp, lim);
    } else if (x < 0.0) {
        lim = (fxpc32_t)ceil(x);
        fxp = lim << FXPC32_FRAC_BITS;
    } else {

        return 0;
    }
    x -= lim;
    d = 0.5;
    bit = UINT32_C(1) << (FXPC32_FRAC_BITS - 1);
    for (i = 0 ; i < FXPC32_FRAC_BITS ; i++) {
        if (x - d > 0) {
            fprintf(stderr, "i == %d (%lx)\n", i, bit);
            x -= d;
            fxp |= bit;
        }
        bit >>= 1;
        d *= 0.5;
    }

    return fxp;
}

void
fxpc32init(struct g_fxpc32 *cp)
{
    if (!cp) {
        cp = &g_fxpc32;
    }
    cp->one = double2fxpc32(1.0);
    cp->halfpi = double2fxpc32(DBL_PI_2);

    return;
}

/*
 * REFERENCE: https://www.drdobbs.com/microcontrollers-cordic-methods/184404244 
 */

fxpc32_t
fxpc32mul(int64_t x, int64_t y)
{
    int64_t     delta = y >> FXPC32_FRAC_BITS;
    int64_t     res = 0;
    ufxpc32_t   p2 = UINT32_C << (FXPC32_BITS - 2);
    int         i;
    
    for (i = 0 ; i < FXPC32_BITS - 1 ; i++) {
        if (x > 0) {
            x -= p2;
            res += y * p2;
            p2 >>= 1;
        } else {
            x += p2;
            res -= y * p2;
            p2 >>= 1;
        }
    }
    delta++;
    res >>= FXPC32_FRAC_BITS;
    if (x > 0) {
        res += delta;
    }

    return res;
}

fxpc32_t
fxpc32div(fxpc32_t x, fxpc32_t y)
{
    fxpc32_t    res = 0;
    fxpc32_t    p2 = UINT32_C(1) << (FXPC32_BITS - 2);
    int         i;
    
    for (i = 0; i <= FXPC32_BITS - 1 ; i++){
        if (fx > 0) {
            x -= y * p2;
            res += p2;
            p2 >>= 1;
        } else {
            x += y * p2;
            res -= p2;
            p2 >>= 1;
        }
    }
        
    return res;
}

#if 0
fxpc32_t
fxpc32div4q(uint32_t x, uint32_t y)
{
    fxpc32_t    fx = fxpc32normint(x);
    fxpc32_t    fy = fxpc32normint(y);
    fxpc32_t    p2 = 0x00000001 << (FXPC32_FRAC_BITS - 1);
    fxpc32_t    res = 0;
    int         i;

    for (i = 1 ; i <= FXPC32_FRAC_BITS ; i++) {
        if (fx > 0 && fy > 0) {
            fx -= fy * p2;
            res += p2;
        } else if (fx > 0) {
            fx += fy * p2;
            res -= p2;
        } else if (fy > 0) {
            fx += fy * p2;
            res -= p2;
        } else {
            fx -= fy * p2;
            res += p2;
        }
        p2 >>= 1;
    }

    return res;
}
#endif

#if 0

Listing Four
log10(x){
   z = 0;
   for ( i=1;i=<B;i++ ){
      if (x > 1)
         x = x - x*2^(-i);
         z = z - log10(1-2^(-i));
       else
         x = x + x*2^(-i);
         z = z - log10(1+2^(-i));
   }
   return(z)
}

Listing Five
10_to_power(x){
   z = 1;
   for ( i=1;i=<B; i++ ){
      if (x > 0)
         x = x - log10(1+2^(-i));
         z = z + z*2^(-i);
      else
         x = x - log10(1-2^(-i));
         z = z - z*2^(-i);
   }
   return(z)
}

Listing Six
sin(z){  
   x = 1.6468;
   y = 0;       
   for (i=0; i=<R; i++){
      if (z > 0)
         x = x - y*2(^-i)
         y = y + x*2(^-i)
         z = z - arctan(2^(-i))
      else  
         x = x + y*2(^-i)
         y = y - x*2^(-i)
         z = z + arctan(2^(-i))
   }
   return(y)
}
cos(z){  
   x = 1.6468;
   y = 0;
   for (i=0; i=<R; i++){
      if (z > 0)
         x = x - y*2(^-i)
         y = y + x*2(^-i)
         z = z - arctan(2^(-i))
      else
         x = x + y*2(^-i)
         y = y - x*2^(-i)
         z = z + arctan(2^(-i))
   }
   return(x)
}

#endif

#if defined(TEST_FXPC32)

int
main(int argc, char *argv[])
{
    fxpc32_t    fxp;
    
    fxpc32init(NULL);
    printf("ONE\t%lx\n", g_fxpc32.one);
    printf("PI/2\t%lx\n", g_fxpc32.halfpi);
    printf("ONE * PI/2\t%lx\n", fxpc32mul(g_fxpc32.one, g_fxpc32.halfpi));
    printf("2 * PI/2\t%lx\n", fxpc32mul(2 << FXPC32_FRAC_BITS,
                                        g_fxpc32.halfpi));
    fxp = double2fxpc32(DBL_PI);
    printf("PI\t%lx\n", fxp);
    printf("2 * PI == %lx (%lx)\n", fxpc32mul(2 << FXPC32_FRAC_BITS, fxp),
           fxp << 1);
    printf("PI / 2 == %lx (%lx)\n", fxpc32div(fpx, 2 << FXPC32_FRAC_BITS),
           fxp >> 1);

    exit(0);
}

#endif /* TEST_FXPC32 */

