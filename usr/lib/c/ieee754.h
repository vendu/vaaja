#ifndef __IEEE_754_H__
#define __IEEE_754_H__

#include <features.h>
#include <endian.h>
#include <zero/param.h>

/* FIXME: add 128-bit unions and pick the right ones for long double :) */

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
#define __IEEE754_LD80_EXP_BITS     52
#define __IEEE754_LD80_MANT0_BITS   20
#define __IEEE754_LD80_MANT1_BITS   32
#define __IEEE754_LD128_MANT0_BITS  48
#define __IEEE754_LD128_MANT1_BITS  64
#define __IEEE754_LD_EXP_BITS       15

#if (__BYTE_ORDER == __LITTLE_ENDIAN)

union __ieee754f {
    float f;
    struct {
        unsigned mant  : __IEEE754_FLOAT_MANT_BITS;
        unsigned exp   : __IEEE754_FLOAT_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
    } val;
    struct {
        unsigned mant  : __IEEE754_FLOAT_MANT_BITS - __IEEE754_NAN_QUIET_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned exp   : __IEEE754_FLOAT_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
    } nan;
};

union __ieee754ld128 {
    long double ld;
    struct {
        unsigned mant1 : __IEEE754_LD128_MANT1_BITS;
        unsigned mant0 : __IEEE754_LD128_MANT0_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
    } val;
};

#if (__FLOAT_WORD_ORDER == __LITTLE_ENDIAN)

union __ieee754d {
    double d;
    struct {
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
    } val;
    struct {
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS - __IEEE754_NAN_QUIET_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
    } nan;
};

union __ieee754ld80 {
    long double ld;
    struct {
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
    } val;
    struct {
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
    } nan;
};

#elif (__FLOAT_WORD_ORDER == __BIG_ENDIAN)

union __ieee754d {
    double d;
    struct {
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
    } val;
    struct {
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS - __IEEE754_NAN_QUIET_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
    } nan;
};

union __ieee754ld80 {
    long double ld;
    struct {
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS;
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
    } val;
    struct {
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS - __IEEE_NAN_QUIET_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned one   : __IEEE754_LD80_ONE_BITS;
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
    } nan;
};

#endif /* __FLOAT_WORD_ORDER */

#elif (__BYTE_ORDER == __BIG_ENDIAN)

union __ieee754f {
    float f;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_FLOAT_EXP_BITS;
        unsigned mant  : __IEEE754_FLOAT_MANT_BITS;
    } val;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_FLOAT_EXP_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned mant  : __IEEE754_FLOAT_MANT_BITS - __IEEE754_NAN_QUIET_BITS;
    } nan;
};

union __ieee754d {
    double d;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS;
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
    } val;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_DOUBLE_EXP_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned mant0 : __IEEE_DOUBLE_MANT0_BITS - __IEEE754_NAN_QUIET_BITS;
        unsigned mant1 : __IEEE754_DOUBLE_MANT1_BITS;
    } nan;
};

union __ieee754ld80 {
    long double ld;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS;
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
    } val;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned _res  : __IEEE754_LD80_RES_BITS;
        unsigned one   : __IEEE754_LD80_ONE_BITS;
        unsigned quiet : __IEEE754_NAN_QUIET_BITS;
        unsigned mant0 : __IEEE754_LD80_MANT0_BITS - __IEEE754_LD80_ONE_BITS - __IEEE_NAN_QUIET_BITS;
        unsigned mant1 : __IEEE754_LD80_MANT1_BITS;
    } nan;
};

union __ieee754ld128 {
    long double ld;
    struct {
        unsigned sign  : __IEEE754_SIGN_BITS;
        unsigned exp   : __IEEE754_LD_EXP_BITS;
        unsigned mant0 : __IEEE754_LD128_MANT0_BITS;
        unsigned mant1 : __IEEE754_LD128_MANT1_BITS;
    } val;
};

#endif /* __BYTE_ORDER */

#enif /* defined(_ZERO_SOURCE) || defined(_GNU_SOURCE) */

#endif /* __IEEE_754_H__ */

