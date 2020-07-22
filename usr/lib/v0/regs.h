#ifndef __V0_VM_REGS_H__
#define __V0_VM_REGS_H__

#include <mach/param.h>

/* MACHINE PARAMETERS */

/* integer sizes */
#define V0_BYTE_SIZE            1
#define V0_HALF_SIZE            2
#define V0_WORD_SIZE            4
#define V0_LONG_SIZE            8

/* floating-point sizes */
#define V0_FLOAT_SIZE           4
#define V0_DOUBLE_SIZE          8

/* GENERAL-registers; internally 64-bit(?) with the low 32 bits visible */
#define V0_R0_REGISTER          0       // zero-register; always returns zero
#define V0_R1_REGISTER          1       // function argument/return register #1
#define V0_R2_REGISTER          2       // function argument register #2
#define V0_R3_REGISTER          3       // function argument register #3
#define V0_R4_REGISTER          4       // function argument register #4
#define V0_R5_REGISTER          5       // function argument register #5
#define V0_R6_REGISTER          6       // scratch register #1
#define V0_R7_REGISTER          7       // scratch register #2
#define V0_R8_REGISTER          8       // scratch register #3
#define V0_R9_REGISTER          9       // scratch register #4
#define V0_R10_REGISTER         10      // scratch register #5
#define V0_TL_REGISTER          11      // thread-local segment base address
#define V0_FP_REGISTER          12      // frame pointer
#define V0_SP_REGISTER          13      // stack pointer
#define V0_LR_REGISTER          14      // link register (caller return address)
#define V0_PC_REGISTER          15      // program counter (instruction pointer)
#define V0_GENERAL_REGISTERS    16      // total # of general registers
/* MSW-register flag-bits */
#define V0_MSW_CF_BIT			(1 << 0)    // carry/borrow-bit
#define V0_MSW_OF_BIT			(1 << 1)    // overflow
#define V0_MSW_ZF_BIT			(1 << 2)    // zero-bit
#define V0_MSW_LT_BIT			(1 << 3)    // less than
#define V0_MSW_SF_BIT           (1 << 4)    // sign-bit
#define V0_MSW_TF_BIT           (1 << 5)    // trap-flag/single-step
#define V0_MSW_IF_BIT           (1 << 6)    // interrupt enable
#define V0_MSW_FR_BIT           (1 << 7)    // round-up (fraction MSB set)

/* SYSTEM-registers */
/* read-only registers in user-mode; 32-bit wide */
/* TCBs must be 256-byte aligned */
#define V0_MSW_REGISTER         0       // machine status word
#define V0_MFR_REGISTER         1       // machine feature register
#define V0_MCR_REGISTER         2       // machine control register
#define V0_TMS_REGISTER         3       // timestamp register
#define V0_DBG_REGISTER         3       // debug-routine base address
#define V0_BLO_REGISTER         4       // bound-check low limit
#define V0_BHI_REGISTER         5       // bound-check high limit
#define V0_THR_REGISTER         6       // TCB-address + thread ID (low byte)
#define V0_TWC_REGISTER         7       // wait-channel for current thread
#define V0_IVB_REGISTER         8       // interrupt-vector base address
#define V0_IMR_REGISTER         9       // interrupt mask register
#define V0_PDB_REGISTER         10      // page-directory base address
#define V0_IOP_REGISTER         11      // IO-port permission bitmap address
#define V0_SP0_REGISTER         12      // system-mode (ring 0) stack pointer
#define V0_MS0_REGISTER         13      // system-mode machine status word
#define V0_TEX_REGISTER         14      // thread exit-status address
#define V0_SEG_REGISTER         15      // segment-table address + # of segments
#define V0_SYSTEM_REGISTERS     16      // maximum/total # of system registers

#define V0_FIXED_REGISTERS      16      // total # of fixed-point registers
#define V0_FLOAT_REGISTERS      16      // total # of floating-point registers

/* [private] segment registers; 64-bit, limit + page-address + flags */
#define V0_CS_REGISTER          0x00    // code-segment
#define V0_DS_REGISTER          0x01    // [initialized] data-segment
#define V0_ES_REGISTER          0x02    // [initialized] read-only data-segment
#define V0_SS_REGISTER          0x03    // stack-segment
#define V0_TS_REGISTER          0x04    // thread-local data-segment
#define V0_XS0_REGISTER         0x05
#define V0_XS1_REGISTER         0x06
#define V0_XS2_REGISTER         0x07
#define V0_SEGMENT_REGISTERS    8

#define V0_SEG_EXEC_BIT         (1 << 0)    // executable [code] segment
#define V0_SEG_WRITE_BIT        (1 << 1)    // writable [data] segment
#define V0_SEG_READ_BIT         (1 << 2)    // readable segment
#define V0_SEG_USER_BIT         (1 << 3)    // software use (not hardware)
#define V0_SEG_DEF_BIT          (1 << 4)    // segment descriptor defined
#define V0_SEG_SYS_BIT          (1 << 5)    // ring #0 access only
#define V0_SEG_DIR_BIT          (1 << 6)    // expansion-direction (stack)

/* per-processor performance-counter registers; 64-bit */
#define V0_TLB_MISS_REGISTER    0x00
#define V0_L1_MISS_REGISTER     0x01
#define V0_L2_MISS_REGISTER     0x02
#define V0_INST_MISS_REGISTER   0x03

#endif /*  __V0_VM_REGS_H__ */

