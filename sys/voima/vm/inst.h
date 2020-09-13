#ifndef __SYS_VOIMA_INST_H__
#define __SYS_VOIMA_INST_H__

#include <zero/cdefs.h>

/*
 * NOTES
 * -----
 * - NOP is a 32-bit instruction structure with all bits set to 1
 *
 * features shamelessly stolen from ARM
 * ------------------------------------
 * - predication; conditional execution
 * - folding of shifts and rotates
 *   a += j << 2;   ADD Ra, Ra, Rj, LSL #2
 *
 * - the assembler and compiler must ensure alignment of all instructions to 32-
 *   bit boundaries
 *
 * - for LDM and STM, op.src1 is the lowest, and op.src2 the highest register;
 *   dst is base address register
 *
 * - addressing modes
 *
 *   ldr lbl, %r        - lbl is address in imm32 (V0_PARM_ADR_OFS)
 *   ldr $lbl, %r       - address of lbl in imm32 (V0_PARM_ADR_OFS)
 *   ldl *%r1, %r2      - %r1 is base address (V0_PARM_ADR_BASE)
 *   ldl $x(%r1), %r2   - %r1 is the base address, $x offset in imm32
 *                        (V0_PARM_ADR_BASE | V0_PARM_ADR_OFS)
 *   ldl %r2(%r1), %r3  - %r1 is the base address, %r2 the index
 *                        (V0_PARM_ADR_BASE, V0_PARM_ADR_NDX)
 */

#define V0_OP_COPROC_BIT        (1 << 4)
#define V0_OP_MAX               0x0f
#define V0_VAL_MAX              0x3f
/* instsize = 32 + (parm & sizebits) * 32 */
#define V0_PARM_OP_SIZE_BITS    2
/* argsize = 8 << (parm & argbits) */
#define V0_PARM_ARG_SIZE_BITS   2
#define V0_PARM_ADR_BASE        (1 << 4)
#define V0_PARM_ADR_NDX         (1 << 5)
#define V0_PARM_ADR_OFS         (1 << 6)
#define V0_PARM_IMM_BIT         (1 << 7)
struct v0reginst {
    unsigned                    op      : 5;    // instruction type, coproc-flag
    unsigned                    src1    : 4;    // source register #1
    unsigned                    dst     : 4;    // destination register
    unsigned                    src2    : 4;    // source register #2
    unsigned                    val     : 6;    // shift count, bit #, ...
    unsigned                    flg     : 1;    // shift 1 = right
    int8_t                      parm;           // instruction ID, flags
};

struct v0imminst {
    unsigned                    op      : 5;    // instruction ID
    unsigned                    dst     : 4;    // destination register
    unsigned                    src     : 4;    // source register
    unsigned                    size    : 2;    // size = 32 + 32 * inst.size;
    unsigned                    aln     : 1;    // immediate aligned by 32 bits
    int16_t                     imm16;
    int32_t                     imm32[C_VLA];
};

struct v0jmpinst {
    unsigned                    op      : 5;
    unsigned                    ofs     : 27;   // toss low 2 + high 3 bits
};

/* for the imm8-member */
#define V0_UNSIGNED_BIT         (1 << 2)    // perform unsigned arithmetics
#define V0_MSW_BIT              (1 << 3)    // set relevant flags in the MSW
#define V0_SATURATE_BIT         (1 << 4)    // saturate to max/min value
#define V0_ATOMIC_BIT           (1 << 5)    // atomic [memory] operation
#define V0_IMM32_BIT            (1 << 6)    // 32-bit immediate follows opcode
#define V0_IMM64_BIT            (1 << 7)    // 64-bit immediate follows opcode

#define V0_SHIFT_BITS           6   // room for 64-bit
#define V0_SHIFT_RIGHT_BIT      (1 << 11)

#define V0_EQ_COND              0           // ZF
#define V0_NE_COND              1           // !ZF
#define V0_CF_COND              2
#define V0_UGTE_COND            2
#define V0_NC_COND              3
#define V0_ULT_COND             3
#define V0_SF_COND              4
#define V0_NS_COND              5
#define V0_OF_COND              6
#define V0_NO_COND              7
#define V0_UGT_COND             8
#define V0_ULE_COND             9
#define V0_GE_COND              10
#define V0_LT_COND              11
#define V0_GT_COND              12
#define V0_LE_COND              13
#define V0_DEF_COND             14

