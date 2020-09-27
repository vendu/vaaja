#ifndef __SYS_VOIMA_INST_EXT_H__
#define __SYS_VOIMA_INST_EXT_H__

#define V0_COPROC_UNIT          0x0f

#define V0_IO_COPROC            0x00
#define V0_IOC                  0x00    // setbuf, probe, conf, seek
#define V0_IOP                  0x01    // chk, set, clr
#define V0_IOR                  0x01    // read operation; flg (RAW|COOK|BUF)
#define V0_IOW                  0x02    // write operation; flg (RAW|COOK|BUF)

/* floating-point coprocessor instruction set */
#define V0_FP_COPROC            0x01
#define V0_FINORM               0x01
#define V0_FLNORM               0x02
#define V0_FSNORM               0x03
#define V0_FDNORM               0x04
#define V0_FF2FX                0x05
#define V0_FD2FX                0x06
#define V0_FADD                 0x07
#define V0_FSUB                 0x08
#define V0_FMUL                 0x09
#define V0_FDIV                 0x0a
#define V0_FSIN                 0x0b
#define V0_FCOS                 0x0c
#define V0_FTAN                 0x0d
#define V0_FASIN                0x0e
#define V0_FACOS                0x0f
#define V0_FATAN                0x10
#define V0_FSINH                0x11
#define V0_FCOSH                0x12
#define V0_FTANH                0x13
#define V0_FLN                  0x14
#define V0_FLOG                 0x15
#define V0_FLG2                 0x16
#define V0_FSQRT                0x17
#define V0_FEXP                 0x18
#define V0_FLOOR                0x19
#define V0_FCEIL                0x1a

/* fixed-point coprocessor instruction set */
#define V0_FXP_COPROC           0x02
#define V0_FXP_ADD              0x00    // fxd = fx1 + fx2;
#define V0_FXP_SUB              0x01    // fxd = fx2 - fx1;
#define V0_FXP_MUL              0x02    // fxd = fx1 * fx2;
#define V0_FXP_IDIV             0x03    // fxd = fx1 / i2;
#define V0_FXP_DIV              0x04    // fxd = fx1 / fx2;
#define V0_FXP_INV              0x05    // fxd = 1 / fx1;
#define V0_FXP_AVG              0x05    // fxd = (fx1 + fx2) >> 1;
#define V0_FXP_HAM              0x06    // fxd = ham(fx1);
#define V0_FXP_CTZ              0x07    // fxd = ctz(fx1);
#define V0_FXP_CLZ              0x08    // fxd = clz(fx1);
#define V0_FXP_HSH              0x09    // fxd = hsh(fx1);
#define V0_FXP_HUN              0x0a    // fxd = hun(fx1);

#define V0_DSP_COPROC           0x03    // digital signal processor

#DEFINE V0_GFX_COPROC           0x04    // graphics coprocessor

#define V0_PFX_COPROC           0x05    // physics coprocessor

#define V0_RAT_COPROC           0x06    // rational-number coprocessor

#endif /* __SYS_VOIMA_INST_EXT_H__ */

