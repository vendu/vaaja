#ifndef DECK_FPU_H
#define DECK_FPU_H

typedef float                   m_fsp_t;
typedef double                  m_fdp_t;

#if defined(V0_FP_EXTENSION)

#define V0_FPU_REG_BITS         64

#if defined(V0_FP_IEEE_754)

#define V0_FLOAT_BITS           32
#define V0_FLOAT_MANT_BITS      24
#define V0_FLOAT_EXP_BITS       8
#define V0_FLOAT_EXP_BIAS       0x7f

#define V0_DOUBLE_BITS          64
#define V0_DOUBLE_MANT_BITS     53
#define V0_DOUBLE_EXP_BITS      11
#define V0_DOUBLE_EXP_BIAS      1023

struct deckfloat {
    unsigned                    mant    : V0_FLOAT_MANT_BITS;
    unsigned                    exp     : V0_FLOAT_EXP_BITS;
};
#define V0_FLOAT_SIZE           (V0_FLOAT_MANT_BITS                     \
                                 + V0_FLOAT_EXP_BITS)

struct deckdouble {
    unsigned                    mant    : V0_DOUBLE_MANT_BITS;
    unsigned                    exp     : V0_DOUBLE_EXP_BITS;
};
#define V0_DOUBLE_SIZE          (V0_DOUBLE_MANT_BITS                    \
                                 + V0_DOUBLE_EXP_BITS)

#endif /* defined(V0_FP_IEEE_754) */

#define V0_FPON_OP              0x00    // enable fixed-point unit
#define V0_FPADD_OP             0x01    // addition
#define V0_FPSUB_OP             0x02    // subtraction
#define V0_FPMUL_OP             0x03    // multiplication
#define V0_FPDIV_OP             0x04    // division
#define V0_FPXTM                0x05    // extract mantissa to integer
#define V0_FPXTF                0x06    // extract fraction to integer
#define V0_FPXTE                0x07    // extract exponent to integer
#define V0_FCMP                 0x08    // compare floating-point values

#endif /* defined(V0_FP_EXTENSION) */

#endif /* DECK_FPU_H */

