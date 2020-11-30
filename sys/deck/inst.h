#ifndef DECK_INST_H
#define DECK_INST_H

/* NOP is a 32-bit parcel of all 1-bits */

#define DECK_NOP                (~UINT32_C(0))

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
#define DECK_ADD_OP             0x0b        // dest = src + arg;
#define DECK_UADD_OP            DECK_ADD_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_SUB_OP             0x0c        // dest = src - arg;
#define DECK_USUB_OP            DECK_SUB_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_MUL_OP             0x0d        // dest = src * arg;
#define DECK_UMUL_OP            DECK_MUL_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_MHI_OP             0x0e        // dest = (src * arg) >> 32;
#define DECK_UMHI_OP            DECK_MHI_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_DIV_OP             0x0f        // dest = src / arg;
#define DECK_UDIV_OP            DECK_DIV_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_REM_OP             0x10        // dest = src % arg;
#define DECK_UREM_OP            DECK_REM_OP // DECK_UNSIGNED_BIT set in parm
/*
 * IOP  src=(CLR|SET|CHK), arg=port, dest=perm
 * IOC  src=cmd, arg=port, dest=parm
 * IOR  src=port, arg=nb, dest=buf
 * IOW  src=buf, arg=nb, dest=port
 */
#define DECK_IOC_OP             0x11        // I/O commands
#define DECK_IOR_OP             0x12        // I/O-port reads
#define DECK_IOW_OP             0x13        // I/O-port writes
#define DECK_CMP_OP             0x14        // I/O-port writes
#define DECK_UCMP_OP            DECK_CMP_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_JMP_OP             0x15        // jump unconditionally
#define DECK_JCF_OP             0x16        // jump if CF-bit set in MSW
#define DECK_JOF_OP             0x17        // jump if OF-bit set in MSW
#define DECK_JNR_OP             0x18        // jump if RF-bit not set in MSW
#define DECK_JEQ_OP             0x19        // jump if ZF-bit set in MSW
#define DECK_JLT_OP             0x1a        // jump if LT-bit set in MSW
#define DECK_JGT_OP             0x1b        // jump if GT-bit set in MSW
#define DECK_SBR_OP             0x1c        // call subroutine
#define DECK_RET_OP             0x1d        // return from subroutine
#define DECK_CTZ_OP             0x1e        // count # of low 0-bits
#define DECK_HAM_OP             0x1f        // count 1-bits (Hamming weight)
#define DECK_OP_BITS            5           // # of bits instruction ID

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

#define DECK_OFS_BITS           27          // # of bits in PC-relative jump-ofs
struct deckjmpop {
    unsigned                    id  : DECK_OP_BITS;
    unsigned                    ofs : DECK_OFS_BITS;
};

/* parm-field flags */
#define DECK_UNSIGNED_BIT       (1 << 7)    // perform unsigned operation
#define DECK_MSW_BIT            (1 << 6)    // set relevant bits in MSW
struct deckop {
    unsigned                    id      : DECK_OP_BITS;
    unsigned                    src     : DECK_REG_BITS;
    unsigned                    arg     : DECK_REG_BITS;
    unsigned                    dest    : DECK_REG_BITS;
    unsigned                    adr     : DECK_ADR_BITS;
    unsigned                    cond    : DECK_COND_BITS;
    unsigned                    fold    : DECK_FOLD_BITS;
    uint8_t                     parm;
    int32_t                     imm32[C_VLA];
}

#endif /* DECK_INST_H */

