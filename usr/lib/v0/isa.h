#ifndef __V0_ISA_H__
#define __V0_ISA_H__

#include <stdint.h>
#include <v0/types.h>

#define v0isnop(ins) (*(int32_t *)ins == INT32_C(0)) // all 0-bits

struct v0romparm {
    m_adr_t     buf;
    m_uword_t   bufsize;
};

/*
 * SYS-unit 0x00; initialization, interrupts/signals, shutdown, atomic ops...
 * function and system-mode calls
 */
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
 * wfe();                       // wait for memory event
 * sev();                       // signal memory event
 * ipg(adr)                     // invalidate page TLB-entry
 * rand(ri_src, r_dest);        // ATOMIC *dest &= src;
 * rlor(ri_src, r_dest);        // ATOMIC *dest |= src;
 * rxor(ri_src, r_dest);        // ATOMIC *dest ^= src;
 * rinc(ri_src, r_dest);        // ATOMIC *src++;
 * rdec(ri_src, r_dest);        // ATOMIC *src--;
 * radd(ri_src, r_dest);        // ATOMIC *dest += src;
 * cas(ri_src, r_dest);         // ATOMIC if (*dest == want) { *dest = val; }
 * xch(ri_src, r_dest);         // ATOMIC *dest |= src;
 * btc(val, ndx);
 * bts(val, ndx);
 * bsf(val, ndx);
 * bsr(val, ndx);
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
#define V0_CSP_OP       0x08    // set cache policy (NONE, WT, WB)
#define V0_CPF_OP       0x09    // cacheline prefetch
#define V0_BAR_OP       0x0a    // memory barrier; default to full
#define V0_RBR_OP       0x0b    // read-barrier
#define V0_WBR_OP       0x0c    // write-barrier
#define V0_WFE_OP       0x0d    // wait for [write] event
#define V0_SEV_OP       0x0e    // signal [write] event
#define V0_IPG_OP       0x1f    // invalidate page TLB-entry
#define V0_RAND_OP      0x10    // atomic fetch-and-AND
#define V0_RLOR_OP      0x11    // atomic fetch-and-OR
#define V0_RXOR_OP      0x12    // atomic fetch-and-XOR
#define V0_RINC_OP      0x13    // atomic fetch-and-increment by one
#define V0_RDEC_OP      0x14    // atomic fetch-and-decrement by one
#define V0_RADD_OP      0x15    // atomic fetch-and-add
#define V0_CAS_OP       0x16    // atomic compare-and-swap
#define V0_XCH_OP       0x17    // atomic exchange
#define V0_BTC_OP       0x18    // atomic bit test-and-clear
#define V0_BTS_OP       0x19    // atomic bit test-and-set
#define V0_BSF_OP       0x1a    // atomic bit scan-and-set forward (up)
#define V0_BSR_OP       0x1b    // atomic bit scan-and-set reverse (down)

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
#define V0_ALU_UNIT     0x01
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
#define V0_ROL_OP       0x0d    // rotate left
#define V0_ROR_OP       0x0e    // rotate right
#define V0_RCL_OP       0x0f    // rotate left with carry
#define V0_RCR_OP       0x10    // rotate right with carry
#define V0_ADD_OP       0x11    // addition
#define V0_ADDU_OP      0x12    // addition (ignore overflow)
#define V0_ADC_OP       0x13    // addition with carry
#define V0_SUB_OP       0x14    // subtraction
#define V0_SBC_OP       0x15    // subtraction with carry/borrow
#define V0_MUL_OP       0x16    // multiplication
#define V0_UMUL_OP      0x17    // unsigned multiplication
#define V0_MHI_OP       0x18    // multiplication, high result word
#define V0_UMHI_OP      0x19    // unsigned multiplication, high result word
#define V0_IRP_OP       0x1a    // inverse reciprocal
#define V0_UIRP_OP      0x1b    // unsigned inverse reciprocal
#define V0_DIV_OP       0x1c    // division
#define V0_UDIV_OP      0x1d    // unsigned division
#define V0_REM_OP       0x1e    // remainder
#define V0_UREM_OP      0x1f    // unsigned remainder

