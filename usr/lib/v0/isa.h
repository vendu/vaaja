#ifndef __V0_ISA_H__
#define __V0_ISA_H__

#include <stdint.h>
#include <v0/types.h>

#define v0isnop(ins) (*(int32_t *)ins == INT32_C(0)) // all 0-bits

struct v0romparm {
    m_adr_t     buf;
    m_uword_t   bufsize;
};

/* SYS-unit 0x00; ROMs, initialization, interrupts/signals, shutdown, ... */
/*
 * ftr(ri_bit, r_dest);     	// check for presence of machine features
 * hlt();                   	// halt, wait for unmasked interrupts
 * rst(r_flg, a_state);     	// flg: HIBERNATE, REBOOT
 * sim(ri_op, r_intrmask);  	// op: INTCLR, INTSET, INTADD
 * int(ri_intr);            	// software interrupt
 * rti();                       // return from interrupt
 * dev(ri_id, r_cmd, a_parm);
 * rom(ri_id, r_cmd, a_parm);
 * csp(ri_pol);         		// cache policy; NONE, WRTHRU, or WRBACK
 * bar(r_flg);          		// r_flg: RDBAR, WRBAR, default to both
 * rbr();               		// convenience for bar(RDBAR)
 * wbr();               		// convenience for bar(WRBAR)
 */
#define V0_SYS_UNIT     0x00
#define V0_NOP_OP       0x00    // no operation
#define V0_FTR_OP       0x01    // check machine feature
#define V0_HLT_OP       0x02    // halt, wait for interrupts not in mask
#define V0_RST_OP       0x03    // reset, hibernate, reboot
#define V0_SIM_OP       0x04    // set interrupt mask
#define V0_INT_OP       0x05    // deliver interrupt
#define V0_RTI_OP       0x06    // return from interrupt
#define V0_DEV_OP       0x07    // [bus and] device operations
#define V0_ROM_OP       0x08    // LIST, LOAD, RUN, TERM, ...
#define V0_CSP_OP       0x09    // set cache policy (NONE, WT, WB)
#define V0_CPF_OP       0x0a    // cacheline prefetch
#define V0_BAR_OP       0x0b    // memory barrier; default to full
#define V0_RBR_OP       0x0c    // read-barrier
#define V0_WBR_OP       0x0d    // write-barrier

/* LGSFT-unit 0x01; logic and shift operations */
#define V0_LGSFT_UNIT   0x01
/*
 * not(r_src, r_dest);
 * neg(r_src, r_dest);
 * and(r_src, r_dest);
 * or(r_src, r_dest);
 * xor(r_src, r_dest);
 * nor(r_src, r_dest);
 * xnor(r_src, r_dest);
 * nand(r_src, r_dest);
 * inc(r_arg);
 * dec(r_arg);
 * sll(rv_cnt, r_arg);
 * srl(rv_cnt, r_arg);
 * sar(rv_cnt, r_arg);
 * cmp(ri_arg);
 * ucmp(ri_arg);
 */
#define V0_NOT_OP       0x00    // logical NOT
#define V0_NEG_OP       0x01    // arithmetic negation
#define V0_AND_OP       0x02    // logical AND
#define V0_OR_OP        0x03    // logical OR
#define V0_XOR_OP       0x04    // logical XOR
#define V0_NOR_OP       0x05
#define V0_XNOR_OP      0x06    // logical XNOR
#define V0_NAND_OP      0x07    // logical NAND
#define V0_INC_OP       0x08    // increment by one
#define V0_DEC_OP       0x09    // decrement by one
#define V0_SLL_OP       0x0a    // shift left (logical)
#define V0_SRL_OP       0x0b    // shift right (logical)
#define V0_SAR_OP       0x0c    // shift right (arithmetic)
#define V0_CMP_OP       0x0d    // comparison
#define V0_UCMP_OP      0x0e    // comparison

/* ARITH-unit 0x02; arithmetic operations */
/*
 * add(ri_src, r_dest);
 * addu(ri_src, r_dest);
 * adc(ri_src, r_dest);
 * sub(ri_src, r_dest);
 * sbc(ri_src, r_dest);
 * mul(ri_src, r_dest);
 * umul(ri_src, r_dest);
 * mhi(ri_src, r_dest);
 * umhi(ri_src, r_dest);
 * irp(ri_src, r_dest);
 * uirp(ri_src, r_dest);
 * div(ri_src, r_dest);
 * udiv(ri_src, r_dest);
 * rem(ri_src, r_dest);
 * urem(ri_src, r_dest);
 */
