#ifndef __MACH_BIGNUM_H__
#define __MACH_BIGNUM_H__

#if defined(__x86_64__) || defined(__amd64__) || defined(_M_AMD64)
#include <mach/x86-64/bignum.h>
#include <mach/op128.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
//#include <mach/ia32/bignum.h>
#include <mach/op64.h>
#elif defined(__arm__)
#include <mach/arm/bignum.h>
#elif defined(__ppc__)
#include <mach/ppc/bignum.h>
#endif

#if defined(__USE_BIGNUM128) && (__USE_BIGNUM128)

u128 addu128(u128 a, u128 b);
u128 mulu128(u128 a, u128 b);

static __inline__ i128
add128(i128 a, i128 b) {

    return a + b;
}

static __inline__ i128
sub128(i128 a, i128 b) {

    return a - b;
}

static __inline__ i128
mul128(i128 a, i128 b) {

    return a * b;
}

static __inline__ i128
div128(i128 a, i128 b) {

    return a / b;
}

#define mod128(a, b) rem128(a, b)

static __inline__ i128
rem128(i128 a, i128 b) {

    return a % b;
}

#endif

#endif /* __MACH_BIGNUM_H__ */

