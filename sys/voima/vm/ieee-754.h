#ifndef __VOIMA_VM_IEEE_754_H__
#define __VOIMA_VM_IEEE_754_H__

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
     << VOIMA_IEEE_754_FLT_MANT_BITS)
#define VOIMA_IEEE_754_FLT_MANT_MASK                                    \
    ((INT32_C(1) << (VOIMA_IEEE_754_FLT_MANT_BITS + 1)) - 1)
#define VOIMA_IEEE_754_FLT_FRAC_MASK                                    \
    ((INT32_C(1) << VOIMA_IEEE_754_FLT_MANT_BITS) - 1)

#define VOIMA_IEEE_754_DBL_SIGN_BIT     (UINT64_C(1) << 31)

#define fgetsign(fu32)                  ((fu32) & VOIMA_IEEE_754_FLT_SIGN_BIT)
#define fgetmant(fu32)                  ((fu32) & VOIMA_IEEE_754_FLT_MANT_MASK)
#define fgetexp(fu32)                   ((fu32) & VOIMA_IEEE_754_FLT_EXP_MASK)

#endif /* __VOIMA_VM_IEEE_754_H__ */

