/* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0376c/Babbfeda.html */
/* http://www.arch.eece.maine.edu/ece473/images/4/4a/Ece473_hw3_IEEE754.pdf */
#ifndef __BITS_IEEE754_H__
#define __BITS_IEEE754_H__

#include <stdint.h>

/* characteristics of [32-bit] single precision type float */
#define FLT_DIG         6
#define FLT_MANT_DIG    24
#define FLT_MIN_EXP     (-125)
#define FLT_MAX_EXP     128
#define FLT_MIN_10_EXP  (-37)
#define FLT_MAX_10_EXP  38
#define FLT_DECIMAL_DIG 9
/* unsigned [integer] bit masks for IEEE-754 32-bit float */
#define __FLT_SIGNBIT   0x80000000U
#define __FLT_MASK      0x7fffffffU
#define __FLT_EXPBITS   0x7f800000U
#define __FLT_MANTBITS  0x007fffffU

/* characteristics of [64-bit] double precision type double */
#define DBL_DIG         15
#define DBL_MANT_DIG    53
#define DBL_MIN_EXP     (-1021)
#define DBL_MAX_EXP     1024
#define DBL_MIN_10_EXP  (-307)
#define DBL_MAX_10_EXP  308
#define DBL_DECIMAL_DIG 17
/* unsigned [integer] bit masks for IEEE-754 64-bit double */
#define __DBL_SIGNBIT   UINT64_C(0x8000000000000000)
#define __DBL_MASK      UINT64_C(0x7fffffffffffffff)
#define __DBL_EXPBITS   UINT64_C(0x7ff0000000000000)
#define __DBL_MANTBITS  UINT64_C(0x000fffffffffffff)

/* LDBL-macros are defined in architecture header files, e.g. <ia32/float.h> */

#define fpclassify(x)                                                   \
    (((sizeof(x) == sizeof(double))                                     \
      ? __fpclassify(x)                                                 \
      : (((sizeof(x) == sizeof(float))                                  \
          ? __fpclassifyf(x)                                            \
          : __fpclassifyl(x)))))

/*
 * isnanl() and __isqnanl() are defined in a per-architecture header such as
 * <x86-64/math.h>
 */
/* NOTE: isinf(), isnan(), finite() are from BSD */
#define __isinf(x)                                                      \
    ((((*(uint64_t *)&(x)) & __DBL_EXPBITS) == __DBL_EXPBITS)           \
     && (!(*(uint64_t *)&(x)) & __DBL_MANTBITS))
#define __isinff(x)                                                     \
    ((((*(uint32_t *)&(x)) & __FLT_EXPBITS) == __FLT_EXPBITS)           \
     && (!(*(uint32_t *)&(x)) & __FLT_MANTBITS))
#define __isnan(x)                                                      \
    ((((*(uint64_t *)&(x)) & __DBL_EXPBITS) == __DBL_EXPBITS)           \
     && ((*(uint64_t *)&(x)) & UINT64_C(0x000fffffffffffff)))
#define __isnanf(x)                                                     \
    ((((*(uint32_t *)&(x)) & __FLT_EXPBITS) == __FLT_EXPBITS)           \
     && ((*(uint32_t *)&(x)) & __FLT_MANTBITS))
#define __issignaling(x)                                                \
    (__isnan(x) && !((*(uint64_t *)&(x)) & UINT64_C(0x0008000000000000)))
#define __issignalingf(x)                                               \
    (__isnanf(x) && !((*(uint32_t *)&(x)) & 0x00400000))

#define __isfinite(x)                                                   \
    (!__isinf(x))
#define __isfinitef(x)                                                  \
    (!__isinff(x))
#define __isfinitel(x)                                                  \
    (!__isinfl(x))

#define isfinite(x)                                                     \
    (((sizeof(x) == sizeof(double))                                     \
      ? __isfinite(x)                                                   \
      : (((sizeof(x) == sizeof(float))                                  \
          ? __isfinitef(x)                                              \
          : __isfinitel(x)))))
#define isnormal(x)                                                     \
    (((sizeof(x) == sizeof(double))                                     \
      ? (__fpclassify(x) == FP_NORMAL)                                  \
      : (((sizeof(x) == sizeof(float))                                  \
          ? ( __fpclassifyf(x) == FP_NORMAL)                            \
          : (__fpclassifyl(x) == FP_NORMAL)))))
