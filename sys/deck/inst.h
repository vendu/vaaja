#ifndef DECK_INST_H
#define DECK_INST_H

#define DECK_MP_EXTENSION       1

/* NOP is a 32-bit parcel of all 1-bits */

#define DECK_NOP                (~UINT32_C(0))

/*
 * NOTES
 * -----
 * - NOT, ORR, XOR, AND, INC, DEC, ADD, SUB may be used with the ATOMIC-bit set
 */
#define DECK_NOT_OP             0x00        // dest = ~src;
#define DECK_ORR_OP             0x01        // dest = src | arg;
#define DECK_XOR_OP             0x02        // dest = src ^ arg;
#define DECK_AND_OP             0x03        // dest = src & arg;
#define DECK_TST_OP             0x04        // dest = src & (1 << parm);
#define DECK_SET_OP             0x05        // dest |= 1 << parm;
#define DECK_CLR_OP             0x06        // dest &= ~(1 << parm);
#define DECK_FLP_OP             0x07        // dest ^= 1 << parm;
#define DECK_SHL_OP             0x08        // dest = src << (parm|arg);
#define DECK_SHR_OP             0x09        // dest = src >> (parm|arg);
#define DECK_SAR_OP             0x0a        // dest = src >>> (parm|arg);
#define DECK_INC_OP             0x0b        // dest = src + 1;
#define DECK_UINC_OP            DEC_INC_OP  // DECK_UNSIGNED_BIT set in parm
#define DECK_DEC_OP             0x0c        // dest = src - 1;
#define DECK_UDEC_OP            DECK_DEC_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_ADD_OP             0x0d        // dest = src + arg;
#define DECK_UADD_OP            DECK_ADD_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_SUB_OP             0x0e        // dest = src - arg;
#define DECK_USUB_OP            DECK_SUB_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_MUL_OP             0x0f        // dest = src * arg;
#define DECK_UMUL_OP            DECK_MUL_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_MHI_OP             0x10        // dest = (src * arg) >> 32;
#define DECK_UMHI_OP            DECK_MHI_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_DIV_OP             0x11        // dest = src / arg;
#define DECK_UDIV_OP            DECK_DIV_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_REM_OP             0x12        // dest = src % arg;
#define DECK_UREM_OP            DECK_REM_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_STR_OP             0x13        // *adr = src;
#define DECK_LDR_OP             0x14        // dest = *adr;
#define DECK_CPY_OP             0x15        // dest = arc;
#define DECK_PSH_OP             0x16        // sp -= 4; *sp = src;
#define DECK_POP_OP             0x17        // dest = *sp; sp += 4;
#define DECK_STM_OP             0x18
#define DECK_LDM_OP             0x19
/*
 * IOP  src=(CLR|SET|CHK), arg=port, dest=perm
 * IOC  src=cmd, arg=port, dest=parm
 * IOR  src=port, arg=nb, dest=buf
 * IOW  src=buf, arg=nb, dest=port
 */
#define DECK_IOC_OP             0x1a        // I/O commands
#define DECK_IOR_OP             0x1b        // I/O-port reads
#define DECK_IOW_OP             0x1c        // I/O-port writes
#define DECK_CMP_OP             0x1d        // I/O-port writes
#define DECK_UCMP_OP            DECK_CMP_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_JMP_OP             0x1e        // jump unconditionally
#define DECK_JCF_OP             0x1f        // jump if CF-bit set in MSW
#define DECK_JOF_OP             0x20        // jump if OF-bit set in MSW
#define DECK_JNR_OP             0x21        // jump if RF-bit not set in MSW
#define DECK_JEQ_OP             0x22        // jump if ZF-bit set in MSW
#define DECK_JLT_OP             0x23        // jump if LT-bit set in MSW
#define DECK_JGT_OP             0x24        // jump if GT-bit set in MSW
#define DECK_SBR_OP             0x25        // call subroutine
#define DECK_REP_OP             0x26        // jump to label if CNT > 0; loop
#define DECK_RET_OP             0x27        // return from subroutine
#define DECK_CTZ_OP             0x28        // count # of low 0-bits
#define DECK_CLZ_OP             0x29        // count # of high 0-bits
#define DECK_HAM_OP             0x2a        // count 1-bits (Hamming weight)
#define DECK_HSH_OP             0x2b        // compute hash value for key
#define DECK_HUN_OP             0x2c        // reverse hash value to key
#define DECK_SYS_OP             0x2d        // system-call interface
#define DECK_HLT_OP             0x2e        // halt to wait for interrupts
#define DECK_CLI_OP             0x2f        // disable interrupts; default all
#define DECK_STI_OP             0x30        // enable interrupts; default all
#define DECK_INT_OP             0x31        // processor ID in parm, -1 for self

#if defined(DECK_MT_EXTENSION)

#define DECK_BAR_OP             0x32        // parm: FDB=0, RDB=1, WRB=2
/*
 * WFE and WFI wait for
 * --------------------
 * - IRQ unless masked
 * - FIQ unless masked
 * - a debug request
 *
 * WFE also waits for
 * ------------------
 * - event signaled by another processor using SEV
 */
