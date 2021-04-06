#ifndef V0_REGS_H
#define V0_REGS_H

#include <mach/param.h>

/* MACHINE PARAMETERS */

/* integer sizes */
#define V0_BYTE_SIZE                1
#define V0_HALF_SIZE                2
#define V0_WORD_SIZE                4
#define V0_LONG_SIZE                8

/* floating-point sizes */
#define V0_FLOAT_SIZE               4
#define V0_DOUBLE_SIZE              8

/* GENERAL-registers; internally 64-bit(?) with the low 32 bits visible */
#define V0_R0_REGISTER              0   // zero-register; always returns zero
#define V0_R1_REGISTER              1   // function argument/return register #1
#define V0_R2_REGISTER              2   // function argument/return register #2
#define V0_R3_REGISTER              3   // function argument register #3
#define V0_R4_REGISTER              4   // function argument register #4
#define V0_CALLEE_SAVE_REGISTERS    4   // R1..R4
#define V0_R5_REGISTER              5   // scratch register #1
#define V0_R6_REGISTER              6   // scratch register #2
#define V0_R7_REGISTER              7   // scratch register #3
#define V0_R8_REGISTER              8   // scratch register #4
#define V0_R9_REGISTER              9   // scratch register #5
#define V0_R10_REGISTER             10  // scratch register #6
#define V0_R11_REGISTER             11  // scratch register #7 (syscall number)
#define V0_CALLER_SAVE_REGISTERS    7   // R5..R11
#define V0_LR_REGISTER              12  // link register (not preserved, JAL)
#define V0_PC_REGISTER              13  // program counter (instruction pointer)
#define V0_FP_REGISTER              14  // frame pointer (high word: low limit)
#define V0_SP_REGISTER              15  // stack pointer
#define V0_INTEGER_REGISTERS        16  // total # of general registers
#define V0_GEN_REG_BITS             4   // integer/general-purpose register ID
#define V0_REG_BITS                 5   // integer or system register ID

/* MSW-register flag-bits */
#define V0_MSW_CF_BIT               (1 << 0)    // carry/borrow-bit, less-than
#define V0_MSW_ZF_BIT               (1 << 1)    // zero-bit, equal
#define V0_MSW_SF_BIT               (1 << 2)    // sign-bit, negative
#define V0_MSW_OF_BIT               (1 << 3)    // overflow
#define V0_MSW_TF_BIT               (1 << 4)    // trap-flag/single-step
#define V0_MSW_IF_BIT               (1 << 5)    // interrupt enable
#define V0_MSW_BC_BIT               (1 << 27)   // check array access boundaries
#define V0_MSW_RF_BIT               (1 << 28)   // ring-bit, set in system mode
#define V0_MSW_IV_BIT               (1 << 29)   // interrupt-vector present
#define V0_MSW_MT_BIT               (1 << 30)   // multithread; honor bus-locks
#define V0_MSW_PF_BIT               (1 << 31)   // paging active

/* SYSTEM-registers */
/* read-only registers in user-mode; 32-bit wide */
/* TCBs must be 256-byte aligned */
/* user-writable registers */
#define V0_MSW_REGISTER             0   // machine status word
#define V0_DBG_REGISTER             1   // debug-routine base address
#define V0_BLO_REGISTER             2   // bound-check low limit
#define V0_BHI_REGISTER             3   // bound-check high limit
#define V0_THR_REGISTER             4   // TCB-address + thread ID (low byte)
#define V0_TWC_REGISTER             5   // wait-channel for current thread
/* system-writable registers */
#define V0_IVB_REGISTER             6   // interrupt-vector base address
#define V0_IMR_REGISTER             7   // interrupt mask register
#define V0_PDB_REGISTER             8   // page-directory base address
#define V0_SEG_REGISTER             9   // segment-table address + # of segments
#define V0_IOP_REGISTER             10  // IO-port permission bitmap address
#define V0_SP0_REGISTER             11  // system-mode (ring 0) stack pointer
#define V0_MSW0_REGISTER            12  // system-mode machine status word
/* read-only registers */
#define V0_TMS_REGISTER             15  // timestamp register
#define V0_MFR_REGISTER             16  // machine feature register
#define V0_SYSTEM_REGISTERS         16  // maximum/total # of system registers

/* per-processor performance-counter registers; 64-bit */
#define V0_INST_MISS_REGISTER       0x00
#define V0_PAGE_FAULT_REGISTER      0x01
#define V0_TLB_MISS_REGISTER        0x02
#define V0_L1_MISS_REGISTER         0x03
#define V0_L2_MISS_REGISTER         0x04
#define V0_L3_MISS_REGISTER         0x05

/* FSR; floating-point status register */
#define V0_FSR_RND_BIT              (1 << 0)    // round to nearest
#define V0_FSR_RTZ_BIT              (1 << 1)    // round towards zero
#define V0_FSR_RPI_BIT              (1 << 2)    // towards positive infinity
#define V0_FSR_RNI_BIT              (1 << 3)    // towards negative infinity
#define V0_FSR_INV_BIT              (1 << 4)    // invalid operand
#define V0_FSR_DVZ_BIT              (1 << 5)    // division by zero
#define V0_FSR_OFL_BIT              (1 << 6)    // overflow
#define V0_FSR_UFL_BIT              (1 << 7)    // underflow
#define V0_FSR_LT_BIT               (1 << 8)    // less than
#define V0_FSR_EQ_BIT               (1 << 9)    // equal
#define V0_FSR_GE_BIT               (1 << 10)   // greater than or equal
#define V0_FSR_UNO_BIT              (1 << 11)   // unordered
#define V0_FSR_PINF_BIT             (1 << 12)   // positive infinity result
#define V0_FSR_NINF_BIT             (1 << 13)   // negative infinity result
#define V0_FSR_NAN_BIT              (1 << 14)   // not-a-number (NaN)
#define V0_FSR_SNAN_BIT             (1 << 15)   // signaling NaN

#endif /*  V0_REGS_H */

