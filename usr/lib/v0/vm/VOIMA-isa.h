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

/*
 * Unit #0; System Functionality
 * -----------------------------
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

#define V0_NOP_OP               (V0_SYS_UNIT_ID | 00) // no operation
#define V0_FTR_OP               (V0_SYS_UNIT_ID | 01) // check machine feature
#define V0_HLT_OP               (V0_SYS_UNIT_ID | 02) // halt, wait for interrupts
#define V0_RST_OP               (V0_SYS_UNIT_ID | 03) // reset, hibernate, reboot
#define V0_SIM_OP               (V0_SYS_UNIT_ID | 04) // set interrupt mask
#define V0_INT_OP               (V0_SYS_UNIT_ID | 05) // deliver interrupt
#define V0_RTI_OP               (V0_SYS_UNIT_ID | 06) // return from interrupt
#define V0_DEV_OP               (V0_SYS_UNIT_ID | 07) // [bus and] device operations
#define V0_CSP_OP               (V0_SYS_UNIT_ID | 08) // set cache policy (NONE, WT, WB)
#define V0_CPF_OP               (V0_SYS_UNIT_ID | 09) // cacheline prefetch
#define V0_RBR_OP               (V0_SYS_UNIT_ID | 0a) // read-barrier
#define V0_WBR_OP               (V0_SYS_UNIT_ID | 0b) // write-barrier
#define V0_BAR_OP               (V0_SYS_UNIT_ID | 0c) // memory barrier; default to full
#define V0_WFE_OP               (V0_SYS_UNIT_ID | 0d) // wait for [write] event
#define V0_SEV_OP               (V0_SYS_UNIT_ID | 0e) // signal [write] event
#define V0_IPG_OP               (V0_SYS_UNIT_ID | 0f) // invalidate page TLB-entry
#define V0_STB_OP               (V0_SYS_UNIT_ID | 10) // set bound-range
#define V0_RAND_OP              (V0_SYS_UNIT_ID | 11) // atomic fetch-and-AND
#define V0_RLOR_OP              (V0_SYS_UNIT_ID | 12) // atomic fetch-and-OR
#define V0_RXOR_OP              (V0_SYS_UNIT_ID | 13) // atomic fetch-and-XOR
#define V0_RINC_OP              (V0_SYS_UNIT_ID | 14) // atomic fetch-and-increment by one
#define V0_RDEC_OP              (V0_SYS_UNIT_ID | 15) // atomic fetch-and-decrement by one
#define V0_RADD_OP              (V0_SYS_UNIT_ID | 16) // atomic fetch-and-add
#define V0_CAS_OP               (V0_SYS_UNIT_ID | 17) // atomic compare-and-swap
#define V0_XCH_OP               (V0_SYS_UNIT_ID | 18) // atomic exchange
#define V0_BTC_OP               (V0_SYS_UNIT_ID | 19) // atomic bit test-and-clear
#define V0_BTS_OP               (V0_SYS_UNIT_ID | 1a) // atomic bit test-and-set
#define V0_BSF_OP               (V0_SYS_UNIT_ID | 1b) // atomic bit scan-and-set forward (up)
#define V0_BSR_OP               (V0_SYS_UNIT_ID | 1c) // atomic bit scan-and-set reverse (down)

/* ALU-Unit */
#define V0_NOT_OP               (V0_ALU_UNIT_ID | 00) // bitwise NOT
#define V0_NEG_OP               (V0_ALU_UNIT_ID | 01) // arithmetic negation
#define V0_AND_OP               (V0_ALU_UNIT_ID | 02) // logical AND
#define V0_OR_OP                (V0_ALU_UNIT_ID | 03) // logical OR
#define V0_XOR_OP               (V0_ALU_UNIT_ID | 04) // logical XOR
#define V0_NOR_OP               (V0_ALU_UNIT_ID | 05) // logical NOR
#define V0_XNOR_OP              (V0_ALU_UNIT_ID | 06) // logical XNOR
#define V0_NAND_OP              (V0_ALU_UNIT_ID | 07) // logical NAND
#define V0_INC_OP               (V0_ALU_UNIT_ID | 08) // increment by one
#define V0_DEC_OP               (V0_ALU_UNIT_ID | 09) // decrement by one
#define V0_SLL_OP               (V0_ALU_UNIT_ID | 0a) // shift left (logical)
#define V0_SRL_OP               (V0_ALU_UNIT_ID | 0b) // shift right (logical)
#define V0_SAR_OP               (V0_ALU_UNIT_ID | 0c) // shift right (arithmetic)
#define V0_ROL_OP               (V0_ALU_UNIT_ID | 0d) // rotate left
#define V0_ROR_OP               (V0_ALU_UNIT_ID | 0e) // rotate right
#define V0_RCL_OP               (V0_ALU_UNIT_ID | 0f) // rotate left with carry
#define V0_RCR_OP               (V0_ALU_UNIT_ID | 10) // rotate right with carry
#define V0_ADD_OP               (V0_ALU_UNIT_ID | 11) // addition
#define V0_ADDU_OP              (V0_ALU_UNIT_ID | 12) // addition (ignore overflow)
#define V0_ADC_OP               (V0_ALU_UNIT_ID | 13) // addition with carry
#define V0_SUB_OP               (V0_ALU_UNIT_ID | 14) // subtraction
#define V0_SBC_OP               (V0_ALU_UNIT_ID | 15) // subtraction with carry/borrow
#define V0_MUL_OP               (V0_ALU_UNIT_ID | 16) // multiplication
//#define V0_UMUL_OP              (V0_ALU_UNIT_ID | 17) // unsigned multiplication
#define V0_MHI_OP               (V0_ALU_UNIT_ID | 18) // multiplication, high result word
//#define V0_UMHI_OP              (V0_ALU_UNIT_ID | 19) // unsigned multiplication, high result word
#define V0_DIV_OP               (V0_ALU_UNIT_ID | 1b) // division
//#define V0_UDIV_OP              (V0_ALU_UNIT_ID | 1c) // unsigned division
#define V0_IDIV_OP              (V0_ALU_UNIT_ID | 1a) // divide by multiplying with inverse reciprocal
#define V0_REM_OP               (V0_ALU_UNIT_ID | 1d) // remainder
#define V0_UREM_OP              (V0_ALU_UNIT_ID | 1e) // unsigned remainder