#define V0_ARITH_UNIT   0x02
#define V0_ADD_OP       0x00    // addition
#define V0_ADDU_OP      0x01    // addition (ignore overflow)
#define V0_ADC_OP       0x02    // addition with carry
#define V0_SUB_OP       0x03    // subtraction
#define V0_SBC_OP       0x04    // subtraction with carry/borrow
#define V0_MUL_OP       0x05    // multiplication
#define V0_UMUL_OP      0x06    // unsigned multiplication
#define V0_MHI_OP       0x07    // multiplication, high result word
#define V0_UMHI_OP      0x08    // unsigned multiplication, high result word
#define V0_IRP_OP       0x09    // inverse reciprocal
#define V0_UIRP_OP      0x0a    // unsigned inverse reciprocal
#define V0_DIV_OP       0x0b    // division
#define V0_UDIV_OP      0x0c    // unsigned division
#define V0_REM_OP       0x0d    // remainder
#define V0_UREM_OP      0x0e    // unsigned remainder

/* BRANCH-unit 0x04; branch instructions */
#define V0_BRANCH_UNIT  0x03
#define V0_BLR_OP       0x00    // branch and link (store next PC to LR)
#define V0_BEQ_OP       0x01    // branch if equal (zero-flag set)
#define V0_BNE_OP       0x02    // branch if not equal (zero-flag not set)
#define V0_BLT_OP       0x03    // branch if less than
#define V0_BLE_OP       0x04    // branch if less or equal
#define V0_BGT_OP       0x05    // branch if greater than
#define V0_BGE_OP       0x06    // branch if greater or equal
#define V0_BULT_OP      0x07    // branch if less than, unsigned
#define V0_BULE_OP      0x08    // branch if less or equal, unsigned
#define V0_BUGT_OP      0x09    // branch if greater than, unsigned
#define V0_BUGE_OP      0x0a    // branch if greater or equal, unsigned
#define V0_BCF_OP       0x0b    // branch if carry set
#define V0_BNC_OP       0x0c    // branch if carry not set
#define V0_BOF_OP       0x0d    // branch if overflow set
#define V0_BNO_OP       0x0e    // branch if overflow not set
#define V0_BNR_OP       0x0f    // branch if no-round set (fixed-point, FPU)

/* SUBRT-unit 0x04; subroutine interface */
/*
 * jmp(ri_adr);
 * jpc(ri_ofs);
 * sbr(ri_adr);
 * ret();
 * sme(ri_adr);
 * smx();
 * thr(ri_func, r_arg);
 * thx();
 */
#define V0_SUBRT_UNIT   0x04
#define V0_JMP_OP       0x00    // jmp(adr);
#define V0_JPC_OP       0x01    // jmp(pc + adr);
#define V0_SBR_OP       0x02    // lr = pc; jmp(adr);
#define V0_RET_OP       0x03    // sp += tmp; jmp(lr);
/* MT-extension */
#define V0_SME_OP       0x04    // save(uctx, map);
                                // lr = pc; fp = fp0; sp = sp0;
                                // sp += src;
#define V0_SMX_OP       0x05    // r1 = src;
                                // fp0 = fp; sp0 = sp;
                                // pop(uctx);
#define V0_THR_OP       0x05    // push(uctx); tcb = ctx; thr(dest, adr);
#define V0_THX_OP       0x06    // trap(func, code); load(tcb);

/* BIT-unit 0x05 */
#define V0_BIT_UNIT     0x05
#define V0_ZEX_OP       0x00    // zero-extend from one width to another
#define V0_SEX_OP       0x01    // sign-extend from one width to another
#define V0_CLZ_OP       0x02    // count leading zero-bits
#define V0_HAM_OP       0x03    // bit population/Hamming weight
#define V0_ROL_OP       0x04    // rotate left
#define V0_ROR_OP       0x05    // rotate right
#define V0_RCL_OP       0x06    // rotate left with carry
#define V0_RCR_OP       0x07    // rotate right with carry
#define V0_HSH_OP       0x08    // compute 32-bit hash-value from key
#define V0_HUN_OP       0x09    // reverse 32-bit hash-value back to key
#define V0_BTC_OP       0x0a    // bit test-and-clear
#define V0_BTS_OP       0x0b    // bit test-and-set
#define V0_BSF_OP       0x0c    // bit scan-and-set forward (up)
#define V0_BSR_OP       0x0d    // bit scan-and-set reverse (down)