#define V0_INT_UNIT             0x0     // integer processor unit
#define V0_FPU_UNIT             0x1     // floating-point unit
#define V0_FXP_UNIT             0x2     // fixed-point unit
#define V0_DSP_UNIT             0x3     // digital signal processor
#define V0_SIMD_UNIT            0x4     // SIMD unit
#define V0_PHYS_UNIT            0x5     // physics unit
#define V0_GPU_UNIT             0x6     // graphics processor
#define V0_COPROC_UNIT          0x7     // auxiliary coprocessor

/*
 * logical instructions
 */
#define V0_LOGIC_OP             0x00    /* op-member */
/* parm-member */
#define V0_NOT_OP               0x00    // r2 = ~r1;
#define V0_OR_OP                0x01    // r2 |= r1;
#define V0_XOR_OP               0x02    // r2 ^= r1;
#define V0_AND_OP               0x03    // r2 &= r1;
#define V0_NOR_OP               0x04    // r2 ~|= r1;
#define V0_XNOR_OP              0x05    // r2 ^| r1;
#define V0_NAND_OP              0x06    // r2 ~& r1;

/*
 * shift and rotate instructions
 */
#define V0_SHIFT_OP             0x01    // op-member
/* parm-member */
#define V0_SHL_OP               0x00    // r3 = r1 << r2;
#define V0_SHR_OP               0x01    // r3 = r1 >> r2;
#define V0_SAR_OP               0x02    // r3 = r1 >>> r2; /* arithmetic */
#define V0_ROL_OP               0x03    // rotate left
#define V0_ROR_OP               0x04    // rotate right
#define V0_RCL_OP               0x05    // rotate left through carry
#define V0_RCR_OP               0x06    // rotate right through carry

/*
 * arithmetic instructions
 * - parm : UNS, ATOM/HIW (X), MSW
 * - parm: r3 for XINC, XDEC, XADD, XSUB, RDIV, and RUDIV
 */
#define V0_BYONE_OP             0x02    // op-member
/* parm-member */
#define V0_INC_OP               0x00    // r2 = ++r1;
#define V0_UINC_OP              0x01    // unsigned variant
#define V0_XINC_OP              0x02    // atomic variant, fetch result
#define V0_DEC_OP               0x03    // r2 = --r1;
#define V0_UDEC_OP              0x04    // unsigned variant
#define V0_XDEC_OP              0x05    // atomic variant, fetch result

#define V0_ADDER_OP             0x03    // op-member
/* parm-member */
#define V0_ADD_OP               0x00    // r3 = r2 + r1;
#define V0_UADD_OP              0x01    // unsigned variant
#define V0_ADC_OP               0x02    // sets CF in the MSW
#define V0_XADD_OP              0x03    // atomic variant, fetch result
#define V0_SUB_OP               0x04    // r3 = r2 - r1;
#define V0_USUB_OP              0x05    // unsigned varinat
#define V0_SBB_OP               0x06    // sets CF in the MSW
#define V0_RSB_OP               0x07    // r3 = r1 - r2;
#define V0_RSC_OP               0x08    // r3 = r1 - r2; setmsw(CF)
#define V0_XSUB_OP              0x09    // atomic variant, fetch result

#define V0_MULTI_OP             0x04    // op-member
/* parm-member */
#define V0_MUL_OP               0x00    // r3 = r2 * r1;
#define V0_UMUL_OP              0x01    // unsigned variant
#define V0_MHI_OP               0x02    // r3 = (r2 * r1) >> 32;
#define V0_UMHI_OP              0x03    // unsigned variant

