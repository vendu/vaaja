#ifndef __VOIMA_UNIT_H__
#define __VOIMA_UNIT_H__

#define V0_INS_BITS                 5
#define V0_UNIT_BITS                3
#define V0_INS_MASK                 0x1f

/* register file */

/* general-purpose integer registers */

#define VOIMA_R0_REG                0   // zero-register
#define VOIMA_R1_REG                1   // function argument/return value #1
#define VOIMA_R2_REG                2   // function argument/return value #2
#define VOIMA_R3_REG                3   // function argument #3
#define VOIMA_R4_REG                4   // function argument #4
#define VOIMA_R5_REG                5   // function scratch-register #1
#define VOIMA_R6_REG                6   // function scratch-register #2
#define VOIMA_R7_REG                7   // function scratch-register #3
#define VOIMA_R8_REG                8   // function scratch-register #4
#define VOIMA_R9_REG                9   // function scratch-register #5
#define VOIMA_R10_REG               10  // function scratch-register #6
#define VOIMA_R11_REG               11  // function scratch-register #7
#define VOIMA_FP_REG                VOIMA_R12_REG   // stack-frame pointer
#define VOIMA_SP_REG                VOIMA_R13_REG
#define VOIMA_LR_REG                VOIMA_R14_REG
#define VOIMA_PC_REG                VOIMA_R15_REG
#define VOIMA_R12_REG               12
#define VOIMA_R13_REG               13
#define VOIMA_R14_REG               14
#define VOIMA_R15_REG               15
#define VOIMA_INTEGER_REGISTERS     16

/* integer-mode system registers */

/*
 * NOTES
 * -----
 * - pick power-of-2 value for TMR 'divisor'; e.g. one tick per every 256 clocks
 *   would use TMR = 255
 * - TMS, TMR   -> if !(TMS & TMR) tick();
 */

#define VOIMA_MSW_REG               0
// if ((TMR & (TMS - 1)) == TMS - 1) { tick(); }
#define VOIMA_THR_REG               1   // TCB-table page address + thread-ID
#define VOIMA_TLS_REG               2   // thread-local storage page base + count
#define VOIMA_TWC_REG               3   // thread wait-channel
#define VOIMA_TLR_REG               4   // thread return-value pointer
#define VOIMA_BLO_REG               5   // boundary-check low limit
#define VOIMA_BHI_REG               6   // boundary-check high limit
#define VOIMA_IVR_REG               7   // interrupt vector base address
#define VOIMA_IMR_REG               8   // interrupt mask register
#define VOIMA_PDR_REG               9   // page-directory base address
#define VOIMA_SDR_REG               10  // segment-directory base address
#define VOIMA_FSR_REG               11  // floating-point status register
#define VOIMA_DSR_REG               12  // digital signal processor status
#define VOIMA_TMR_REG               13  // tick/interrupt counter
#define VOIMA_TMS_REG               14  // [32-bit] time-stamp register
#define VOIMA_MFR_REG               15  // machine feature flags // MT, PF, IOP
#define VOIMA_SYSTEM_REGISTERS      16  // # of system registers

/* hidden, CPU-internal registers */
#define VOIMA_MSW0_REG              0   // system-mode machine status word
#define VOIMA_FP0_REG               1   // system-mode interrupt stack pointer
#define VOIMA_SP0_REG               2   // system-mode stack pointer
#define VOIMA_STK_REG               3   // system-mode stack-limit/top
#define VOIMA_SIV_REG               4   // system interrupt-vector base address
#define VOIMA_HIDDEN_REGISTERS      5

/* MSW-bits; machine status word */
#define VOIMA_MSW_ZF                (1 << 0)    // zero/equal-bit
#define VOIMA_MSW_LT                (1 << 1)    // less-than-bit
#define VOIMA_MSW_CF                (1 << 2)    // carry/borrow-bit
#define VOIMA_MSW_OF                (1 << 3)    // overflow
#define VOIMA_MSW_SF                (1 << 4)    // sign-flag
#define VOIMA_MSW_TF                (1 << 5)    // trap-flag (single-step)
#define VOIMA_MSW_IF                (1 << 6)    // interrupt enable flag
#define VOIMA_MSW_CMOVE_MASK        (VOIMA_MSW_ZF                       \
                                     | VOIMA_MSW_LT                     \
                                     | VOIMA_MSW_CF                     \
                                     | VOIMA_MSW_OF)
#define VOIMA_MSW_BC                (1 << 28)   // check array access boundaries
#define VOIMA_MSW_RF                (1 << 31)   // ring-bit, set in system mode
#define VOIMA_MSW_IV                (1 << 29)   // interrupt-vector present
#define VOIMA_MSW_MT                (1 << 30)   // multithread; honor bus-locks
#define VOIMA_MSW_PF                (1 << 31)   // paging active

/* floating-point registers */

#define VOIMA_F0_REG                0
#define VOIMA_F1_REG                1
#define VOIMA_F2_REG                2
#define VOIMA_F3_REG                3
#define VOIMA_F4_REG                4
#define VOIMA_F5_REG                5
#define VOIMA_F6_REG                6
#define VOIMA_F7_REG                7
#define VOIMA_F8_REG                8
#define VOIMA_F9_REG                9
#define VOIMA_F10_REG               10
#define VOIMA_F11_REG               11
#define VOIMA_F12_REG               12
#define VOIMA_F13_REG               13
#define VOIMA_F14_REG               14
#define VOIMA_F15_REG               15
#define VOIMA_FPU_REGISTERS         16

/* FSR; floating-point status register */
#define VOIMA_FSR_INV   (1 << 0)    // invalid operand
#define VOIMA_FSR_DVZ   (1 << 1)    // division by zero
#define VOIMA_FSR_OFL   (1 << 2)    // overflow
#define VOIMA_FSR_LT    (1 << 3)    // less than
#define VOIMA_FSR_EQ    (1 << 4)    // equal
#define VOIMA_FSR_GE    (1 << 5)    // greater than or equal
#define VOIMA_FSR_UNO   (1 << 6)    // unordered
#define VOIMA_FSR_PINF  (1 << 7)    // positive infinity result
#define VOIMA_FSR_NINF  (1 << 8)    // negative infinity result
#define VOIMA_FSR_NAN   (1 << 9)    // not-a-number (NaN)
#define VOIMA_FSR_SNAN  (1 << 10)   // signaling NaN
#define VOIMA_FSR_RND   (1 << 28)   // round to nearest
#define VOIMA_FSR_RNZ   (1 << 29)   // round towards zero
#define VOIMA_FSR_RPI   (1 << 30)   // round towards positive infinity
#define VOIMA_FSR_RNI   (1 << 31)   // round towards negative infinity

#endif /* __VOIMA_UNIT_H__ */

