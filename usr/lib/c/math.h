#ifndef __MATH_H__
#define __MATH_H__

#include <features.h>
#include <float.h>
#include <bits/ieee754.h>
#include <bits/math.h>
#if defined(__x86_64__) || defined(__amd64__) || defined(_M_AMD64)
#include <x86-64/math>
#else
#include <ia32/math.h>
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ > 199901L)
#if (FLT_EVAL_METHOD == 0)
typedef float           float_t;
typedef double          double_t;
#define __NO_LONG_DOUBLE_MATH 1
#elif (FLT_EVAL_METHOD == 1)
typedef double          float_t;
typedef double          double_t;
#define __NO_LONG_DOUBLE_MATH 1
#elif (FLT_EVAL_METHOD == 2)
typedef long double     float_t;
typedef long double     double_t;
#undef  __NO_LONG_DOUBLE_MATH
#endif /* FLT_EVAL_METHOD */
/* floating-point number categories */
#define FP_NAN          0
#define FP_INFINITE     1
#define FP_ZERO         2
#define FP_SUBNORMAL    3
#define FP_NORMAL       4
/* the __-prefix macros need to be defined in per-architecture header files */
#define FP_FAST_FMA     __FP_FAST_FMA
#define FP_FAST_FMAF    __FP_FAST_FMAF
#define FP_FAST_FMAL    __FP_FAST_FMAL
#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ > 199901L) */

#if defined(_SVID_SOURCE)

/* type values */
#define DOMAIN          1
#define SING            2
#define OVERFLOW        3
#define UNDERFLOW       4
#define TLOSS           5
#define PLOSS           6
/* return this large value instead of infinity */
#define HUGE            (3.40282347e+38f)

struct exception {
    int                 type;
    char               *name;
    double              arg1;
    double              arg2;
    double              retval;
};

extern int              matherr(struct exception *exc);
#else /* !defined(_SVID_SOURCE) */

#if defined(_XOPEN_SOURCE)
#define MAXFLOAT        3.40282347e+38f
#endif /* defined(_XOPEN_SOURCE) */

#endif /* defined(_SVID_SOURCE) */

#if defined(_GNU_SOURCE) && defined(_IEEE_QUAD_128)
/* these constants are precise enough for 128-bit IEEE quad support */
#define M_El            2.718281828459045235360287471352662498L /* e */
#define M_LOG2El        1.442695040888963407359924681001892137L /* log_2 e */
#define M_LOG10El       0.434294481903251827651128918916605082L /* log_10 e */
#define M_LN2l          0.693147180559945309417232121458176568L /* log_e 2 */
#define M_LN10l         2.302585092994045684017991454684364208L /* log_e 10 */
#define M_PIl           3.141592653589793238462643383279502884L /* pi */
#define M_PI_2l         1.570796326794896619231321691639751442L /* pi/2 */
#define M_PI_4l         0.785398163397448309615660845819875721L /* pi/4 */
#define M_1_PIl         0.318309886183790671537767526745028724L /* 1/pi */
#define M_2_PIl         0.636619772367581343075535053490057448L /* 2/pi */
#define M_2_SQRTPIl     1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
#define M_SQRT2l        1.414213562373095048801688724209698079L /* sqrt(2) */
#define M_SQRT1_2l      0.707106781186547524400844362104849039L /* 1/sqrt(2) */
#elif defined(_GNU_SOURCE)
#define M_El            2.7182818284590452353602874713526625L /* e */
#define M_LOG2El        1.4426950408889634073599246810018922L /* log_2 e */
#define M_LOG10El       0.4342944819032518276511289189166051L /* log_10 e */
#define M_LN2l          0.6931471805599453094172321214581766L /* log_e 2 */
#define M_LN10l         2.3025850929940456840179914546843642L /* log_e 10 */
#define M_PIl           3.1415926535897932384626433832795029L /* pi */
#define M_PI_2l	        1.5707963267948966192313216916397514L /* pi/2 */
#define M_PI_4l	        0.7853981633974483096156608458198757L /* pi/4 */
#define M_1_PIl	        0.3183098861837906715377675267450287L /* 1/pi */
#define M_2_PIl	        0.6366197723675813430755350534900574L /* 2/pi */
#define M_2_SQRTPIl     1.1283791670955125738961589031215452L /* 2/sqrt(pi) */
#define M_SQRT2l        1.4142135623730950488016887242096981L /* sqrt(2) */
#define M_SQRT1_2l      0.7071067811865475244008443621048490L /* 1/sqrt(2) */
#endif /* defined(_GNU_SOURCE) && defined(_IEEE_QUAD_128) */

#if defined(_BSD_SOURCE)) || (defined(_XOPEN_SOURCE)