#define V0_DIVIDE_OP            0x05    // op-member
/* parm-member */
#define V0_DIV_OP               0x00    // r3 = r2 / r1;
#define V0_UDIV_OP              0x01    // unsigned variant
#define V0_RDIV_OP              0x02    // r2 = r2 / r1; r3 = r2 % r1;
#define V0_URDIV_OP             0x03    // unsigned variant
#define V0_REM_OP               0x04    // r3 = r2 % r1;
#define V0_UREM_OP              0x05    // unsigned variant
#define V0_IRP_OP               0x06    // r2 = irp(r1) so x * r2 => x / r2
#define V0_UIRP_OP              0x07    // unsigned variant

/* sign- and zero-extension */
#define V0_EXT_OP               0x06    // op-member
/* flg-member */
#define V0_ZEX_OP               0x00    // zero-extend to register size
#define V0_SEX_OP               0x01    // sign-extend to register size

/* bit operations */
#define V0_BIT_OP               0x07    // op-member
/* parm-member */
#define V0_BST_OP               0x00    // set bit; r3 |= r2 << r1;
#define V0_BCL_OP               0x01    // clear bit; r3 &= ~(r2 << r1);
#define V0_BFL_OP               0x02    // flip bit; r3 ^= r2 << r1;
#define V0_BIC_OP               0x03    // bitwise clear; r3 = r2 & ~r1;

#define V0_SCAN_OP              0x08    // op-member
/* parm-member */
#define V0_BFS_OP               0x00    // bit find first [lowest] set
#define V0_BLS_OP               0x01    // bit find latest [highest] set
#define V0_PAR_OP               0x02    // bit parity, 0 for even
#define V0_HAM_OP               0x03    // hamming weight, bit population
#define V0_CTZ                  0x04    // count trailing [low] zero-bits
#define V0_CLZ_OP               0x05    // count leading [high] zero-bits
#define V0_HSH_OP               0x06    // compute hash function for value
#define V0_HUN_OP               0x07    // compute reverse hash function

/*
 * comparison operations
 */
#define V0_COMPARE_OP           0x09    // op-member
/* parm-member */
#define V0_CMP_OP               0x00    // comparison
#define V0_CMU_OP               0x01    // unsigned comparison
#define V0_TST_OP               0x02    // r3 = r2 & r1;
#define V0_TEQ_OP               0x03    // r3 = r2 ^ r1;

/* jump operations */

#define V0_JUMP_OP              0x0a    // op-member
/* parm-member */
#define V0_JMP_OP               0x00    // jump/branch unconditionally
#define V0_JMR_OP               0x01    // address in register
#define V0_JAL_OP               0x02    // branch and link
#define V0_JPC_OP               0x03    // PC-relative; position-independent

/* branch operation */

#define V0_BRANCH_OP            0x0b    // op-member
/* parm-member */
#define V0_BEQ_OP               V0_EQUAL_COND
#define V0_BNE_OP               V0_NOT_EQUAL_COND
#define V0_BCF_OP               V0_CARRY_SET_COND
#define V0_BNC_OP               V0_CARRY_CLEAR_COND
#define V0_BUGE_OP              V0_UNSIGNED_GTE_COND
#define V0_BULT_OP              V0_UNSIGNED_LT_COND
#define V0_BSF_OP               V0_NEGATIVE_COND
#define V0_BNS_OP               V0_POSITIVE_COND
#define V0_BOF_OP               V0_OVERFLOW_COND
#define V0_BNO_OP               V0_NO_OVERFLOW_COND
#define V0_BUGT_OP              V0_UNSIGNED_GT_COND
#define V0_BULE_OP              V0_UNSIGNED_LTE_COND
#define V0_BGE_OP               V0_SIGNED_GTE_COND
#define V0_BLT_OP               V0_SIGNED_LT_COND
#define V0_BGT_OP               V0_SIGNED_GT_COND
#define V0_BLE_OP               V0_SIGNED_LTE_COND
#define V0_BNB_OP               V0_NEVER_COND
#define V0_BNR_OP               (V0_NEVER_COND + 1)    // branch if no rounding

#define V0_SUBRT_OP             0x0c    // op-member
/* parm-member */
#define V0_SRT_OP               0x00    // call subroutine
#define V0_RET_OP               0x01    // return from subroutine
#define V0_SME_OP               0x02    // enter system mode
#define V0_SMX_OP               0x03    // exit system mode
#define V0_THR_OP               0x04    // launch thread
#define V0_THX_OP               0x05    // exit thread