/* SUBRT-Unit */
#define V0_CMP_OP               (V0_SUBRT_UNIT_ID | 00) // comparison
#define V0_UCMP_OP              (V0_SUBRT_UNIT_ID | 01) // comparison
#define V0_JMP_OP               (V0_SUBRT_UNIT_ID | 02) // jmp(adr);
#define V0_JPC_OP               (V0_SUBRT_UNIT_ID | 03) // jmp(pc + adr);
#define V0_BLR_OP               (V0_SUBRT_UNIT_ID | 04) // branch and link (store next PC to LR)
#define V0_BEQ_OP               (V0_SUBRT_UNIT_ID | 05) // branch if equal (zero-flag set)
#define V0_BNE_OP               (V0_SUBRT_UNIT_ID | 06) // branch if not equal (zero-flag not set)
#define V0_BLT_OP               (V0_SUBRT_UNIT_ID | 07) // branch if less than
#define V0_BLE_OP               (V0_SUBRT_UNIT_ID | 08) // branch if less or equal
#define V0_BGT_OP               (V0_SUBRT_UNIT_ID | 09) // branch if greater than
#define V0_BGE_OP               (V0_SUBRT_UNIT_ID | 0a) // branch if greater or equal
#define V0_BULT_OP              (V0_SUBRT_UNIT_ID | 0b) // branch if less than, unsigned
#define V0_BULE_OP              (V0_SUBRT_UNIT_ID | 0c) // branch if less or equal, unsigned
#define V0_BUGT_OP              (V0_SUBRT_UNIT_ID | 0d) // branch if greater than, unsigned
#define V0_BUGE_OP              (V0_SUBRT_UNIT_ID | 0e) // branch if greater or equal, unsigned
#define V0_BCF_OP               (V0_SUBRT_UNIT_ID | 0f) // branch if carry set
#define V0_BNC_OP               (V0_SUBRT_UNIT_ID | 10) // branch if carry not set
#define V0_BOF_OP               (V0_SUBRT_UNIT_ID | 11) // branch if overflow set
#define V0_BNO_OP               (V0_SUBRT_UNIT_ID | 12) // branch if overflow not set
#define V0_BNR_OP               (V0_SUBRT_UNIT_ID | 13) // branch if no-round set (fixed-point, FPU)
#define V0_SBR_OP               (V0_SUBRT_UNIT_ID | 14) // lr = pc; jmp(adr);
#define V0_RET_OP               (V0_SUBRT_UNIT_ID | 15) // sp += tmp; jmp(lr);
/* MT-extension */
#define V0_SME_OP               (V0_SUBRT_UNIT_ID | 16) // save(uctx, map);
                              // lr = pc; fp = fp0; sp = sp0;
                              // sp += src;
#define V0_SMX_OP               (V0_SUBRT_UNIT_ID | 17) // r1 = src;
                              // fp0 = fp; sp0 = sp;
                              // pop(uctx);
