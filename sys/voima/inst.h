#ifndef __MACH_V0_INST_H__
#define __MACH_V0_INST_H__

#include <zero/cdefs.h>

/*
 * NOTES
 * -----
 * - NOP is a 32-bit instruction structure with all bits set to 1
 *
 * - unit ID 0xf is reserved for coprocessor use
 * - the standard units use <= 32 instructions per unit, but there's 256 op IDs
 *   reserved per unit
 *
 * features shamelessly stolen from ARM
 * ------------------------------------
 * - predication; conditional execution
 * - folding of shifts and rotates
 *   a += j << 2;   ADD Ra, Rj, LSL #2
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

#define V0_UNITS                16
#define V0_UNIT_INSTRUCTIONS    64
#define V0_OP_COPROC_UNIT       0x0f
#define V0_OP_MAX               0x0f
#define V0_VAL_MAX              0x3f

/* for conditional execution, cond-member is set and the conditional in src2 */
#define V0_ANY_COND             0x00            // execute always
#define V0_EQ_COND              0x01            // ZF
#define V0_NE_COND              0x02            // !ZF
#define V0_GT_COND              0x03            // !ZF && (SF == OF)
#define V0_LT_COND              0x04            // (SF != OF)
#define V0_GE_COND              0x05            // (SF == OF)
#define V0_LE_COND              0x06            // ZF || (SF != OF)
#define V0_HS_COND              0x07            // CF
#define V0_LO_COND              0x08            // !CF
#define V0_MI_COND              0x09            // SF
#define V0_PL_COND              0x0a            // !SF
#define V0_OF_COND              0x0b            // OF
#define V0_NO_COND              0x0c            // !OF
#define V0_HI_COND              0x0d            // CF || !ZF
#define V0_LS_COND              0x0e            // !CF || ZF
#define V0_NEVER_COND           0x0f            // never execute
#define V0_PARM_COND_BITS       4               // see above
#define V0_PARM_COND_MASK       0x0f

#define V0_PARM_SHIFT_BITS      6
#define V0_PARM_SHIFT_MASK      0x1f
#define V0_PARM_FOLD_SLL        0x00            // fold left shift
#define V0_PARM_FOLD_SRL        0x40            // fold logical right shift
#define V0_PARM_FOLD_SAR        0x80            // fold arithmetic right shift
#define V0_PARM_FOLD_ROR        0xc0            // fold rotate right
#define V0_PARM_FOLD_MASK       0xc0
#define V0_PARM_IMM_SIZE_BITS   2
#define V0_PARM_IMM_SIZE_MASK   0x03            // 1-, 2-, 4-, or 8-bit operand
#define V0_PARM_IMM_ALN         (1 << 3)        // aligned 64-bit immediate
#define V0_PARM_ADR_MASK        0xe0            // addressing-mode bits
#define V0_PARM_SRC_ADR         (1 << 4)        // source address (else target)
#define V0_PARM_ADR_BASE        (1 << 5)        // base register in use
#define V0_PARM_ADR_NDX         (1 << 6)        // index register in use
#define V0_PARM_IMM             (1 << 7)        // immediate operand present
#define V0_PARM_ADR_OFS         V0_PARM_IMM     // immediate address offset
struct v0inst {
    int8_t                      parm;           // shift-count, size, flags
    unsigned                    unit    : 4;    // instruction type, coproc-flag
    unsigned                    src     : 4;    // source register #1
    unsigned                    dest    : 4;    // destination register
    unsigned                    data    : 4;    // condition or 2nd register
    unsigned                    op      : 6;    // instruction ID
    unsigned                    pred    : 1;    // conditional in src2
    unsigned                    trap    : 1;    // instruction breakpoint
    int32_t                     imm32[C_VLA];   // optional immediate operand
};

#if 0
/* for the imm8-member */
#define V0_UNSIGNED_BIT         (1 << 2)    // perform unsigned arithmetics
#define V0_MSW_BIT              (1 << 3)    // set relevant flags in the MSW
#define V0_ATOMIC_BIT           (1 << 4)    // atomic [memory] operation
#define V0_SATURATE_BIT         (1 << 5)    // saturate to max/min value
#define V0_IMM32_BIT            (1 << 6)    // 32-bit immediate follows opcode
#define V0_IMM64_BIT            (1 << 7)    // 64-bit immediate follows opcode
#endif

