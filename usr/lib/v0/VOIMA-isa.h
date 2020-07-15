#ifndef __V0_VOIMA_ISA_H__
#define __V0_VOIMA_ISA_H__

#include <zero/cdefs.h>

/*
 * Voima Game Console
 * ------------------
 * - modern 32-bit RISC-inspired microprocessor with instructions for efficient
 *   data processing
 */

/*
 * Instruction Set
 * ---------------
 * - one-to-one mapping to C-language operators
 */

#define V0_UNIT_SHIFT           5
#define V0_SYS_UNIT_ID          (V0_SYS_UNIT << V0_UNIT_SHIFT)
#define V0_ALU_UNIT_ID          (V0_ALU_UNIT << V0_UNIT_SHIFT)
#define V0_SUBRT_UNIT_ID        (V0_SYS_UNIT << V0_UNIT_SHIFT)
#define V0_BIT_UNIT_ID          (V0_BIT_UNIT << V0_UNIT_SHIFT)
#define V0_LDSTR_UNIT_ID        (V0_LDSTR_UNIT << V0_UNIT_SHIFT)
#define V0_SYS_UNIT             0x00
#define V0_ALU_UNIT             0x01
#define V0_SUBRT_UNIT           0x02
#define V0_BIT_UNIT             0x03
#define V0_LDSTR_UNIT           0x04
#define V0_ATOM_UNIT            0x05

/*
 * Unit #0; System Functionality
 * -----------------------------
 */

/* SYS-unit */
#define V0_NOP_OP               0x00 // no operation
#define V0_FTR_OP               0x01 // check machine feature
#define V0_HLT_OP               0x02 // halt, wait for interrupts
#define V0_RST_OP               0x03 // reset, hibernate, reboot
#define V0_SIM_OP               0x04 // set interrupt mask
#define V0_INT_OP               0x05 // deliver interrupt
#define V0_RTI_OP               0x06 // return from interrupt
#define V0_DEV_OP               0x07 // [bus and] device operations
#define V0_CPF_OP               0x08 // cacheline prefetch
#define V0_RBR_OP               0x09 // read-barrier
#define V0_WBR_OP               0x0a // write-barrier
#define V0_BAR_OP               0x0b // memory barrier; default to full
#define V0_WFE_OP               0x0c // wait for [write] event
#define V0_SEV_OP               0x0d // signal [write] event
#define V0_IPG_OP               0x0e // invalidate page TLB-entry
#define V0_STB_OP               0x0f // set bound-range

/*
 * Unit #1; Arithmetic-Logic Unit
 * ------------------------------
 */

/* ALU-unit */
#define V0_NOT_OP               0x00 // bitwise NOT
#define V0_NEG_OP               0x01 // arithmetic negation
#define V0_AND_OP               0x02 // logical AND
#define V0_OR_OP                0x03 // logical OR
#define V0_XOR_OP               0x04 // logical XOR
#define V0_NOR_OP               0x05 // logical NOR
#define V0_XNOR_OP              0x06 // logical XNOR
#define V0_NAND_OP              0x07 // logical NAND
#define V0_INC_OP               0x08 // increment by one
#define V0_DEC_OP               0x09 // decrement by one
#define V0_SLL_OP               0x0a // shift left (logical
#define V0_SRL_OP               0x0b // shift right (logical
#define V0_SAR_OP               0x0c // shift right (arithmetic
#define V0_ROL_OP               0x0d // rotate left
#define V0_ROR_OP               0x0e // rotate right
#define V0_RCL_OP               0x0f // rotate left with carry
#define V0_RCR_OP               0x10 // rotate right with carry
#define V0_ADD_OP               0x11 // addition
#define V0_ADDU_OP              0x12 // addition (ignore overflow
#define V0_ADC_OP               0x13 // addition with carry
#define V0_SUB_OP               0x14 // subtraction
#define V0_SBC_OP               0x15 // subtraction with carry/borrow
#define V0_MUL_OP               0x16 // multiplication
#define V0_UMUL_OP              0x17 // unsigned multiplication
#define V0_MHI_OP               0x18 // multiplication, high result word
#define V0_UMHI_OP              0x19 // unsigned multiplication, high result word
#define V0_DIV_OP               0x1a // division
#define V0_UDIV_OP              0x1b // unsigned division
#define V0_IDIV_OP              0x1c // divide by multiplying with inverse reciprocal
#define V0_REM_OP               0x1d // remainder
#define V0_UREM_OP              0x1e // unsigned remainder

/*
 * Unit #2; Subroutines and Branches
 * ---------------------------------
 */