#define V0_THR_OP               (V0_SUBRT_UNIT_ID | 18) // push(uctx); tcb = ctx; thr(dest, adr);
#define V0_THX_OP               (V0_SUBRT_UNIT_ID | 19) // trap(func, code); load(tcb);

/* BIT-Unit */
#define V0_TST_OP               (V0_BIT_UNIT_ID | 00) // AND without setting result (test bits)
#define V0_ZEX_OP               (V0_BIT_UNIT_ID | 01) // zero-extend from given width to 32-bit
#define V0_SEX_OP               (V0_BIT_UNIT_ID | 02) // sign-extend from given width to 32-bit
#define V0_CLZ_OP               (V0_BIT_UNIT_ID | 03) // count leading zero-bits
#define V0_CTZ_OP               (V0_BIT_UNIT_ID | 04) // count trailing zero-bits
#define V0_HAM_OP               (V0_BIT_UNIT_ID | 05) // bit population/Hamming weight/count of 1-bits
#define V0_PAR_OP               (V0_BIT_UNIT_ID | 06) // compute bit-parity; 0 for even # of 1-bits
#define V0_HSH_OP               (V0_BIT_UNIT_ID | 07) // compute 32-bit hash-value from key
#define V0_HUN_OP               (V0_BIT_UNIT_ID | 08) // reverse 32-bit hash-value back to key
#define V0_BEX_OP               (V0_BIT_UNIT_ID | 09) // extract bits
#define V0_BPK_OP               (V0_BIT_UNIT_ID | 0a) // pack bits