#define DECK_WFE_OP             0x33        // wait for event; INTR, SEV
#define DECK_WFI_OP             DECK_WFE_OP // WFE-bit set
#define DECK_AORR_OP            DECK_ORR_OP // ATOMIC, *adr |= src;
#define DECK_AXOR_OP            DECK_XOR_OP // ATOMIC, *adr ^= src;
#define DECK_AAND_OP            DECK_AND_OP // ATOMIC, *adr &= src;
#define DECK_AINC_OP            DECK_INC_OP // ATOMIC, dest = *adr++;
#define DECK_ADEC_OP            DECK_DEC_OP // ATOMIC, dest = *adr--;
#define DECK_AADD_OP            DECK_ADD_OP // ATOMIC, ret = *adr, *adr += src;
#define DECK_ASUB_OP            DECK_DEC_OP // ATOMIC, ret = *adr, *adr -= src;
#define DECK_SEV_OP             0x34        // signal event (IRQ/TRAP, MEM)
/* ATOMIC, dest = *adr & (1 << src), *adr |= 1 << src; */
#define DECK_BTS_OP             0x35        // test-and-set bit
/* ATOMIC, dest = *adr & (1 << src), *adr &= ~(1 << src); */
#define DECK_BTC_OP             0x36        // test-and-clear bit
/* ATOMIC, dest = *adr & (1 << src), *adr ^= 1 << src; */
#define DECK_BTF_OP             0x37        // test-and-flip bit
#define DECK_CAS_OP             0x38
#define DECK_MTR_OP             0x39        // memory type range register access
#define DECK_FCL_OP             0x3a        // flush cache up level (all)
#define DECK_IPG_OP             0x3b        // invalidate TLB-entry for page

#endif /* defined(DECK_MT_EXTENSION)

#define DECK_OP_BITS            6           // # of bits instruction ID

/*
 * adr-field
 *
 * LDR: adr=src, ndx=arg, ofs=imm
 * STR: adr=dest, ndx=arg, ofs=imm
 */
#define DECK_NDX_BIT            (1 << 0)
#define DECK_IMM_BIT            (1 << 1)
#define DECK_ADR_BITS           2

#define DECK_ANY_COND           0x00        // always execute
#defune DECK_EQ_COND            0x01        // MSW & ZF
#define DECK_LT_COND            0x02        // MSW & LT
#defune DECK_GT_COND            0x03        // MSW & GT
#define DECK_CF_COND            0x04        // MSW & CF
#define DECK_OF_COND            0x05        // MSW & OF
#define DECK_RF_COND            0x06        // MSW & RF
#define DECK_NO_COND            0x07        // never execute
#define DECK_COND_BITS          3           // # of bits to represent conditions

/* the shift counts for folded instructions are stored in the parm-field */
#define DECK_NO_FOLD            0x00        // no fold-instruction
#define DECK_FOLD_SHL           0x01        // fold left shift
#define DECK_FOLD_SHR           0x02        // fold logical right shift
#define DECK_FOLD_SAR           0x03        // fold arithmetic right shift
#define DECK_FOLD_BITS          2           // # of bits for fold-instruction ID

#define DECK_OFS_BITS           26          // # of bits in PC-relative jump-ofs
struct deckjmpop {
    unsigned                    id      : DECK_OP_BITS;     // [5:0]
    unsigned                    ofs     : DECK_OFS_BITS;    // [31:6]
};

/* parm-field flags */
#define DECK_SHIFT_BITS         5           // # of bits in shift counts
#define DECK_SIZE_BITS          2           // 0 - 8-bit, 1 - 16, 2 - 32, 3 - 64
#define DECK_FXP_BIT            (1 << 4)    // fixed-point operation
#define DECK_ATOMIC_BIT         (1 << 5)    // atomic [memory] operation
#define DECK_WFE_BIT            (1 << 5)    // WFE-instruction (WFI-modifier)
#define DECK_UNSIGNED_BIT       (1 << 6)    // perform unsigned operation
struct deckop {
    unsigned                    id      : DECK_OP_BITS;     // [5:0]    (6)
    unsigned                    src     : DECK_REG_BITS;    // [9:6]    (4)
    unsigned                    arg     : DECK_REG_BITS;    // [12:9]   (4)
    unsigned                    dest    : DECK_REG_BITS;    // [16:13]  (4)
    unsigned                    adr     : DECK_ADR_BITS;    // [18:17]  (2)
    unsigned                    fold    : DECK_FOLD_BITS;   // [20:19]  (2)
    unsigned                    cond    : DECK_COND_BITS;   // [23:21]  (3)
    unsigned                    parm    : 7;
    int32_t                     imm32[C_VLA];
}

struct deckmanyop {
    unsigned                    id      : DECK_OP_BITS; // STM or LDM
    unsigned                    map     : DECK_USER_REGS;
    unsigned                    cond    : DECK_COND_BITS;
    unsigned                    parm    : 7;
};

#endif /* DECK_INST_H */