#define isnan(x)                                                        \
    (((sizeof(x) == sizeof(double))                                     \
      ? (__isnan(x))                                                    \
      : (((sizeof(x) == sizeof(float))                                  \
          ? (__isnanf(x))                                               \
          : (__isnanl(x))))))
#define issignaling(x)                                                  \
    (((sizeof(x) == sizeof(double))                                     \
      ? (__issignaling(x))                                              \
      : (((sizeof(x) == sizeof(float))                                  \
          ? (__issignalingf(x))                                         \
          : (__issignalingl(x))))))

#if defined(_BSD_SOURCE)
#define isinf(x)        __isinf(x)
#define isinff(x)       __isinff(x)
#define isinfl(x)       __isinfl(x)
#if !defined(isnan)
#define isnan(x)        __isnan(x)
#define isnanf(x)       __isnanf(x)
#define isnanl(x)       __isnanl(x)
#endif /* !defined(isnan) */
#if defined(_BSD_SOURCE) && defined(_FAVOR_BSD)
/* finite() returns non-zero for INFINITY or NaN */
#define finite(x)       (!(__isinf(x) || isnan(x)))
#define finitef(x)      (!(__isinff(x) || __isnanf(x)))
#define finitel(x)      (!(__isinfl(x) || __isnanl(x)))
#endif /* defined(_BSD_SOURCE) && defined(_FAVOR_BSD) */

#if defined(_ZERO_SOURCE) || defined(_GNU_SOURCE)

#if !defined(__FLOAT_WORD_ORDER)
#define __FLOAT_WORD_ORDER          __BYTE_ORDER
#endif

#define IEEE754_FLOAT_BIAS          0x7f
#define IEEE754_DOUBLE_BIAS         0x3ff
#define IEEE754_LONG_DOUBLE80_BIAS  0x3fff
#define IEEE754_LONG_DOUBLE128_BIAS 0x7fff

#if defined(_GNU_SOURCE)
#define ieee754_float               __ieee754f
#define ieee754_double              __ieee754d
#if defined(LONGDOUBLESIZE) && (LONGDOUBLESIZE == 80)
#define ieee754_long_double         __ieee754ld80
#elif defined(LONGDOUBLESIZE) && (LONGDOUBLESIZE == 128)
#define ieee754_long_double         __ieee754ld128
#endif
#define val                         ieee
#define nan                         ieee_nan
#define mantissa                    mant
#define mantissa0                   mant0
#define mantissa1                   mant1
#define exponent                    exp
#define negative                    sign
#define quiet_nan                   quiet
#endif

#if defined(LONGDOUBLESIZE) && (LONGDOUBLESIZE == 80)
#define IEEE754_LONG_DOUBLE_BIAS    IEEE754_LONG_DOUBLE80_BIAS
#define __ieee754ld                 __ieee754ld80
#elif defined(LONGDOUBLESIZE) && (LONGDOUBLESIZE == 128)
#define IEEE754_LONG_DOUBLE_BIAS    IEEE754_LONG_DOUBLE128_BIAS
#define __ieee754ld                 __ieee754ld128
#endif

#define __IEEE754_SIGN_BITS         1
#define __IEEE754_NAN_QUIET_BITS    1
#define __IEEE754_LD80_ONE_BITS     1
#define __IEEE754_FLOAT_MANT_BITS   23
#define __IEEE754_FLOAT_EXP_BITS    8
#define __IEEE754_DOUBLE_MANT_BITS  52
#define __IEEE754_DOUBLE_MANT0_BITS 20
#define __IEEE754_DOUBLE_MANT1_BITS 32
#define __IEEE754_DOUBLE_EXP_BITS   11
#define __IEEE754_LD80_RES_BITS     16
#define __IEEE754_LD80_EXP_BITS     16
#define __IEEE754_LD80_MANT0_BITS   32
#define __IEEE754_LD80_MANT1_BITS   32
#define __IEEE754_LD128_MANT0_BITS  48
#define __IEEE754_LD128_MANT1_BITS  64
#define __IEEE754_LD_EXP_BITS       15

#endif /* __BITS_IEEE754_H__ */