/* LDSTR-unit 0x06 */
#define V0_MOV_OP       0x00    // register-to-register copy
#define V0_LDR_OP       0x01    // memory-to-register load
#define V0_STR_OP       0x02    // register-to-memory store
#define V0_PSH_OP       0x03    // push register
#define V0_POP_OP       0x04    // pop register
#define V0_STM_OP       0x05    // store many (uses register bitmap)
#define V0_LDM_OP       0x06    // load many (uses register bitmap)
#define V0_MVX_OP       0x07    // user-to-system register copy
#define V0_LDX_OP       0x08    // memory-to-system-register load
#define V0_STX_OP       0x09    // system-register to memory store
#define V0_SMX_OP       0x0a    // store many system registers
#define V0_LMX_OP       0x0b    // load many system registers
#define V0_LNT_OP       0x0c    // non-temporal load (bypass cache)
#define V0_SNT_OP       0x0d    // non-temporal store

/* MT-extension unit 0x07 */
#define V0_MTEXT_UNIT   0x07
#define V0_WFE_OP       0x00    // wait for [write] event
#define V0_SEV_OP       0x01    // signal [write] event
#define V0_IPG_OP       0x02    // invalidate page TLB-entry
#define V0_RAND_OP      0x03    // fetch-and-AND
#define V0_RLOR_OP      0x04    // fetch-and-OR
#define V0_RXOR_OP      0x05    // fetch-and-XOR
#define V0_RINC_OP      0x06    // fetch-and-increment by one
#define V0_RDEC_OP      0x07    // fetch-and-decrement by one
#define V0_RADD_OP      0x08    // fetch-and-add
#define V0_CAS_OP       0x09    // compare-and-swap
#define V0_XCH_OP       0x0a    // atomic exchange
#define V0_BTC_OP		0x0b    // bit test-and-clear
#define V0_BTS_OP		0x0c    // bit test-and-set
#define V0_BSF_OP		0x0d    // bit scan-and-set forward
#define V0_BSR_OP		0x0e    // bit scan-and-set reverse (backwards)