/* SUBRT-unit */
#define V0_CMP_OP               0x00 // comparison
#define V0_UCMP_OP              0x01 // comparison
#define V0_JMP_OP               0x02 // jmp(adr);
#define V0_JPC_OP               0x03 // jmp(pc + adr);
#define V0_BLR_OP               0x04 // branch and link (store next PC to LR)
#define V0_BEQ_OP               0x05 // branch if equal (zero-flag set)
#define V0_BNE_OP               0x06 // branch if not equal (zero-flag not set)
#define V0_BLT_OP               0x07 // branch if less than
#define V0_BLE_OP               0x08 // branch if less or equal
#define V0_BGT_OP               0x09 // branch if greater than
#define V0_BGE_OP               0x0a // branch if greater or equal
#define V0_BULT_OP              0x0b // branch if less than, unsigned
#define V0_BULE_OP              0x0c // branch if less or equal, unsigned
#define V0_BUGT_OP              0x0d // branch if greater than, unsigned
#define V0_BUGE_OP              0x0e // branch if greater or equal, unsigned
#define V0_BCF_OP               0x0f // branch if carry set
#define V0_BNC_OP               0x10 // branch if carry not set
#define V0_BOF_OP               0x11 // branch if overflow set
#define V0_BNO_OP               0x12 // branch if overflow not set
#define V0_BNR_OP               0x13 // branch if no-round set (fixed-point, FPU)
#define V0_SBR_OP               0x14 // lr = pc; jmp(adr);
#define V0_RET_OP               0x15 // sp += tmp; jmp(lr);
/* MT-extension */
#define V0_SME_OP               0x16 // save(uctx, map);
                              // lr = pc; fp = fp0; sp = sp0;
                              // sp += src;
#define V0_SMX_OP               0x17 // r1 = src;
                              // fp0 = fp; sp0 = sp;
                              // pop(uctx);
#define V0_THR_OP               0x18 // push(uctx); tcb = ctx; thr(dest, adr);
#define V0_THX_OP               0x19 // trap(func, code); load(tcb);

/*
 * Unit #3; Counting and other Bit-Routines
 * ----------------------------------------
 */

/* BIT-unit */
#define V0_TST_OP               0x00 // AND without setting result (test bits)
#define V0_ZEX_OP               0x01 // zero-extend from given width to 32-bit
#define V0_SEX_OP               0x02 // sign-extend from given width to 32-bit
#define V0_CLZ_OP               0x03 // count leading zero-bits
#define V0_CTZ_OP               0x04 // count trailing zero-bits
#define V0_HAM_OP               0x05 // bit population/Hamming weight/count of 1-bits
#define V0_PAR_OP               0x06 // compute bit-parity; 0 for even # of 1-bits
#define V0_HSH_OP               0x07 // compute 32-bit hash-value from key
#define V0_HUN_OP               0x08 // reverse 32-bit hash-value back to key
#define V0_BEX_OP               0x09 // extract bits
#define V0_BPK_OP               0x0a // pack bits

/*
 * Unit #4; Load-Store (incl. Conditional Move)
 * --------------------------------------------
 */

/* LDSTR-unit */
#define V0_LEA_OP               0x00 // load effective address
#define V0_LDR_OP               0x01 // memory-to-register load
#define V0_STR_OP               0x02 // register-to-memory store
#define V0_PSH_OP               0x03 // push register
#define V0_POP_OP               0x04 // pop register
#define V0_STM_OP               0x05 // store many (uses register bitmap)
#define V0_LDM_OP               0x06 // load many (uses register bitmap)
#define V0_MVS_OP               0x07 // user-to-system register copy
#define V0_LDS_OP               0x08 // memory-to-system-register load
#define V0_STS_OP               0x09 // system-register to memory store
#define V0_SMS_OP               0x0a // store many system registers
#define V0_LMS_OP               0x0b // load many system registers
#define V0_LNT_OP               0x0c // non-temporal load (bypass cache)
#define V0_SNT_OP               0x0d // non-temporal store
/* CMOVE-extension; transfer type determined by parm & V0_XFER_DIR_MASK*/
#define V0_MEQ_OP               0x10 // if (ZF) { dest = src; }
#define V0_LEQ_OP               0x10 // if (ZF) { dest =-adr; }
#define V0_SEQ_OP               0x10 // if (ZF) {-adr = src; }
#define V0_MNE_OP               0x11 // if (!ZF) { dest = src; }
#define V0_LNE_OP               0x11 // if (!ZF) { dest =-adr; }
#define V0_SNE_OP               0x11 // if (!ZF) {-adr = src; }
#define V0_MLT_OP               0x12 // if (LT) { dest = src; }
#define V0_LLT_OP               0x12 // if (LT) { dest =-adr; }
#define V0_SLT_OP               0x12 // if (LT) {-adr = src; }
#define V0_MLE_OP               0x13 // if (LT || ZF) { dest = src; }
#define V0_LLE_OP               0x13 // if (LT || ZF) { dest =-adr; }
#define V0_SLE_OP               0x13 // if (LT || ZF) {-adr = src; }
#define V0_MGT_OP               0x14 // if (!LT && !ZF) { dest = src; }
#define V0_LGT_OP               0x14 // if (!LT && !ZF) { dest = src; }
#define V0_SGT_OP               0x14 // if (!LT && !ZF) { dest = src; }
#define V0_MGE_OP               0x15 // if (!LT || ZF) { dest = src; }
#define V0_LGE_OP               0x15 // if (!LT || ZF) { dest = src; }
#define V0_SGE_OP               0x15 // if (!LT || ZF) { dest = src; }
#define V0_MULT_OP              0x16 // if (LT) { dest = src; }
#define V0_LULT_OP              0x16 // if (LT) { dest =-adr; }
#define V0_SULT_OP              0x16 // if (LT) { dest = src; }
#define V0_MULE_OP              0x17 // if (LT || ZF) { dest = src; }
#define V0_LULE_OP              0x17 // if (LT || ZF) { dest =-adr; }
#define V0_SULE_OP              0x17 // if (LT || ZF) { *adr = src; }
#define V0_MUGT_OP              0x18 // if (!LT && !ZF) { dest = src; }
#define V0_LUGT_OP              0x18 // if (!LT && !ZF) { dest = *adr; }
#define V0_SUGT_OP              0x18 // if (!LT && !ZF) { *adr = src; }
#define V0_MUGE_OP              0x19 // if (!LT || ZF) { dest = src; }
#define V0_LUGE_OP              0x19 // if (!LT || ZF) { dest = *adr; }
#define V0_SUGE_OP              0x19 // if (!LT || ZF) { *adr = src; }
#define V0_MCF_OP               0x1a // if (CF) { dest = src; }
#define V0_LCF_OP               0x1a // if (CF) { dest = *adr; }
#define V0_SCF_OP               0x1a // if (CF) { *adr = src; }
#define V0_MNC_OP               0x1b // if (!CF) { dest = src; }
#define V0_LNC_OP               0x1b // if (!CF) { dest = *adr; }
#define V0_SNC_OP               0x1b // if (!CF) { *adr = src; }
#define V0_MOF_OP               0x1c // if (OF) { dest = src; }
#define V0_LOF_OP               0x1c // if (OF) { dest = *adr; }
#define V0_SOF_OP               0x1c // if (OF) { *adr = src; }
#define V0_MNO_OP               0x1d // if (!OF) { dest = src; }
#define V0_LNO_OP               0x1d // if (!OF) { dest = *adr; }
#define V0_SNO_OP               0x1d // if (!OF) { *adr = src; }
#define V0_MNR_OP               0x1e // if (!RF) { dest = src; }
#define V0_LNR_OP               0x1e // if (!RF) { dest = *adr; }
#define V0_SNR_OP               0x1e // if (!RF) { *adr = src; }