#if 0
#define V0_INT_UNIT             0x0     // integer processor unit
#define V0_FPU_UNIT             0x1     // floating-point unit
#define V0_FXP_UNIT             0x2     // fixed-point unit
#define V0_DSP_UNIT             0x3     // digital signal processor
#define V0_SIMD_UNIT            0x4     // SIMD unit
#define V0_PHYS_UNIT            0x5     // physics unit
#define V0_GPU_UNIT             0x6     // graphics processor
#define V0_COPROC_UNIT          0x7     // auxiliary coprocessor
#endif

/*
 * logical instructions
 * - register-only operands
 * - conditional execution, condition code in src2
 * - instruction folding, shift-count and operation in parm
 */
#define V0_LOGIC_UNIT           0x00    /* op-member */
/* instruction ID */
#define V0_NOT_OP               0x00    // rs2 = ~rs1;
#define V0_ORR_OP               0x01    // rd = rs1 |= rs2;
#define V0_XOR_OP               0x02    // rd = rs1 ^= rs2;
#define V0_AND_OP               0x03    // rd = rs1 &= rs2;
#define V0_NOR_OP               0x04    // rd = rs1 ~|= rs2;
#define V0_XNOR_OP              0x05    // rd = rs1 ^| rs2;
#define V0_NAND_OP              0x06    // rd = rs1 ~& rs2;

/*
 * shift and rotate instructions
 * - register-only operands
 * - conditional execution, condition code in src2
 */
#define V0_SHIFT_UNIT           0x01
/* instruction ID */
#define V0_SLR_OP               0x00    // rd = rs1 >> r2;
#define V0_SAR_OP               0x01    // rd = rs1 >>> r2; /* arithmetic */
#define V0_SLL_OP               0x02    // rd = rs1 << r2;
#define V0_ROR_OP               0x03    // rd = ror(rs1, r2);
#define V0_ROL_OP               0x04    // rd = rol(rs1, r2);
#define V0_RCR_OP               0x05    // rd = rcr(rs1, r2);
#define V0_RCL_OP               0x06    // rd = rcl(rs1, r2);

/*
 * add/sub instructions
 * - register-only operands
 * - conditional execution, condition code in src2
 * - instruction folding, shift-count and operation in parm
 */
#define V0_ADDER_UNIT           0x02
/* instruction ID */
#define V0_INC_OP               0x00    // rs2 = ++rs1;
#define V0_UINC_OP              0x01    // unsigned variant
#define V0_DEC_OP               0x02    // rs2 = --rs1;
#define V0_UDEC_OP              0x03    // unsigned variant
#define V0_ADD_OP               0x04    // dst = rs2 + rs1;
#define V0_UADD_OP              0x05    // unsigned variant
#define V0_ADC_OP               0x06    // sets CF in the MSW
#define V0_SUB_OP               0x07    // dst = rs2 - rs1;
#define V0_USUB_OP              0x08    // unsigned varinat
#define V0_SBB_OP               0x09    // sets CF in the MSW

/*
 * multiplication and division
 * - register-only operands
 * - conditional execution, condition code in src2
 * - instruction folding, shift-count and operation in parm
 */
#define V0_MULDIV_UNIT          0x03
/* instruction ID */
#define V0_MUL_OP               0x00    // dst = rs2 * rs1;
#define V0_UMUL_OP              0x01    // unsigned variant
#define V0_MHI_OP               0x02    // dst = (rs2 * rs1) >> 32;
#define V0_UMHI_OP              0x03    // unsigned variant
#define V0_DIV_OP               0x04    // dst = rs2 / rs1;
#define V0_UDIV_OP              0x05    // unsigned variant
#define V0_REM_OP               0x06    // dst = rs2 % rs1;
#define V0_UREM_OP              0x07    // unsigned variant
#define V0_IRP_OP               0x08    // rs2 = irp(rs1) so x * rs2 => x / rs2
#define V0_UIRP_OP              0x09    // unsigned variant

