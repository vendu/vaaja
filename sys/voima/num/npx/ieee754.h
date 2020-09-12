#ifndef __IEEE_754_H__
#define __IEEE_754_H__

#include <features.h>
#include <endian.h>
//#include <zero/param.h>
#include <voima/num/npx/bits/ieee754.h>

/* FIXME: add 128-bit unions and pick the right ones for long double :) */

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