/* LDSTR-Unit */
#define V0_LEA_OP               (V0_LDSTR_UNIT_ID | 00) // load effective address
#define V0_LDR_OP               (V0_LDSTR_UNIT_ID | 01) // memory-to-register load
#define V0_STR_OP               (V0_LDSTR_UNIT_ID | 02) // register-to-memory store
#define V0_PSH_OP               (V0_LDSTR_UNIT_ID | 03) // push register
#define V0_POP_OP               (V0_LDSTR_UNIT_ID | 04) // pop register
#define V0_STM_OP               (V0_LDSTR_UNIT_ID | 05) // store many (uses register bitmap)
#define V0_LDM_OP               (V0_LDSTR_UNIT_ID | 06) // load many (uses register bitmap)
#define V0_MVS_OP               (V0_LDSTR_UNIT_ID | 07) // user-to-system register copy
#define V0_LDS_OP               (V0_LDSTR_UNIT_ID | 08) // memory-to-system-register load
#define V0_STS_OP               (V0_LDSTR_UNIT_ID | 09) // system-register to memory store
#define V0_SMS_OP               (V0_LDSTR_UNIT_ID | 0a) // store many system registers
#define V0_LMS_OP               (V0_LDSTR_UNIT_ID | 0b) // load many system registers
#define V0_LNT_OP               (V0_LDSTR_UNIT_ID | 0c) // non-temporal load (bypass cache)
#define V0_SNT_OP               (V0_LDSTR_UNIT_ID | 0d) // non-temporal store
/* CMOVE-extension; transfer type determined by parm & V0_XFER_DIR_MASK*/
#define V0_MEQ_OP               (V0_LDSTR_UNIT_ID | 10) // if (ZF) { dest = src; }
#define V0_LEQ_OP               (V0_LDSTR_UNIT_ID | 10) // if (ZF) { dest =-adr; }
#define V0_SEQ_OP               (V0_LDSTR_UNIT_ID | 10) // if (ZF) {-adr = src; }
#define V0_MNE_OP               (V0_LDSTR_UNIT_ID | 11) // if (!ZF) { dest = src; }
#define V0_LNE_OP               (V0_LDSTR_UNIT_ID | 11) // if (!ZF) { dest =-adr; }
#define V0_SNE_OP               (V0_LDSTR_UNIT_ID | 11) // if (!ZF) {-adr = src; }
#define V0_MLT_OP               (V0_LDSTR_UNIT_ID | 12) // if (LT) { dest = src; }
#define V0_LLT_OP               (V0_LDSTR_UNIT_ID | 12) // if (LT) { dest =-adr; }
#define V0_SLT_OP               (V0_LDSTR_UNIT_ID | 12) // if (LT) {-adr = src; }
#define V0_MLE_OP               (V0_LDSTR_UNIT_ID | 13) // if (LT || ZF) { dest = src; }
#define V0_LLE_OP               (V0_LDSTR_UNIT_ID | 13) // if (LT || ZF) { dest =-adr; }
#define V0_SLE_OP               (V0_LDSTR_UNIT_ID | 13) // if (LT || ZF) {-adr = src; }
#define V0_MGT_OP               (V0_LDSTR_UNIT_ID | 14) // if (!LT && !ZF) { dest = src; }
#define V0_LGT_OP               (V0_LDSTR_UNIT_ID | 14) // if (!LT && !ZF) { dest = src; }
#define V0_SGT_OP               (V0_LDSTR_UNIT_ID | 14) // if (!LT && !ZF) { dest = src; }
#define V0_MGE_OP               (V0_LDSTR_UNIT_ID | 15) // if (!LT || ZF) { dest = src; }
#define V0_LGE_OP               (V0_LDSTR_UNIT_ID | 15) // if (!LT || ZF) { dest = src; }
#define V0_SGE_OP               (V0_LDSTR_UNIT_ID | 15) // if (!LT || ZF) { dest = src; }
#define V0_MULT_OP              (V0_LDSTR_UNIT_ID | 16) // if (LT) { dest = src; }
#define V0_LULT_OP              (V0_LDSTR_UNIT_ID | 16) // if (LT) { dest =-adr; }
#define V0_SULT_OP              (V0_LDSTR_UNIT_ID | 16) // if (LT) { dest = src; }
#define V0_MULE_OP              (V0_LDSTR_UNIT_ID | 17) // if (LT || ZF) { dest = src; }
#define V0_LULE_OP              (V0_LDSTR_UNIT_ID | 17) // if (LT || ZF) { dest =-adr; }
#define V0_SULE_OP              (V0_LDSTR_UNIT_ID | 17) // if (LT || ZF) { *adr = src; }
#define V0_MUGT_OP              (V0_LDSTR_UNIT_ID | 18) // if (!LT && !ZF) { dest = src; }
#define V0_LUGT_OP              (V0_LDSTR_UNIT_ID | 18) // if (!LT && !ZF) { dest = *adr; }
#define V0_SUGT_OP              (V0_LDSTR_UNIT_ID | 18) // if (!LT && !ZF) { *adr = src; }
#define V0_MUGE_OP              (V0_LDSTR_UNIT_ID | 19) // if (!LT || ZF) { dest = src; }
#define V0_LUGE_OP              (V0_LDSTR_UNIT_ID | 19) // if (!LT || ZF) { dest = *adr; }
#define V0_SUGE_OP              (V0_LDSTR_UNIT_ID | 19) // if (!LT || ZF) { *adr = src; }
#define V0_MCF_OP               (V0_LDSTR_UNIT_ID | 1a) // if (CF) { dest = src; }
#define V0_LCF_OP               (V0_LDSTR_UNIT_ID | 1a) // if (CF) { dest = *adr; }
#define V0_SCF_OP               (V0_LDSTR_UNIT_ID | 1a) // if (CF) { *adr = src; }
#define V0_MNC_OP               (V0_LDSTR_UNIT_ID | 1b) // if (!CF) { dest = src; }
#define V0_LNC_OP               (V0_LDSTR_UNIT_ID | 1b) // if (!CF) { dest = *adr; }
#define V0_SNC_OP               (V0_LDSTR_UNIT_ID | 1b) // if (!CF) { *adr = src; }
#define V0_MOF_OP               (V0_LDSTR_UNIT_ID | 1c) // if (OF) { dest = src; }
#define V0_LOF_OP               (V0_LDSTR_UNIT_ID | 1c) // if (OF) { dest = *adr; }
#define V0_SOF_OP               (V0_LDSTR_UNIT_ID | 1c) // if (OF) { *adr = src; }
#define V0_MNO_OP               (V0_LDSTR_UNIT_ID | 1d) // if (!OF) { dest = src; }
#define V0_LNO_OP               (V0_LDSTR_UNIT_ID | 1d) // if (!OF) { dest = *adr; }
#define V0_SNO_OP               (V0_LDSTR_UNIT_ID | 1d) // if (!OF) { *adr = src; }
#define V0_MNR_OP               (V0_LDSTR_UNIT_ID | 1e) // if (!RF) { dest = src; }
#define V0_LNR_OP               (V0_LDSTR_UNIT_ID | 1e) // if (!RF) { dest = *adr; }
#define V0_SNR_OP               (V0_LDSTR_UNIT_ID | 1e) // if (!RF) { *adr = src; }

#define V0_COPROC_UNIT          0x07

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
/* addressing-mode flags */
#define V0_INS_ADR_BASE_BIT     0x01000000  // *%reg or (%reg)
#define V0_INS_ADR_NDX_BIT      0x02000000  // %reg(%reg)
#define V0_INS_ADR_OFS_BIT      0x04000000  // $x(%reg), $x in imm32
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