/*
 * bit operations
 * - register-only operands
 * - conditional execution, condition code in src2
 */
#define V0_BIT_UNIT             0x04
/* instruction ID */
#define V0_ZEXB_OP              0x00    // zero-extend byte to register size
#define V0_ZEXH_OP              0x01    // zero-extend halfword to register size
#define V0_ZEX_OP               0x02    // zero-extend word to register size
#define V0_SEXB_OP              0x03    // sign-extend byte to register size
#define V0_SEXH_OP              0x04    // sign-extend halfword to register size
#define V0_SEX_OP               0x05    // sign-extend word to register size
#define V0_NEG_OP               0x06    // arithmetic negation
#define V0_PAR_OP               0x07    // bit parity, 0 for even
#define V0_HAM_OP               0x08    // hamming weight, bit population
#define V0_CTZ_OP               0x09    // count trailing [low] zero-bits
#define V0_CLZ_OP               0x0a    // count leading [high] zero-bits
#define V0_HSH_OP               0x0b    // compute hash function for value
#define V0_HUN_OP               0x0c    // compute reverse hash function

/*
 * comparison operations
 * - register- and address-operands
 */
#define V0_COMP_UNIT            0x05
/* instruction ID */
#define V0_CMP_OP               0x00    // comparison
#define V0_CMU_OP               0x01    // unsigned comparison
#define V0_TST_OP               0x02    // dst = rs2 & rs1;
#define V0_TEQ_OP               0x03    // dst = rs2 ^ rs1;

/*
 * branch and subroutine operations
 * - register- and address operands
 */
#define V0_BRANCH_UNIT          0x06
/* instruction ID */
#define V0_JMP_OP               0x00    // unconditional jump
#define V0_BEQ_OP               0x01    // equal/zero
#define V0_BZF_OP               V0_BEQ_OP
#define V0_BNE_OP               0x02    // not equal/zero
#define V0_BNZ_OP               V0_BNE_OP
#define V0_BGT_OP               0x03    // greater than
#define V0_BLT_OP               0x04    // less than
#define V0_BGE_OP               0x05    // greater than or equal
#define V0_BLE_OP               0x06    // less than or equal
#define V0_BHS_OP               0x07    // unsigned greater than or equal
#define V0_BLO_OP               0x08    // unsigned less then
#define V0_BMI_OP               0x09    // negative
#define V0_BPL_OP               0x0a    // positive
#define V0_BOF_OP               0x0b    // overflow
#define V0_BNO_OP               0x0c    // no overflow
#define V0_BHI_OP               0x0d    // unsigned greater than
#define V0_BLS_OP               0x0e    // unsigned less than or equal
#define V0_SKP_OP               0x0f    // skip next instruction
/* unconditional branch operations */
#define V0_JMI_OP               0x10    // jump to immediate address
#define V0_JMR_OP               0x11    // jump to address in register
#define V0_JAL_OP               0x12    // jump and link
#define V0_JPC_OP               0x13    // PC-relative; position-independent
#define V0_JMF_OP               0x14    // branch to specified segment
#define V0_SRT_OP               0x15    // call subroutine
#define V0_RET_OP               0x16    // return from subroutine
#define V0_SME_OP               0x17    // enter system mode
#define V0_SMX_OP               0x18    // exit system mode
#define V0_THR_OP               0x19    // launch thread
#define V0_THX_OP               0x1a    // exit thread
#define V0_TAT_OP               0x1b    // set waiter (join/attach)
#define V0_TDT_OP               0x1c    // remove waiter
#define V0_TWT_OP               0x1d    // wait on channel
#define V0_TWK_OP               0x1e    // wake-up from wait

/*
 * load and store operations
 * - register- and address-operands
 * - conditional execution, condition code in src2
 */
