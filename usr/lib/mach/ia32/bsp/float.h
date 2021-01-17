#ifndef __BSP_IA32_FLOAT_H__
#define __BSP_IA32_FLOAT_H__

//#include <ia32/i387.h>
//
#if defined(__x86_64__) || defined(__amd64__)
#include <x86-64/float.h>
#endif

#define DECIMAL_DIG         21
/*
 * values for FLT_EVAL_METHOD
 * -1 - indeterminable
 *  0 - evaluate just to the range and precision of the type
 *  1 - evaluate float and double to the range and precision of double
 *      evaluate long double to its range and precision
 *  2 - evaluate all operations and constants to the range and precision of
 *      long double
 */
#if !defined(FLT_EVAL_METHOD)
#define FLT_EVAL_METHOD     2
#endif

#define FLT_MIN             (1.17549435082228750797E-38F)
#define FLT_MAX             (3.40282346638528859812E+38F)
#define FLT_EPSILON         (1.19209289550781250000E-7F)

#define DBL_MIN             ((double)2.22507385850720138309E-308L)
#define DBL_MAX             ((double)1.79769313486231570815E+308L)
#define DBL_EPSILON         ((double)2.22044604925031308085E-16L)

#define LDBL_MIN            (3.36210314311209350626E-4932L)
#define LDBL_MAX            (1.18973149535723176502E+4932L)
#define LDBL_EPSILON        (1.08420217248550443401e-19L)

/* IEEE 80-bit floating point */
#define	LDBL_DIG	    18
#define	LDBL_MANT_DIG	    64
#define	LDBL_MIN_EXP	    (-16381)
#define	LDBL_MAX_EXP	    16384
#define	LDBL_MIN_10_EXP	    (-4931)
#define	LDBL_MAX_10_EXP	    4932

#if !defined(__FLT_DENORM_MIN__)
#define __FLT_DENORM_MIN__  (1.40129846432481707092E-45F)
#endif
#if !defined(__DBL_DENORM_MIN__)
#define __DBL_DENORM_MIN__  ((double)4.94065645841246544177E-324L)
#endif
#if !defined(__LDBL_DENORM_MIN__)
#define __LDBL_DENORM_MIN__ (3.64519953188247460253E-4951L)
#endif

#endif /* __BSP_IA32_FLOAT_H__ */

