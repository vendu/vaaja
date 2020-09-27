#ifndef __MACH_V0_REGS_H__
#define __MACH_V0_REGS_H__

/* register file */

/* general-purpose integer registers */

#define V0_R0_REG               0   // zero-register
#define V0_R1_REG               1   // function argument/return value #1
#define V0_R2_REG               2   // function argument/return value #2
#define V0_R3_REG               3   // function argument #3
#define V0_R4_REG               4   // function argument #4
#define V0_R5_REG               5   // function scratch-register #1
#define V0_R6_REG               6   // function scratch-register #2
#define V0_R7_REG               7   // function scratch-register #3
#define V0_R8_REG               8   // function scratch-register #4
#define V0_R9_REG               9   // function scratch-register #5
#define V0_R10_REG              10  // function scratch-register #6
#define V0_R11_REG              11  // function scratch-register #7
#define V0_SP_REG               V0_R12_REG   // stack pointer
#define V0_LR_REG               V0_R13_REG   // link register
#define V0_FP_REG               V0_R14_REG   // stack-frame pointer
#define V0_PC_REG               V0_R15_REG   // program counter
#define V0_R12_REG              12
#define V0_R13_REG              13
#define V0_R14_REG              14
#define V0_R15_REG              15
#define V0_INTEGER_REGISTERS    16  // # of user registers
#define V0_CALLER_REGISTERS     4   // r1..r4
#define V0_CALLEE_REGISTERS     9   // r1..r13

/* integer-mode system registers */

/*
 * NOTES
 * -----
 * - pick power-of-2 value for TMR 'divisor'; e.g. one tick per every 256 clocks
 *   would use TMR = 255
 * - TMS, TMR   -> if !(TMS & TMR) tick();
 */
#define V0_MSW_REG              0   // machine status word
#define V0_TCB_REG              1   // TCB address + TCB_SYS-bit [0]
#define V0_THR_REG              2   // thread ID in low 16 bits + flags
#define V0_TWC_REG              3   // thread wait-channel
#define V0_TST_REG              4   // thread status register
#define V0_BLO_REG              5   // boundary-check low limit
#define V0_BHI_REG              6   // boundary-check high limit
#define V0_IVR_REG              7   // interrupt vector base address
#define V0_IMR_REG              8   // interrupt mask register
#define V0_PDR_REG              9   // page-directory base address
#define V0_SDR_REG              10  // segment-directory base address
#define V0_FSR_REG              11  // floating-point status register
#define V0_DSR_REG              12  // digital signal processor status
// if ((TMR & (TMS - 1)) == 0) { tick(); }
#define V0_TMR_REG              13  // tick/interrupt counter
#define V0_TMS_REG              14  // [32-bit] time-stamp register
#define V0_MFR_REG              15  // machine feature flags // MT, PF, IOP
#define V0_SYSTEM_REGISTERS     16  // # of system registers

/* hidden, CPU-internal registers */
#define V0_MSW0_REG             0   // system-mode machine status word
#define V0_FP0_REG              1   // system-mode interrupt stack pointer
#define V0_SP0_REG              2   // system-mode stack pointer
#define V0_STK_REG              3   // system-mode stack-limit/bottom
#define V0_IV0_REG              4   // system interrupt-vector base address
#define V0_HIDDEN_REGISTERS     5

/* MSW-bits; machine status word */
#define V0_MSW_ZF_BIT           (1 << 0)    // zero/equal-bit
#define V0_MSW_LT_BIT           (1 << 1)    // less-than-bit
#define V0_MSW_CF_BIT           (1 << 2)    // carry/borrow-bit
#define V0_MSW_OF_BIT           (1 << 3)    // overflow
#define V0_MSW_SF_BIT           (1 << 4)    // sign-flag
#define V0_MSW_TF_BIT           (1 << 5)    // trap-flag (single-step)
#define V0_MSW_IF_BIT           (1 << 6)    // interrupt enable flag
#define V0_MSW_CD_BIT           (1 << 7)    // cache disable flag
#define V0_MSW_WT_BIT           (1 << 8)    // cache write-through policy
#define V0_MSW_COND_MASK        (V0_MSW_ZF_BIT                      \
                                 | V0_MSW_LT_BIT                    \
                                 | V0_MSW_CF_BIT                    \
                                 | V0_MSW_OF_BIT)
#define V0_MSW_BC_BIT           (1 << 28)   // check array access boundaries
#define V0_MSW_RF_BIT           (1 << 31)   // ring-bit, set in system mode
#define V0_MSW_IV_BIT           (1 << 29)   // interrupt-vector present
#define V0_MSW_MT_BIT           (1 << 30)   // multithread; honor bus-locks
#define V0_MSW_PF_BIT           (1 << 31)   // paging active