/*
 * load and store operations
 */

#define V0_LDSTR_OP             0x0d    // op-member
/* parm-member */
#define V0_LEA_OP               0x00
#define V0_MVR_OP               0x01    // user-to-user register
#define V0_MVU_OP               0x02    // user-to-system register
#define V0_MVX_OP               0x03    // system-to-user register
#define V0_MVC_OP               0x04    // conditional user-to-user register
#define V0_LDB_OP               0x05    // load byte (8-bit)
#define V0_LDH_OP               0x06    // load halfword (16-bit)
#define V0_LDW_OP               0x07    // load word (32-bit)
#define V0_LDD_OP               0x08    // load doubleword (64-bit)
#define V0_LFS_OP               0x09    // load single-precision float (32-bit)
#define V0_LFD_OP               0x0a    // load double-precision float (64-bit)
#define V0_LFX_OP               0x0b    // load fixed-point number (64-bit)
#define V0_POP_OP               0x0c    // pop from stack to register
#define V0_LDM_OP               0x0d    // load many registers
#define V0_LDL_OP               0x0e    // load linked (soft-lock cacheline)
#define V0_CLD_OP               0x0f    // conditional load
#define V0_STB_OP               0x10    // store byte to memory
#define V0_STH_OP               0x11    // store halfword to memory
#define V0_STW_OP               0x12    // store word to memory
#define V0_STD_OP               0x13    // store doubleword to memory
#define V0_SFS_OP               0x14    // store single-precision float
#define V0_SFD_OP               0x15    // store double-precision float
#define V0_SFX_OP               0x16    // store fixed-point number
#define V0_PSH_OP               0x17    // push register to memory
#define V0_STM_OP               0x18    // store many registers
#define V0_STL_OP               0x19    // store linked
#define V0_CST_OP               0x1a    // conditional store

#define V0_ATOMIC_OP            0x0e    // op-member
/* parm-member */
#define V0_BTS_OP               0x00    // bit test-and-set
#define V0_BTC_OP               0x01    // bit test-and-clear
#define V0_BTF_OP               0x02    // bit test-and-flip
#define V0_CAS_OP               0x03    // [atomic] compare and swap
#define V0_STM_OP               0x04    // start transactional memory [page]
#define V0_FTM_OP               0x05    // end start transactional memory [page]

#define V0_SYSTEM_OP            0x0f    // op-member
/* parm-member */
#define V0_HLT_OP               0x00
#define V0_RST_OP               0x01
#define V0_BRK_OP               0x02    // execution breakpoints
#define V0_INT_OP               0x03
#define V0_NMI_OP               0x04    // non-maskable interrupt
#define V0_CLI_OP               0x05
#define V0_STI_OP               0x06
#define V0_CSP_OP               0x07    // set [global] cache-policy
#define V0_CPF_OP               0x08    // cache prefetch; level argument
#define V0_FCL_OP               0x09    // flush cachelines; level argument
#define V0_FPG_OP               0x0a    // invalidate/flush page TLB-entry
#define V0_MTR_OP               0x0b    // control memory-type ranges
#define V0_WFE_OP               0x0c    // wait for memory (cacheline?) event
#define V0_SEV_OP               0x0d    // signal a memory event
#define V0_RBR_OP               0x0e    // memory read barrier
#define V0_WBR_OP               0x0f    // memory write barrier
#define V0_FBR_OP               0x10    // full memory barrier
#define V0_IOP_OP               0x11    // allow or deny I/O on port
#define V0_IOR_OP               0x12    // I/O-port read operation
#define V0_IOW_OP               0x13    // I/O-port write operation
#define V0_IOC_OP               0x14    // I/O-control operation
#define V0_REG_OP               0x15    // shared-register operation e.g. time
#define V0_IPI_OP               0x16    // Inter-Processor Interrupt
#define V0_BUS_OP               0x17    // bus operation
#define v0_DEV_OP               0x18    // peripheral device operation
#define V0_MPC_OP               0x19    // [multi]processor control operation

#endif /* __SYS_VOIMA_INST_H__ */

