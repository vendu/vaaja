#ifndef DECK_IEEE_754_H__
#define DECK_IEEE_754_H__

/* FIXME: add 128-bit unions and pick the right ones for long double :) */

#define LONG_DOUBLE_SIZE                64

#if !defined(__FLOAT_WORD_ORDER)
#define __FLOAT_WORD_ORDER              __BYTE_ORDER
#endif

#define IEEE_754_FLOAT_BIAS             0x7f
#define IEEE_754_DOUBLE_BIAS            0x3ff
#define IEEE_754_LONG_DOUBLE80_BIAS     0x3fff
#define IEEE_754_LONG_DOUBLE128_BIAS    0x7fff

#if defined(_GNU_SOURCE)
#define ieee754_float                   __ieee754f
#define ieee754_double                  __ieee754d
#if defined(LONG_DOUBLE_SIZE) && (LONG_DOUBLE_SIZE == 80)
#define ieee754_long_double             __ieee754ld80
#elif defined(LONG_DOUBLE_SIZE) && (LONG_DOUBLE_SIZE == 128)
#define ieee754_long_double             __ieee754ld128
#endif
#define val                             ieee
#define nan                             ieee_nan
#define mantissa                        mant
#define mantissa0                       mant0
#define mantissa1                       mant1
#define exponent                        exp
#define negative                        sign
#define quiet_nan                       quiet
#endif

#if defined(LONG_DOUBLE_SIZE) && (LONG_DOUBLE_SIZE == 80)
#define IEEE_754_LONG_DOUBLE_BIAS       IEEE_754_LONG_DOUBLE_BIAS
#define __ieee754ld                     __ieee754ld64
#elif defined(LONG_DOUBLE_SIZE) && (LONG_DOUBLE_SIZE == 80)
#define IEEE_754_LONG_DOUBLE_BIAS       IEEE_754_LONG_DOUBLE80_BIAS
#define __ieee754ld                     __ieee754ld80
#elif defined(LONG_DOUBLE_SIZE) && (LONG_DOUBLE_SIZE == 128)
#define IEEE_754_LONG_DOUBLE_BIAS       IEEE_754_LONG_DOUBLE128_BIAS
#define __ieee754ld                     __ieee754ld128
#endif

#define DECK_IEEE_754_SIGN_BITS         1
#define DECK_IEEE_754_NAN_QUIET_BITS    1
#define DECK_IEEE_754_LD80_ONE_BITS     1
#define DECK_IEEE_754_FLOAT_MANT_BITS   24
#define DECK_IEEE_754_FLOAT_EXP_BITS    8
#define DECK_IEEE_754_DOUBLE_MANT_BITS  53
#define DECK_IEEE_754_DOUBLE_MANT0_BITS 20
//#define DECK_IEEE_754_DOUBLE_MANT1_BITS 32
#define DECK_IEEE_754_DOUBLE_EXP_BITS   11
#define DECK_IEEE_754_LD80_RES_BITS     16
#define DECK_IEEE_754_LD80_EXP_BITS     52
#define DECK_IEEE_754_LD80_MANT_BITS   20
//#define DECK_IEEE_754_LD80_MANT1_BITS   32
#define DECK_IEEE_754_LD128_MANT_BITS  48
//#define DECK_IEEE_754_LD128_MANT1_BITS  64
#define DECK_IEEE_754_LD_EXP_BITS       15

#if (__BYTE_ORDER == __LITTLE_ENDIAN)

union __ieee754f {
    float f;
    struct {
        unsigned                        mant     : DECK_IEEE_754_FLOAT_MANT_BITS;
        unsigned                        exp      : DECK_IEEE_754_FLOAT_EXP_BITS;
        unsigned                        sign     : DECK_IEEE_754_SIGN_BITS;
    } val;
    struct {
        unsigned                        mant     : DECK_IEEE_754_FLOAT_MANT_BITS - DECK_IEEE_754_NAN_QUIET_BITS;
        unsigned                        quiet    : DECK_IEEE_754_NAN_QUIET_BITS;
        unsigned                        exp      : DECK_IEEE_754_FLOAT_EXP_BITS;
        unsigned                        sign     : DECK_IEEE_754_SIGN_BITS;
    } nan;
};

#elif (__BYTE_ORDER == __BIG_ENDIAN)

union __ieee754f {
    float f;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_FLOAT_EXP_BITS;
        unsigned                         mant    : DECK_IEEE_754_FLOAT_MANT_BITS;
    } val;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_FLOAT_EXP_BITS;
        unsigned                         quiet   : DECK_IEEE_754_NAN_QUIET_BITS;
        unsigned                         mant    : DECK_IEEE_754_FLOAT_MANT_BITS - DECK_IEEE_754_NAN_QUIET_BITS;
    } nan;
};

union __ieee754d {
    double d;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_DOUBLE_EXP_BITS;
        unsigned                         mant    : DECK_IEEE_754_DOUBLE_MANT_BITS;
    } val;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_DOUBLE_EXP_BITS;
        unsigned                         quiet   : DECK_IEEE_754_NAN_QUIET_BITS;
        unsigned                         mant    : DECK_IEEE_754_DOUBLE_MANT_BITS - DECK_IEEE_754_NAN_QUIET_BITS;
    } nan;
};

union __ieee754ld80 {
    long double ld;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_LD_EXP_BITS;
        unsigned                         _res    : DECK_IEEE_754_LD80_RES_BITS;
        unsigned                         mant    : DECK_IEEE_754_LD80_MANT_BITS;
    } val;
    struct {
        unsigned                         sign    : DECK_IEEE_754_SIGN_BITS;
        unsigned                         exp     : DECK_IEEE_754_LD_EXP_BITS;
        unsigned                         _res    : DECK_IEEE_754_LD80_RES_BITS;
        unsigned                         one     : DECK_IEEE_754_LD80_ONE_BITS;
        unsigned                         quiet   : DECK_IEEE_754_NAN_QUIET_BITS;
        unsigned                         mant    : DECK_IEEE_754_LD80_MANT_BITS - DECK_IEEE_754_LD80_ONE_BITS - DECK_IEEE_NAN_QUIET_BITS;
    } nan;
};

#endif /* __BYTE_ORDER */

#endif /* DECK_IEEE_754_H__ */

