#ifndef __V0_FPU_H__
#define __V0_FPU_H__

#define V0_FPU_REGISTERS        32

typedef float  v0flt;   // IEE754 32-bit single-precision value
typedef double v0dbl;   // IEE754 64-bit double-precision value

/* F and L suffixes for float and long double (as opposed to double) operands */
/* FADD, FSUB, FMUL, FDIV, FREM, FIRP, FTOI, FINT
/* FPOW:    e**x */
/* FPOW2:   2**x */
/* FPOW10:  10**x */
/* FPOW1:   e**x - 1 */
/* FPOW21   2**x - 1 */
/* FPOW101: 10**x - 1 */
/* FLN, FLG2, FLOG */
/* FLNP1:   ln(1 + x) */
/* FLG2P1:  log2(1 + x) */
/* FL10P1:  log10(1 + x) */
/* FSQR:    sqrt(x) */
/* FSQR2:   sqrt(x**2 + y** 2) */
/* FEXP1:   (1 + x)**n */
/* FEXPI:   x**(1/n) */
/* FSIN, FCOS, FTAN, FASIN, FACOS, FATAN */
/* FSINH, FCOSH, FTANH, FASINH, FACOSH, FATANH */

#define V0_FST_RF               (1 << 31)
#define V0_FST_TF               (1 << 30)
#define V0_FST_ZI               (1 << 29)
#define V0_FST_NI               (1 << 28)
#define V0_FST_ZF               (1 << 27)
#define V0_FST_NF               (1 << 26)
#define V0_FST_UF               (1 << 25)
#define V0_FST_OF               (1 << 24)

/* floating-point registers */
#define V0_FST_REG              V0_FP0_REG
#define V0_FRT_REG              V0_FP1_REG
#define V0_FP0_REG              0x00
#define V0_FP1_REG              0x01
#define V0_FP2_REG              0x02
#define V0_FP3_REG              0x03
#define V0_FP4_REG              0x04
#define V0_FP5_REG              0x05
#define V0_FP6_REG              0x06
#define V0_FP7_REG              0x07
#define V0_FP8_REG              0x08
#define V0_FP9_REG              0x09
#define V0_FP10_REG             0x0a
#define V0_FP11_REG             0x0b
#define V0_FP12_REG             0x0c
#define V0_FP13_REG             0x0d
#define V0_FP14_REG             0x0e
#define V0_FP15_REG             0x0f

#endif /* __V0_FPU_H__ */

