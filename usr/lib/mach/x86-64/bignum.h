#ifndef __MACH_X86_64_BIGNUM_H__
#define __MACH_X86_64_BIGNUM_H__

#define __USE_BIGNUM128 1

/* determine proper 128-bit scalar type */
#if defined(__GNUC__)
typedef __int128          i128;
typedef unsigned __int128 u128;
#else
typedef  __int128_t       i128
typedef  __uint128_t      u128
#endif
/* determine proper 128-bit floating-point type */
#if defined(__GNUC__)
#include <quadmath.h>
typedef __float128        f128;
#else
/* let's try */
typedef Quad              f128;
#endif

#endif /* __MACH_X86_64_BIGNUM_H__ */

