#ifndef __VOIMA_FPU_H__
#define __VOIMA_FPU_H__

/* imm16-member fields */
#define FPU_UNS_BIT     (1 << 0)    // perform unsigned operation
#define FPU_NORM_BIT    (1 << 1)    // perform normalization on destination
#define FPU_TRUNC_BIT   (1 << 2)    // truncate result
#define FPU_DBL_BIT     (1 << 3)    // convert result to 64-bit value (not 32)
#define FPU_DEC_BIT     (1 << 4)    // decimal operations (as opposed to binary)
#define FPU_ARC_BIT     (1 << 5)    // arcus-trigonometry operation
#define FPU_HYP_BIT     (1 << 6)    // hyperbolic trigonometry

struct voimafpuins {
    int8_t  op;                 // unit and operation IDs
    int8_t  src : 4;            // source register ID
    int8_t  dst : 4;            // destination register ID
    int16_t imm16[];            // 16-bit immediate argument (if present)
};

/*
 * NOTES
 * -----
 * - internally, the FPU operates on 80-bit values with 64-bit significand
 *   and 16-bit exponent; the result values are then converted to 64-bit double-
 *   precision or 32-bit single precision
 * - if FPU_DBL_BIT is not set in the flg-member, we operate on 32-bit single-
 *   precision floating-point results
 */

#define FPU_FLF         0x00    // load single-precision floating-point value
#define FPU_FLD         0x01    // load double-precision floating-point value
#define FPU_FNORM       0x02    // normalize floating-point value
#define FPU_FADD        0x03    // floating-point addition
#define FPU_FSUB        0x04    // floating-point subtraction
#define FPU_FMUL        0x05    // floating-point multiplication
#define FPU_FIRP        0x06    // floating-point inverse reciprocal
#define FPU_FDIV        0x07    // floating-point division
#define FPU_FREM        0x08    // floating-point remainder
#define FPU_FSIN        0x09    // sine
#define FPU_FCOS        0x0a    // cosine
#define FPU_FTAN        0x0b    // tangent
#define FPU_FASIN       0x09    // arcus-sine
#define FPU_FACOS       0x0a    // arcus-cosine
#define FPU_FATAN       0x0b    // arcus-tangent
#define FPU_FSINH       0x09    // hyperbolic sine
#define FPU_FCOSH       0x0a    // hyperbolic cosine
#define FPU_FTANH       0x0b    // hyperbolic tangent
#define FPU_FCMP        0x0c    // floating-point comparison
#define FPU_FLGN        0x0d    // natural logarithm
#define FPU_FLOG2       0x0e    // base-2 logarithm
#define FPU_FLOG10      0x0f    // base-10 logarithm
#define FPU_FPOW        0x10    // exponentiation
#define FPU_DTOI64      0x11    // convert double-precision to 64-bit integer
#define FPU_FTOI32      0x12    // convert single-precision to 32-bit integer
#define FPU_FABS        0x13    // floating-point absolute value
#define FPU_FSQRT       0x14    // square root

#endif /* __VOIMA_FPU_H__ */

