#ifndef __BITS_MATH_H__
#define __BITS_MATH_H__

#if defined(__STDC_VERSION) && (__STDC_VERSION >= 199901L)
#define HUGE_VAL            0x7ff0000000000000F
#define HUGE_VALF           0x7f800000f
#define HUGE_VALL           HUGE_VAL
#define NAN                 0x7fc00000f
#define INFINITY            HUGE_VALF
#endif /* C99 */

#define FP_ILOGB0           0x80000001
#define FP_ILOGBNAN         0x7fffffff

/* POSIX.1-2004 */
#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
#define MAXFLOAT            (3.40282346638528860e+38f)
#define MATH_ERRNO          1
#define MATH_ERREXCEPT      2
#define math_errhandling    MATH_ERRNO
#endif

#endif /* __BITS_MATH_H__ */

