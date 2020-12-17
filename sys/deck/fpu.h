#ifndef DECK_FPU_H
#define DECK_FPU_H

#if defined(DECK_FP_EXTENSION)

#define DECK_FPU_REG_BITS       64
#define DECK_FP0_REG

typedef int64_t                 deckfpureg_t;

#if defined(DECK_FP_IEEE_754)

#define DECK_FLOAT_BITS         32
#define DECK_FLOAT_MANT_BITS    24
#define DECK_FLOAT_EXP_BITS     8
#define DECK_FLOAT_EXP_BIAS     0x7f

#define DECK_DOUBLE_BITS        64
#define DECK_DOUBLE_MANT_BITS   53
#define DECK_DOUBLE_EXP_BITS    11
#define DECK_DOUBLE_EXP_BIAS    1023

struct deckfloat {
    unsigned                            mant    : DECK_FLOAT_MANT_BITS;
    unsigned                            exp     : DECK_FLOAT_EXP_BITS;
};

struct deckdouble {
    unsigned                            mant    : DECK_DOUBLE_MANT_BITS;
    unsigned                            exp     : DECK_DOUBLE_EXP_BITS;
};

#endif

#define DECK_FPON_OP            0x00    // enable fixed-point unit
#define DECK_FPADD_OP           0x01    // addition
#define DECK_FPSUB_OP           0x02    // subtraction
#define DECK_FPMUL_OP           0x03    // multiplication
#define DECK_FPDIV_OP           0x04    // division
#define DECK_FPXTM              0x05    // extract mantissa to integer
#define DECK_FPXTF              0x06    // extract fraction to integer
#define DECK_FPXTE              0x07    // extract exponent to integer
#define DECK_FCMP               0x08    // compare floating-point values

#endif /* defined(DECK_FP_EXTENSION) */

#endif /* DECK_FPU_H */

