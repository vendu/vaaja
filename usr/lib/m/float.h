#ifndef __FLOAT_H__
#define __FLOAT_H__

#include <features.h>
#include <bits/ieee754.h>
#if defined(__x86_64__) || defined(__amd64__)                      \
    || defined(__i386__) || defined(__i486__) || defined(__i586__) \
    || defined(__i686__)
#include <ia32/float.h>
#elif defined(__arm__)
#include <arm/float.h>
#elif defined(sparc) || defined(__sparc) || defined(__sparc__)
#include <sparc/float.h>
#endif

#define FLT_RADIX  2
#define DBL_RADIX  FLT_RADIX
#define LDBL_RADIX FLT_RADIX

/*
 * values for FLT_ROUNDS
 * -1 - indeterminable
 *  0 - toward zero (FE_TOWARDZERO)
 *  1 - to nearest (FE_TONEAREST)
 *  2 - toward positive infinity (FE_UPWARD)
 *  3 - toward negative infinity (FE_DOWNWARD)
 */
#define FLT_ROUNDS  fegetround()
/* TODO: fix DBL_ROUNDS and LDBL_ROUNDS */
#define DBL_ROUNDS  FLT_ROUNDS
#define LDBL_ROUNDS FLT_ROUNDS

/*
 * FIXME
 * -----
 * - *_HAS_SUBNORM
 */

/*
 * values for FLT_HAS_SUBNORM, DBL_HAS_SUBNORM, and LDBL_HAS_SUBNORM
 * -1 - indeterminable
 *  0 - absent (type does not support subnormal numbers)
 *  1 - present (type does support subnormal numbers)
 */

#if defined(__STDC_VERSION__) && (__STDC_VERSION >= 201112L)

#undef FLT_TRUE_MIN
#undef DBL_TRUE_MIN
#undef LDBL_TRUE_MIN
/* *_DECIMAL_DIG come from <bits/ieee754.h> */
#define FLT_HAS_SUBNORM  __FLT_HAS_DENORM__
#define DBL_HAS_SUBNORM  __DBL_HAS_DENORM__
#define LDBL_HAS_SUBNORM __LDBL_HAS_DENORM__

/* fixes for some systems/compilers */
#if defined(__GNUC__) || defined(__clang__)

#if !defined(FLT_TRUE_MIN) && defined(__FLT_HAS_DENORM__)
#define FLT_TRUE_MIN    __FLT_DENORM_MIN__
#endif /* denormal float numbers */

#if !defined(DBL_TRUE_MIN) && defined(__DBL_HAS_DENORM__)
#define DBL_TRUE_MIN    __DBL_DENORM_MIN__
#endif /* denormal double numbers */

#if !defined(LDBL_TRUE_MIN) && defined(__LDBL_HAS_DENORM__)
#define LDBL_TRUE_MIN   __LDBL_DENORM_MIN__
#endif /* denormal long double numbers */

#endif /* __GNUC__ || __clang__ */

#endif /* C11 */

#endif /* __FLOAT_H__ */