#define M_E             2.7182818284590452354  /* e */
#define M_LOG2E         1.4426950408889634074  /* log_2 e */
#define M_LOG10E        0.43429448190325182765 /* log_10 e */
#define M_LN2           0.69314718055994530942 /* log_e 2 */
#define M_LN10          2.30258509299404568402 /* log_e 10 */
#define M_PI            3.14159265358979323846 /* pi */
#define M_PI_2          1.57079632679489661923 /* pi/2 */
#define M_PI_4          0.78539816339744830962 /* pi/4 */
#define M_1_PI          0.31830988618379067154 /* 1/pi */
#define M_2_PI          0.63661977236758134308 /* 2/pi */
#define M_2_SQRTPI      1.12837916709551257390 /* 2/sqrt(pi) */
#define M_SQRT2	        1.41421356237309504880 /* sqrt(2) */
#define M_SQRT1_2       0.70710678118654752440 /* 1/sqrt(2) */

#endif /* defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE) */

extern int              signgam;

extern double           acos(double);
extern double           asin(double);
extern double           atan(double);
extern double           atan2(double, double);
extern double           ceil(double);
extern double           cos(double);
extern double           cosh(double);
extern double           exp(double);
extern double           fabs(double);
extern double           floor(double);
extern double           fmod(double, double);
extern double           frexp(double, int *);
extern double           ldexp(double, int);
extern double           log(double);
extern double           log10(double);
extern double           modf(double, double *);
extern double           pow(double, double);
extern double           sin(double);
extern double           sinh(double);
extern double           sqrt(double);
extern double           tan(double);
extern double           tanh(double);
extern double           erf(double);
extern double           erfc(double);
extern double           gamma(double);
extern double           hypot(double, double);
extern double           j0(double);
extern double           j1(double);
extern double           jn(int, double);
extern double           lgamma(double);
extern double           y0(double);
extern double           y1(double);
extern double           yn(int, double);
//extern int         isnan(double);
extern double           acosh(double);
extern double           asinh(double);
extern double           atanh(double);
extern double           cbrt(double);
extern double           expm1(double);
extern int              ilogb(double);
extern double           log1p(double);
extern double           logb(double);
extern double           nextafter(double, double);
extern double           remainder(double, double);
extern double           rint(double);
extern double           scalb(double, double);

extern float            acosf(float);
extern float            asinf(float);
extern float            atanf(float);
extern float            atan2f(float, float);
extern float            ceilf(float);
extern float            cosf(float);
extern float            coshf(float);
extern float            expf(float);
extern float            fabsf(float);
extern float            floorf(float);
extern float            fmodf(float, float);
extern float            frexpf(float, int *);
extern float            ldexpf(float, int);
extern float            logf(float);
extern float            log10f(float);
extern float            modff(float, float *);
extern float            powf(float, float);
extern float            sinf(float);
extern float            sinhf(float);
extern float            sqrtf(float);
extern float            tanf(float);
extern float            tanhf(float);
extern float            erff(float);
extern float            erfcf(float);
extern float            gammaf(float);
extern float            hypotf(float, float);
extern float            j0f(float);
extern float            j1f(float);
extern float            jnf(int, float);
extern float            lgammaf(float);
extern float            y0f(float);
extern float            y1f(float);
extern float            ynf(int, float);
//extern int         isnanf(float);
extern float            acoshf(float);
extern float            asinhf(float);
extern float            atanhf(float);
extern float            cbrtf(float);
extern float            expm1f(float);
extern int              ilogbf(float);
extern float            log1pf(floata);
extern float            logbf(float);
extern float            nextafterf(float, float);
extern float            remainderf(float, float);
extern float            rintf(float);
extern float            scalbf(float, float);

extern long double      acosl(long double);
extern long double      asinl(long double);
extern long double      atanl(long double);
extern long double      atan2l(long double, long double);
extern long double      ceill(long double);
extern long double      cosl(long double);
extern long double      coshl(long double);
extern long double      expl(long double);
extern long double      fabsl(long double);
extern long double      floorl(long double);
extern long double      fmodl(long double, long double);
extern long double      frexpl(long double, int *);
extern long double      ldexpl(long double, int);
extern long double      logl(long double);
extern long double      log10l(long double);
extern long double      modfl(long double, long double *);
extern long double      powl(long double, long double);
extern long double      sinl(long double);
extern long double      sinhl(long double);
extern long double      sqrtl(long double);
extern long double      tanl(long double);
extern long double      tanhl(long double);
extern long double      erfl(long double);
extern long double      erfcl(long double);
extern long double      gammal(long double);
extern long double      hypotl(long double, long double);
extern long double      j0l(long double);
extern long double      j1l(long double);
extern long double      jnl(int, long double);
extern long double      lgammal(long double);
extern long double      y0l(long double);
extern long double      y1l(long double);
extern long double      ynl(int, long double);
//extern int         isnanl(long double);
extern long double      acoshl(long double);
extern long double      asinhl(long double);
extern long double      atanhl(long double);
extern long double      cbrtl(long double);
extern long double      expm1l(long double);
extern int              ilogbl(long double);
extern long double      log1pl(long double);
extern long double      logbl(long double);
extern long double      nextafterl(long double, long double);
extern long double      remainderl(long double, long double);
extern long double      rintl(long double);
extern long double      scalbl(long double, long double);

#if defined(_GNU_SOURCE)
void                    sincos(double x, double *sin, double *cos);
void                    sincosf(float x, float *sin, float *cos);
void                    sincosl(long double x, long double *sin,
                                long double *cos);
#endif

#endif /* __MATH_H__ */