/* floating-point registers [64-bit, 80-bit internal] */

#define V0_F0_REG               0
#define V0_F1_REG               1
#define V0_F2_REG               2
#define V0_F3_REG               3
#define V0_F4_REG               4
#define V0_F5_REG               5
#define V0_F6_REG               6
#define V0_F7_REG               7
#define V0_F8_REG               8
#define V0_F9_REG               9
#define V0_F10_REG              10
#define V0_F11_REG              11
#define V0_F12_REG              12
#define V0_F13_REG              13
#define V0_F14_REG              14
#define V0_F15_REG              15
#define V0_FPU_REGISTERS        16

/* FSR; floating-point status register */
#define V0_FSR_INV_BIT          (1 << 0)    // invalid operand
#define V0_FSR_DVZ_BIT          (1 << 1)    // division by zero
#define V0_FSR_OFL_BIT          (1 << 2)    // overflow
#define V0_FSR_UFL_BIT          (1 << 3)    // overflow
#define V0_FSR_LT_BIT           (1 << 4)    // less than
#define V0_FSR_EQ_BIT           (1 << 5)    // equal
#define V0_FSR_GE_BIT           (1 << 6)    // greater than or equal
#define V0_FSR_UNO_BIT          (1 << 7)    // unordered
#define V0_FSR_PINF_BIT         (1 << 8)    // positive infinity result
#define V0_FSR_NINF_BIT         (1 << 9)    // negative infinity result
#define V0_FSR_NAN_BIT          (1 << 10)   // not-a-number (NaN)
#define V0_FSR_SNAN_BIT         (1 << 11)   // signaling NaN
#define V0_FSR_RND_BIT          (1 << 28)   // round to nearest
#define V0_FSR_RTZ_BIT          (1 << 29)   // round towards zero
#define V0_FSR_RPI_BIT          (1 << 30)   // towards positive infinity
#define V0_FSR_RNI_BIT          (1 << 31)   // towards negative infinity

/* digital signal registers [32-bit floating-point, 64-bit internal] */

#define V0_DS0_REG              0
#define V0_DS1_REG              1
#define V0_DS2_REG              2
#define V0_DS3_REG              3
#define V0_DS4_REG              4
#define V0_DS5_REG              5
#define V0_DS6_REG              6
#define V0_DS7_REG              7
#define V0_DS8_REG              8
#define V0_DS9_REG              9
#define V0_DS10_REG             10
#define V0_DS11_REG             11
#define V0_DS12_REG             12
#define V0_DS13_REG             13
#define V0_DS14_REG             14
#define V0_DS15_REG             15
#define V0_DSP_REGISTERS        16

/* DSR; digital signal status register */

/* fixed-point registers [32-bit with 64-bit internal resolution] */

#define V0_FX0_REG              0
#define V0_FX1_REG              1
#define V0_FX2_REG              2
#define V0_FX3_REG              3
#define V0_FX4_REG              4
#define V0_FX5_REG              5
#define V0_FX6_REG              6
#define V0_FX7_REG              7
#define V0_FX8_REG              8
#define V0_FX9_REG              9
#define V0_FX10_REG             10
#define V0_FX11_REG             11
#define V0_FX12_REG             12
#define V0_FX13_REG             13
#define V0_FX14_REG             14
#define V0_FX15_REG             15
#define V0_FXP_REGISTERS        16

/* FXSR; fixed-point status register */

/* process segment registers; 64-bit, limit + page-address + flags */
#define V0_CODE_REG             0
#define V0_RODATA_REG           1
#define V0_DATA_REG             2
#define V0_HEAP_REG             3
#define V0_STACK_REG            4
#define V0_MAP_REG              5
#define V0_TLS_REG              6
#define V0_DEBUG_REG            7
#define V0_SEG0_REG             8
#define V0_SEG1_REG             9
#define V0_SEG2_REG             10
#define V0_SEG3_REG             11
#define V0_SEG4_REG             12
#define V0_SEG5_REG             13
#define V0_SEG6_REG             14
#define V0_SEG7_REG             15
#define V0_SEGMENT_REGISTERS    16

#define V0_SEG_EXEC_BIT         (1 << 0)    // executable [code] segment
#define V0_SEG_WRITE_BIT        (1 << 1)    // writable [data] segment
#define V0_SEG_READ_BIT         (1 << 2)    // readable segment
#define V0_SEG_SYS_BIT          (1 << 3)    // system segment, ring #0 only
#define V0_SEG_DEF_BIT          (1 << 4)    // segment defined
#define V0_SEG_DIR_BIT          (1 << 5)    // grow-down (stack segment)

#endif /* __MACH_V0_REGS_H__ */