/* CMOVE-unit 0x08; transfer type determined by parm & V0_XFER_DIR_MASK */
#define V0_CMOVE_UNIT   0x08
#define V0_MEQ_OP       0x00    // if (ZF) { dest = src; }
#define V0_LEQ_OP       0x00    // if (ZF) { dest = *adr; }
#define V0_SEQ_OP       0x00    // if (ZF) { *adr = src; }
#define V0_MNE_OP       0x01    // if (!ZF) { dest = src; }
#define V0_LNE_OP       0x01    // if (!ZF) { dest = *adr; }
#define V0_SNE_OP       0x01    // if (!ZF) { *adr = src; }
#define V0_MLT_OP       0x02    // if (LT) { dest = src; }
#define V0_LLT_OP       0x02    // if (LT) { dest = *adr; }
#define V0_SLT_OP       0x02    // if (LT) { *adr = src; }
#define V0_MLE_OP       0x03    // if (LT || ZF) { dest = src; }
#define V0_LLE_OP       0x03    // if (LT || ZF) { dest = *adr; }
#define V0_SLE_OP       0x03    // if (LT || ZF) { *adr = src; }
#define V0_MGT_OP       0x04    // if (!LT && !ZF) { dest = src; }
#define V0_LGT_OP       0x04    // if (!LT && !ZF) { dest = src; }
#define V0_SGT_OP       0x04    // if (!LT && !ZF) { dest = src; }
#define V0_MGE_OP       0x05    // if (!LT || ZF) { dest = src; }
#define V0_LGE_OP       0x05    // if (!LT || ZF) { dest = src; }
#define V0_SGE_OP       0x05    // if (!LT || ZF) { dest = src; }
#define V0_MULT_OP      0x06    // if (LT) { dest = src; }
#define V0_LULT_OP      0x06    // if (LT) { dest = *adr; }
#define V0_SULT_OP      0x06    // if (LT) { dest = src; }
#define V0_MULE_OP      0x07    // if (LT || ZF) { dest = src; }
#define V0_LULE_OP      0x07    // if (LT || ZF) { dest = *adr; }
#define V0_SULE_OP      0x07    // if (LT || ZF) { *adr = src; }
#define V0_MUGT_OP      0x08    // if (!LT && !ZF) { dest = src; }
#define V0_LUGT_OP      0x08    // if (!LT && !ZF) { dest = *adr; }
#define V0_SUGT_OP      0x08    // if (!LT && !ZF) { *adr = src; }
#define V0_MUGE_OP      0x09    // if (!LT || ZF) { dest = src; }
#define V0_LUGE_OP      0x09    // if (!LT || ZF) { dest = *adr; }
#define V0_SUGE_OP      0x09    // if (!LT || ZF) { *adr = src; }
#define V0_MCF_OP       0x0a    // if (CF) { dest = src; }
#define V0_LCF_OP       0x0a    // if (CF) { dest = *adr; }
#define V0_SCF_OP       0x0a    // if (CF) { *adr = src; }
#define V0_MNC_OP       0x0b    // if (!CF) { dest = src; }
#define V0_LNC_OP       0x0b    // if (!CF) { dest = *adr; }
#define V0_SNC_OP       0x0b    // if (!CF) { *adr = src; }
#define V0_MOF_OP       0x0c    // if (OF) { dest = src; }
#define V0_LOF_OP       0x0c    // if (OF) { dest = *adr; }
#define V0_SOF_OP       0x0c    // if (OF) { *adr = src; }
#define V0_MNO_OP       0x0d    // if (!OF) { dest = src; }
#define V0_LNO_OP       0x0d    // if (!OF) { dest = *adr; }
#define V0_SNO_OP       0x0d    // if (!OF) { *adr = src; }
#define V0_MNR_OP       0x0e    // if (!RF) { dest = src; }
#define V0_LNR_OP       0x0e    // if (!RF) { dest = *adr; }
#define V0_SNR_OP       0x0e    // if (!RF) { *adr = src; }

#define V0_FXP_UNIT     0x1b
#define V0_FPU_UNIT     0x1c
#define V0_GPU_UNIT     0x1d
#define V0_DSP_UNIT     0x1e
#define V0_COPROC_UNIT  0x1f

/* indicates presence [possibly-aligned] imm32-argument after opcode */
#define V0_PARM_IMM32_BIT       0x8000
/* 8-bit immediate value */
#define V0_PARM_VAL8_MASK       0x00ff
/* 4-bit extra register ID */
#define V0_PARM_REG_MASK        0x000f
/* addressing-mode flags */
#define V0_PARM_ADR_NDX_BIT     0x0010
#define V0_PARM_ADR_OFS_BIT     0x0020
/* operand-size flags */
#define V0_PARM_OPSIZE_BIT_1    0x0040
#define V0_PARM_OPSIZE_BIT_2    0x0080
/* (extra) register/memory transfer directions */
#define V0_XFER_DIR_MASK        0x7000
#define V0_PARM_RTOX_BIT        0x0010
#define V0_PARM_XTOR_BIT        0x0020
#define V0_PARM_XTOM_BIT        0x0040
#define V0_PARM_MTOX_BIT        0x0080
#define V0_PARM_RTOR_BIT        0x0100
#define V0_PARM_RTOM_BIT        0x0200
#define V0_PARM_MTOR_BIT        0x0400
struct v0ins {
    int8_t  op;                     // 4-bit unit and 4-bit operation ID
    int8_t  regs;                   // source and destination register IDs
    int16_t imm;                    // instruction-dependent operands/parameters
    int32_t imm32[C_VARLENARRAY];   // possible 32-bit immediate operand
};

struct v0uctx {
    m_reg   retadr;
    m_reg   oldfp;
    m_reg   nstkb;
    m_reg   r1_5[5];
    m_reg   rx0_rx14[15];
    m_reg   r12_r15[4];             // FP, SP, PC, LR
    /* 416 bytes reserved for coprocessor use */
};

#endif /* __V0_ISA_H__ */
