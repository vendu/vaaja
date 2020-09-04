#ifndef __V0IMA_FXP_H__
#define __V0IMA_FXP_H__

#define VOIMA_FXP8_INT_BITS     9       // 8 bits + sign
#define VOIMA_FXP8_FRAC_BITS    23
#define VOIMA_FXP16_INT_BITS    17      // 16 bits + sign
#define VOIMA_FXP16_FRAC_BITS   15
#define VOIMA_FXP24_INT_BITS    25      // 24 bits + sign
#define VOIMA_FXP24_FRAC_BITS   7

#define VOIMA_FXSR_DZ_BIT   (1 << 0)    // division by zero
#define VOIMA_FXSR_OF_BIT   (1 << 1)    // overflow
#define VOIMA_FXSR_UF_BIT   (1 << 2)    // underflow

typedef voimafxp_t          int32_t;    // programmer presentation
typedef voimawfxp_t         int64_t;    // internal presentation

#define voimafxpadd(a, b)   ((b) + (a))
#define voimafxpsub(a, b)   ((b) - (a))
#define voimafxpmul(a, b)   ((b) * (a)) // result is Q31.32 fixed-point value

#define VOIMA_FXP_RND_BIT   (1L << 15)

#endif __V0IMA_FXP_H__

