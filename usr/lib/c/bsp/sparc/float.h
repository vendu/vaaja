#ifndef __SPARC_FLOAT_H__
#define __SPARC_FLOAT_H__

#define FLT_MIN         (1.17549435E-38F)
#define FLT_MAX         (3.40282347E+38F)
#define FLT_EPSILON     (1.19209290E-07F)

#define DBL_MIN         (2.2250738585072014E-308)
#define DBL_MAX         (1.7976931348623157E+308)
#define DBL_EPSILON     (2.2204460492503131E-16)

#if defined(__sparcv9) || defined(__arch64__)

/* IEEE 128-bit floating point */
#define	LDBL_DIG	33
#define	LDBL_MANT_DIG	113
#define	LDBL_MIN_EXP	(-16381)
#define	LDBL_MAX_EXP	16384
#define	LDBL_MIN	(3.362103143112093506262677817321752603E-4932L)
#define	LDBL_MAX	(1.189731495357231765085759326628007016E+4932L)
#define	LDBL_EPSILON	(1.925929944387235853055977942584927319E-34L)
#define	LDBL_MIN_10_EXP	(-4931)
#define	LDBL_MAX_10_EXP	4932
#define DECIMAL_DIG     36

#else /* IEEE 64-bit floating point */

#define	LDBL_DIG	DBL_DIG
#define	LDBL_MANT_DIG	DBL_MANT_DIG
#define	LDBL_MIN_EXP	DBL_MIN_EXP
#define	LDBL_MAX_EXP	DBL_MAX_EXP
#define	LDBL_MIN	DBL_MIN
#define	LDBL_MAX	DBL_MAX
#define	LDBL_EPSILON	DBL_EPSILON
#define	LDBL_MIN_10_EXP	DBL_MIN_10_EXP
#define	LDBL_MAX_10_EXP	DBL_MAX_10_EXP
#define DECIMAL_DIG     DBL_DECIMAL_DIG

#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

/*
 * values for FLT_EVAL_METHOD
 * -1 - indeterminable
 *  0 - evaluate just to the range and precision of the type
 *  1 - evaluate float and double to the range and precision of double
 *      evaluate long double to its range and precision
 *  2 - evaluate all operations and constants to the range and precision of
 *      long double
 */
#undef  FLT_EVAL_METHOD
#define FLT_EVAL_METHOD 0

#endif /* __STDC_VERSION >= 199901L */

#endif /* __SPARC_FLOAT_H__ */

