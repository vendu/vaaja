#ifndef __SYS_VCODE_INST_H__
#define __SYS_VCODE_INST_H__

#define V_MAX_REGISTERS 256

/* INSTRUCTION FLAGS */
#define V_BOOLEAN_BIT   (1 << 0)    // boolean
#define V_ADDRESS_BIT   (1 << 1)    // address (pointer value)
#define V_FLOAT_BIT     (1 << 2)    // floating-point
#define V_FIXED_BIT     (1 << 3)    // fixed point
#define V_SIMD_BIT      (1 << 4)    // SIMD vector-operation
#define V_DSP_BIT       (1 << 5)    // DSP-operation

/* INSTRUCTIONS */
#define V0_PAD          0x00    // padding byte-value
#define V0_RED          0xaa    // red-zone/guardian value (stack) (0b01010101)
#define V_NOP           0xff    // no operation
/* logical operations */
#define V_NOT           0x01    // bitwise negation; 2's complement
#define V_AND           0x02    // bitwise AND
#define V_OR            0x03    // bitwise OR
#define V_XOR           0x04    // bitwise exclusive-OR
#define V_NAD           0x05    // NAND
#define V_SHL           0x10    // left shift
#define V_SHR           0x11    // logical right shift; fill with zero
#define V_SAR           0x12    // arithmetic right shift; fill with sign-bit
#define V_INC           0x20    // arg1++;
#define V_DEC           0x21    // arg1--;
#define V_ADD           0x22    // arg2 += arg1;
#define V_SUB           0x23    // arg2 -= arg1;
#define V_MUL           0x30    // arg2 *= arg1;
#define V_RPC           0x31    // compute 1/D so that X * 1/D replaces X/D
#define V_MOD           0x32    // arg2 %= arg1;
/* LOAD-STORE INSTRUCTIONS */
#define V_MOV           0x40    // move between registers or register and memory
#define V_PSH           0x41    // push item or items to stack
#define V_POP           0x42    // pop item or items from stack
#define V_POM           0x43    // pop register range
#define V_PSM           0x44    // push register range
/* BRANCH INSTRUCTIONS */
#define V_ULT           0x50    // unsigned arg1 < arg2
#define V_LT            0x51    // arg1 < arg2;
#define V_ULTE          0x52    // unsigned arg1 <= arg2
#define V_LTE           0x53    // arg1 <= arg2;
#define V_UGT           0x54    // unsigned arg1 > arg2
#define V_GT            0x55    // arg1 > arg2;
#define V_UGTE          0x56    // unsigned arg1 >= arg2
#define V_GTE           0x57    // arg1 >= arg2;
#define V_EQ            0x58    // arg1 == arg2;
#define V_NEQ           0x59    // arg1 != arg2;
/* MISCELLANEOUS INSTRUCTIONS */
#define V_SEL           0x60    // select
#define V_RAND          0x61    // arg2 = (c) ? arg1 : arg2;
/* FLOATING POINT INSTRUCTIONS */
#define V_FLOOR         0x70
#define V_CEIL          0x71
#define V_TRUNC         0x72
#define V_ITOB          0x73
#define V_ITOF          0x74
#define V_ITOD          0x75
#define V_BTOI          0x76
#define V_FTOI          0x77
#define V_DTOI          0x77
/* logarithms, roots and exponents */
#define V_LOG           0x80
#define V_SQRT          0x81
#define V_CBRT          0x82
#define V_EXP           0x83
/* trigonometry */
#define V_SIN           0x90
#define V_COS           0x91
#define V_TAN           0x92
#define V_ASIN          0x93
#define V_ACOS          0x94
#define V_ATAN          0x95
#define V_SINH          0x96
#define V_COSH          0x97
#define V_TANH          0x98

struct vcodeinst {
    uintptr_t   adr;
    uint8_t     code;
    uint8_t     flg;
    uint8_t     reg;
};

#endif /* __SYS_VCODE_INST_H__ */