/*
 * Unit #5; Atomic [Memory] Operations
 * -----------------------------------
 */

/* ATOM-unit; bus-locking memory operations */
#define V0_RAND_OP              0x00 // atomic fetch-and-AND
#define V0_RLOR_OP              0x01 // atomic fetch-and-OR
#define V0_RXOR_OP              0x02 // atomic fetch-and-XOR
#define V0_RINC_OP              0x03 // atomic fetch-and-increment by one
#define V0_RDEC_OP              0x04 // atomic fetch-and-decrement by one
#define V0_RADD_OP              0x05 // atomic fetch-and-add
#define V0_CAS_OP               0x06 // atomic compare-and-swap
#define V0_XCH_OP               0x07 // atomic exchange
#define V0_BTC_OP               0x08 // atomic bit test-and-clear
#define V0_BTS_OP               0x09 // atomic bit test-and-set
#define V0_BSF_OP               0x0a // atomic bit scan-and-set forward (up
#define V0_BSR_OP               0x0b // atomic bit scan-and-set reverse (down

/*
 * Unit #6; CORDIC Floating-Point Unit? :)
 */

#define V0_COPROC_UNIT          0x07

#define V0_INS_UNIT_MASK        0xe0
#define V0_INS_OP_MASK          0x1f
#define V0_INS_SRC_SHIFT        8
#define V0_INS_DEST_SHIFT       12
#define V0_INS_REG3_SHIFT       16
#define V0_INS_VAL8_SHIFT       16
#define V0_INS_PARM_SHIFT       16
#define V0_INS_REG_BITS         4
/* indicates presence [possibly-aligned] imm32-argument after opcode */
#define V0_INS_IMM32_BIT        0x80000000
/* 8-bit immediate value */
#define V0_INS_VAL8_MASK        0x00ff0000
/* 4-bit extra register ID */
#define V0_INS_REG3_MASK        0x000f0000
#define V0_INS_IMM_UNS_BIT      0x00100000
/* addressing-mode flags */
#define V0_INS_ADR_BASE_BIT     0x01000000
#define V0_INS_ADR_NDX_BIT      0x02000000
#define V0_INS_ADR_OFS_BIT      0x04000000
#define V0_INS_REG_TO_REG_BIT   0x00000000
#define V0_INS_REG_TO_MEM_BIT   0x08000000
#define V0_INS_MEM_TO_REG_BIT   0x10000000
#define V0_INS_MEM_XFER_MASK    0x18000000
/* operand-size flags */
#define V0_INS_OPSIZE_BIT_1     0x20000000
#define V0_INS_OPSIZE_BIT_2     0x40000000
struct v0ins {
    int8_t  op;             // 3-bit unit and 5-bit operation ID
    int8_t  regs;           // source and destination register IDs
    int16_t parm;           // instruction-dependent operands/parameters
    int32_t imm32[C_VLA];   // possible 32-bit immediate operand
};

#endif /* __V0_VOIMA_ISA_H__ */

