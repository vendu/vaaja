#ifndef __SYS_VOIMA_NUM_IEEE_754_H__
#define __SYS_VOIMA_NUM_IEEE_754_H__

#include <stdint.h>

#define VOIMA_IEEE_754_FLT_BITS         32
#define VOIMA_IEEE_754_FLT_SIGN_BIT     (UINT32_C(1) << 31)
#define VOIMA_IEEE_754_FLT_EXP_BITS     8
#define VOIMA_IEEE_754_FLT_MANT_BITS    23
#define VOIMA_IEEE_754_FLT_FRAC_BITS    22
//#define VOIMA_IEEE_754_FLT_EXP_BITS     9
#define VOIMA_IEEE_754_FLT_EXP_BIAS     127
#define VOIMA_IEEE_754_FLT_EXP_MASK                                     \
    (((INT32_C(1) << (VOIMA_IEEE_754_FLT_EXP_BITS + 1)) - 1)            \
     << VOIMA_IEEE_754_FLT_FRAC_BITS)
#define VOIMA_IEEE_754_FLT_MANT_MASK                                    \
    ((INT32_C(1) << (VOIMA_IEEE_754_FLT_MANT_BITS + 1)) - 1)
#define VOIMA_IEEE_754_FLT_FRAC_MASK                                    \
    ((INT32_C(1) << VOIMA_IEEE_754_FLT_MANT_BITS) - 1)

#define VOIMA_IEEE_754_DBL_SIGN_BIT     (UINT64_C(1) << 63)
#define VOIMA_IEEE_754_DBL_BITS         64
#define VOIMA_IEEE_754_DBL_EXP_BITS     11
#define VOIMA_IEEE_754_DBL_MANT_BITS    53
#define VOIMA_IEEE_754_DBL_FRAC_BITS    52
//#define VOIMA_IEEE_754_DBL_EXP_BITS     9
#define VOIMA_IEEE_754_DBL_EXP_BIAS     1023
#define VOIMA_IEEE_754_DBL_EXP_MASK                                     \
    (((UINT64_C(1) << (VOIMA_IEEE_754_DBL_EXP_BITS + 1)) - 1)           \
     << (VOIMA_IEEE_754_DBL_FRAC_BITS))
#define VOIMA_IEEE_754_DBL_MANT_MASK                                    \
    ((UINT64_C(1) << (VOIMA_IEEE_754_DBL_MANT_BITS + 1)) - 1)
#define VOIMA_IEEE_754_DBL_FRAC_MASK                                    \
    ((UINT64_C(1) << VOIMA_IEEE_754_DBL_FRAC_BITS) - 1)

#define fgetsign(f32)                   ((f32) & VOIMA_IEEE_754_FLT_SIGN_BIT)
#define fgetmant(f32)                   ((f32) & VOIMA_IEEE_754_FLT_MANT_MASK)
#define fgetexp(f32)                    ((f32) & VOIMA_IEEE_754_FLT_EXP_MASK)
#define dgetsign(f64)                   ((f64) & VOIMA_IEEE_754_DBL_SIGN_BIT)
#define dgetmant(f64)                   ((f64) & VOIMA_IEEE_754_DBL_MANT_MASK)
#define dgetfrac(f64)                   ((f64) & VOIMA_IEEE_754_DBL_FRAC_MASK)
#define dgetexp(f64)                    ((f64) & VOIMA_IEEE_754_DBL_EXP_MASK)

#endif /* __SYS_VOIMA_NUM_IEEE_754_H__ */