/* SUBRT-unit 0x02; branch instructions, function and system-calls, threads */
/*
 * cmp(ri_val);
 * ucmp(ri_uval);
 * jmp(ri_adr);
 * jpc(ri_ofs);
 * blr(ri_adr);
 * beq(ri_adr);
 * bne(ri_adr);
 * blt(ri_adr);
 * ble(ri_adr);
 * bgt(ri_adr);
 * bge(ri_adr);
 * bult(ri_adr);
 * bule(ri_adr);
 * bugt(ri_adr);
 * buge(ri_adr);
 * bcf(ri_adr);
 * bnc(ri_adr);
 * bof(ri_adr);
 * bno(ri_adr);
 * bnr(ri_adr);
 * sbr(ri_adr);
 * ret(ri_adr);
 * sme(ri_adr);
 * smx();
 * thr(ri_func, r_arg);
 * thx();
 */
#define V0_SUBRT_UNIT   0x02
#define V0_CMP_OP       0x00    // comparison
#define V0_UCMP_OP      0x01    // comparison
#define V0_JMP_OP       0x02    // jmp(adr);
#define V0_JPC_OP       0x03    // jmp(pc + adr);
#define V0_BLR_OP       0x04    // branch and link (store next PC to LR)
#define V0_BEQ_OP       0x05    // branch if equal (zero-flag set)
#define V0_BNE_OP       0x06    // branch if not equal (zero-flag not set)
#define V0_BLT_OP       0x07    // branch if less than
#define V0_BLE_OP       0x08    // branch if less or equal
#define V0_BGT_OP       0x09    // branch if greater than
#define V0_BGE_OP       0x0a    // branch if greater or equal
#define V0_BULT_OP      0x0b    // branch if less than, unsigned
#define V0_BULE_OP      0x0c    // branch if less or equal, unsigned
#define V0_BUGT_OP      0x0d    // branch if greater than, unsigned
#define V0_BUGE_OP      0x0e    // branch if greater or equal, unsigned
#define V0_BCF_OP       0x0f    // branch if carry set
#define V0_BNC_OP       0x10    // branch if carry not set
#define V0_BOF_OP       0x11    // branch if overflow set
#define V0_BNO_OP       0x12    // branch if overflow not set
#define V0_BNR_OP       0x13    // branch if no-round set (fixed-point, FPU)
#define V0_SBR_OP       0x14    // lr = pc; jmp(adr);
#define V0_RET_OP       0x15    // sp += tmp; jmp(lr);
/* MT-extension */
#define V0_SME_OP       0x16    // save(uctx, map);
                                // lr = pc; fp = fp0; sp = sp0;
                                // sp += src;
#define V0_SMX_OP       0x17    // r1 = src;
                                // fp0 = fp; sp0 = sp;
                                // pop(uctx);
#define V0_THR_OP       0x18    // push(uctx); tcb = ctx; thr(dest, adr);
#define V0_THX_OP       0x19    // trap(func, code); load(tcb);

/* BIT-unit 0x03 */
/*
 * zex(sz, val);
 * sex(sex, val);
 * clz(val);
 * ham(val);
 * hsh(val);
 * hun(val);
 */
#define V0_BIT_UNIT     0x03
#define V0_ZEX_OP       0x00    // zero-extend from given width to 32-bit
#define V0_SEX_OP       0x01    // sign-extend from given width to 32-bit
#define V0_CLZ_OP       0x02    // count leading zero-bits
#define V0_HAM_OP       0x03    // bit population/Hamming weight/count of 1-bits
#define V0_PAR_OP       0x04    // compute bit-parity; 0 for even # of 1-bits
#define V0_HSH_OP       0x04    // compute 32-bit hash-value from key
#define V0_HUN_OP       0x05    // reverse 32-bit hash-value back to key