#define V0_LDSTR_UNIT           0x07
/* instruction ID */
#define V0_ADR_OP               0x00
#define V0_MVR_OP               0x01    // user-to-user register
#define V0_MVU_OP               0x02    // user-to-system register
#define V0_MVX_OP               0x03    // system-to-user register
#define V0_LDB_OP               0x04    // load byte (8-bit)
#define V0_LDH_OP               0x05    // load halfword (16-bit)
#define V0_LDW_OP               0x06    // load word (32-bit)
#define V0_LDD_OP               0x07    // load doubleword (64-bit)
#define V0_LFS_OP               0x08    // load single-precision float (32-bit)
#define V0_LFD_OP               0x09    // load double-precision float (64-bit)
#define V0_LFX_OP               0x0a    // load fixed-point number (64-bit)
#define V0_POP_OP               0x0b    // pop from stack to register
#define V0_LDM_OP               0x0c    // load many registers
#define V0_STB_OP               0x0d    // store byte to memory
#define V0_STH_OP               0x0e    // store halfword to memory
#define V0_STW_OP               0x0f    // store word to memory
#define V0_STD_OP               0x10    // store doubleword to memory
#define V0_SFS_OP               0x11    // store single-precision float
#define V0_SFD_OP               0x12    // store double-precision float
#define V0_SFX_OP               0x13    // store fixed-point number
#define V0_PSH_OP               0x14    // push register to memory
#define V0_STM_OP               0x15    // store many registers; immediate bits

/*
 * atomic operations
 * - conditional execution, condition code in src2
 */
#define V0_ATOMIC_UNIT          0x08
/* instruction ID */
#define V0_XORR                 0x00    // atomic OR, memory destination
#define V0_XX0R                 0x01    // atomic XOR, memory destination
#define V0_XAND                 0x02    // atomic AND, memory destination
#define V0_XINC_OP              0x03    // atomic increment, fetch result
#define V0_XDEC_OP              0x04    // atomic decrement, fetch result
#define V0_XADD_OP              0x05    // atomic addition, fetch result
#define V0_BST_OP               0x06    // set bit; dst |= rs2 << rs1;
#define V0_BCL_OP               0x07    // clear bit; dst &= ~(rs2 << rs1);
#define V0_BFL_OP               0x08    // flip bit; dst ^= rs2 << rs1;
#define V0_BIC_OP               0x09    // bitwise clear; dst = rs2 & ~rs1;
#define V0_BTS_OP               0x0a    // bit test-and-set
#define V0_BTC_OP               0x0b    // bit test-and-clear
#define V0_BTF_OP               0x0c    // bit test-and-flip
#define V0_CAS_OP               0x0d    // [atomic] compare and swap
#define V0_SMT_OP               0x0e    // start memory tramsaction [page]
#define V0_FMT_OP               0x0f    // end memory transaction [page]

#define V0_SYSTEM_UNIT          0x09
/* instruction ID */
#define V0_HLT_OP               0x00    // halt processor, wait for interrupts
#define V0_RST_OP               0x01    // reset processor
#define V0_BRK_OP               0x02    // execution breakpoints
#define V0_INT_OP               0x03    // raise software interrupt
#define V0_NMI_OP               0x04    // non-maskable interrupt
#define V0_CLI_OP               0x05    // disable interrupts or clear mask-bits
#define V0_STI_OP               0x06    // enable interrupts or set mask-bits
#define V0_CSP_OP               0x07    // set [global] cache-policy
#define V0_CPF_OP               0x08    // cache prefetch; level argument
#define V0_FCL_OP               0x09    // flush cacheline(s); level argument
#define V0_FPG_OP               0x0a    // invalidate/flush page TLB-entr(y/ies)
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
#define V0_IPI_OP               0x16    // inter-processor interrupt
#define V0_BUS_OP               0x17    // bus operation
#define v0_DEV_OP               0x18    // peripheral device operation
#define V0_MPC_OP               0x19    // [multi]processor control operation

/* IOC-commands (in parm-field) */
#define IO_PROBE_BUS            0x00
#define IO_CONF_BUS             0x01
#define IO_PROBE_DEV            0x02
#define IO_CONF_DEV             0x03

#endif /* __MACH_V0_INST_H__ */

