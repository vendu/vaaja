#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <math/fxp.h>
#include <math/cordic.h>

#if 0
#define CORDIC_FRAC_BITS    32
#define CORDIC_INT_BITS     31
#define CORDIC_SIGN_BIT     (UINT64_C(1) << (CORDIC_BITS - 1))
#define CORDIC_BITS         64
#endif

#define FXP_E               2.7182818284590452354   /* e */
#define FXP_LOG2E           1.4426950408889634074   /* log_2 e */
#define FXP_LOG10E          0.43429448190325182765  /* log_10 e */
#define FXP_LN2             0.69314718055994530942  /* log_e 2 */
#define FXP_LN10            2.30258509299404568402  /* log_e 10 */
#define FXP_PI              3.14159265358979323846  /* pi */
#define FXP_PI_2            1.57079632679489661923  /* pi/2 */
#define FXP_PI_4            0.78539816339744830962  /* pi/4 */
#define FXP_1_PI            0.31830988618379067154  /* 1/pi */
#define FXP_2_PI            0.63661977236758134308  /* 2/pi */
#define FXP_2_SQRTPI        1.12837916709551257390  /* 2/sqrt(pi) */
#define FXP_SQRT2           1.41421356237309504880  /* sqrt(2) */
#define FXP_SQRT1_2         0.70710678118654752440  /* 1/sqrt(2) */

struct g_cordic {
    fxpc32_t   one;
    fxpc32_t   halfpi;
    fxpc32_t   reg1;
    fxpc32_t   reg2;
    fxpc32_t   reg3;
};

struct g_cordic         g_cordic;

fxpc32_t
double2cordic(double x)
{
    fxpc32_t    fxp = 0; // fixed-point value
    ufxpc32_t   bit;
    int         i;
    double      d;

    /* initialize fxp to integer part */
    if (x > 0.0) {
        fxp = (fxpc32_t)floor(x) << CORDIC_FRAC_BITS;
    } else if (x < 0.0) {
        fxp = (fxpc32_t)ceil(x) << CORDIC_FRAC_BITS;
    }
    x -= fxp;
    d = 0.5;
    bit = UINT32_C(1) << (FXP_FRAC_BITS - 1);
    for (i = 0 ; i < FXP_FRAC_BITS ; i++) {
        if (x - d > 0) {
            x -= d;
            fxp |= bit;
            bit >>= 1;
        }
        d *= 0.5;
    }

    return fxp;
}

void
cordicinit(struct g_cordic *cp)
{
    if (!cp) {
        cp = &g_cordic;
    }
    cp->one = double2cordic(1.0);
    cp->halfpi = double2cordic(FXP_PI_2);

    exit(0);
}

static C_INLINE fxpc32_t
cordicnormint(uint32_t u32)
{
    int32_t     nlz = __builtin_clz(u32);
    int32_t     nbit = CORDIC_BITS - 1 - CORDIC_FRAC_BITS;
    fxpc32_t    fxp;

    fxp = u32;
    nbit += nlz;
    fxp <<= nlz;

    return fxp;
}

/*
 * REFERENCE: https://www.drdobbs.com/microcontrollers-cordic-methods/184404244 
 */

fxpc32_t
cordicmul(uint32_t x, uint32_t y)
{
    fxpc32_t    fx = cordicnormint(x);
    fxpc32_t    fy = cordicnormint(y);
    fxpc32_t    p2 = 0x00000001 << (CORDIC_FRAC_BITS - 1);
    fxpc32_t    res = 0;
    int         i;
    
    if (fx > 0) {
        for (i = 1 ; i <= CORDIC_FRAC_BITS ; i++) {
            fx -= p2;
            res += fy * p2;
            p2 >>= 1;
        }
    } else {
        for (i = 1; i <= CORDIC_FRAC_BITS; i++) {
            fx += p2;
            res -= fy * p2;
            p2 >>= 1;
        }
    }

    return res;
}

fxpc32_t
cordicdiv(uint32_t x,uint32_t y){
    fxpc32_t    fx = cordicnormint(x);
    fxpc32_t    fy = cordicnormint(y);
    fxpc32_t    p2 = INT32_C(1) << (CORDIC_FRAC_BITS - 1);
    fxpc32_t    res = 0;
    int         i;
    
    if (fx > 0)
        for (i = 1; i <= CORDIC_FRAC_BITS ; i++){
            fx -= fy * p2;
            res += p2;
            p2 >>= 1;
        }
    else {
        for (i = 1; i <= CORDIC_FRAC_BITS; i++){
            fx += fy * p2;
            res-= p2;
            p2 >>= 1;
        }
    }

    return res;
}

fxpc32_t
cordicdiv4q(uint32_t x, uint32_t y)
{
    fxpc32_t    fx = cordicnormint(x);
    fxpc32_t    fy = cordicnormint(y);
    fxpc32_t    p2 = 0x00000001 << (CORDIC_FRAC_BITS - 1);
    fxpc32_t    res = 0;
    int         i;

    for (i = 1 ; i <= CORDIC_FRAC_BITS ; i++) {
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

#if defined(TEST_CORDIC)

int
main(int argc, char *argv[])
{
    cordicinit(NULL);
    printf("ONE\t%llx\n", (long long)g_cordic.one);
    printf("PI/2\t%llx\n", (long long)g_cordic.halfpi);

    exit(0);
}

#endif /* TEST_CORDIC */