/* LDSTR-unit 0x04 */
#define V0_LDSTR_UNIT   0x04
#define V0_LDR_OP       0x00    // memory-to-register load
#define V0_STR_OP       0x01    // register-to-memory store
#define V0_PSH_OP       0x02    // push register
#define V0_POP_OP       0x03    // pop register
#define V0_STM_OP       0x04    // store many (uses register bitmap)
#define V0_LDM_OP       0x05    // load many (uses register bitmap)
#define V0_MVX_OP       0x06    // user-to-system register copy
#define V0_LDX_OP       0x07    // memory-to-system-register load
#define V0_STX_OP       0x08    // system-register to memory store
#define V0_SMX_OP       0x09    // store many system registers
#define V0_LMX_OP       0x0a    // load many system registers
#define V0_LNT_OP       0x0b    // non-temporal load (bypass cache)
#define V0_SNT_OP       0x0c    // non-temporal store
/* opcodes 0x0d-0x0f are reserved for future use */
/* CMOVE-extension; transfer type determined by parm & V0_XFER_DIR_MASK */
#define V0_MEQ_OP       0x10    // if (ZF) { dest = src; }
#define V0_LEQ_OP       0x10    // if (ZF) { dest = *adr; }
#define V0_SEQ_OP       0x10    // if (ZF) { *adr = src; }
#define V0_MNE_OP       0x11    // if (!ZF) { dest = src; }
#define V0_LNE_OP       0x11    // if (!ZF) { dest = *adr; }
#define V0_SNE_OP       0x11    // if (!ZF) { *adr = src; }
#define V0_MLT_OP       0x12    // if (LT) { dest = src; }
#define V0_LLT_OP       0x12    // if (LT) { dest = *adr; }
#define V0_SLT_OP       0x12    // if (LT) { *adr = src; }
#define V0_MLE_OP       0x13    // if (LT || ZF) { dest = src; }
#define V0_LLE_OP       0x13    // if (LT || ZF) { dest = *adr; }
#define V0_SLE_OP       0x13    // if (LT || ZF) { *adr = src; }
#define V0_MGT_OP       0x14    // if (!LT && !ZF) { dest = src; }
#define V0_LGT_OP       0x14    // if (!LT && !ZF) { dest = src; }
#define V0_SGT_OP       0x14    // if (!LT && !ZF) { dest = src; }
#define V0_MGE_OP       0x15    // if (!LT || ZF) { dest = src; }
#define V0_LGE_OP       0x15    // if (!LT || ZF) { dest = src; }
#define V0_SGE_OP       0x15    // if (!LT || ZF) { dest = src; }
#define V0_MULT_OP      0x16    // if (LT) { dest = src; }
#define V0_LULT_OP      0x16    // if (LT) { dest = *adr; }
#define V0_SULT_OP      0x16    // if (LT) { dest = src; }
#define V0_MULE_OP      0x17    // if (LT || ZF) { dest = src; }
#define V0_LULE_OP      0x17    // if (LT || ZF) { dest = *adr; }
#define V0_SULE_OP      0x17    // if (LT || ZF) { *adr = src; }
#define V0_MUGT_OP      0x18    // if (!LT && !ZF) { dest = src; }
#define V0_LUGT_OP      0x18    // if (!LT && !ZF) { dest = *adr; }
#define V0_SUGT_OP      0x18    // if (!LT && !ZF) { *adr = src; }
#define V0_MUGE_OP      0x19    // if (!LT || ZF) { dest = src; }
#define V0_LUGE_OP      0x19    // if (!LT || ZF) { dest = *adr; }
#define V0_SUGE_OP      0x19    // if (!LT || ZF) { *adr = src; }
#define V0_MCF_OP       0x1a    // if (CF) { dest = src; }
#define V0_LCF_OP       0x1a    // if (CF) { dest = *adr; }
#define V0_SCF_OP       0x1a    // if (CF) { *adr = src; }
#define V0_MNC_OP       0x1b    // if (!CF) { dest = src; }
#define V0_LNC_OP       0x1b    // if (!CF) { dest = *adr; }
#define V0_SNC_OP       0x1b    // if (!CF) { *adr = src; }
#define V0_MOF_OP       0x1c    // if (OF) { dest = src; }
#define V0_LOF_OP       0x1c    // if (OF) { dest = *adr; }
#define V0_SOF_OP       0x1c    // if (OF) { *adr = src; }
#define V0_MNO_OP       0x1d    // if (!OF) { dest = src; }
#define V0_LNO_OP       0x1d    // if (!OF) { dest = *adr; }
#define V0_SNO_OP       0x1d    // if (!OF) { *adr = src; }
#define V0_MNR_OP       0x1e    // if (!RF) { dest = src; }
#define V0_LNR_OP       0x1e    // if (!RF) { dest = *adr; }
#define V0_SNR_OP       0x1e    // if (!RF) { *adr = src; }

#define V0_COPROC_UNIT  0x07

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
#define V0_XFER_DIR_MASK        0x7f00
#define V0_PARM_RTOR_BIT        0x0100
#define V0_PARM_RTOM_BIT        0x0200
#define V0_PARM_RTOX_BIT        0x0400
#define V0_PARM_XTOR_BIT        0x0800
#define V0_PARM_XTOM_BIT        0x1000
#define V0_PARM_MTOX_BIT        0x2000
#define V0_PARM_MTOR_BIT        0x4000
struct v0ins {
    int8_t  op;                     // 3-bit unit and 5-bit operation ID
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

