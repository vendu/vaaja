#ifndef DECK_INST_H
#define DECK_INST_H

#define DECK_MP_EXTENSION       1

/*
 * NOTES
 * -----
 * - ORR, XOR, AND, INC, DEC, ADD, SUB may be used with the ATOMIC-bit set; see
 *   AORR etc. under DECK_MP_EXTENSION
 */

/* NOP is a 32-bit parcel of all 1-bits */
#define DECK_NOP                (~UINT32_C(0))

/* logical operations */
#define DECK_NOT_OP             0x00        // dest = ~src;
#define DECK_ORR_OP             0x01        // dest = src | arg;
#define DECK_XOR_OP             0x02        // dest = src ^ arg;
#define DECK_AND_OP             0x03        // dest = src & arg;

/* bit-operations */
#define DECK_TST_OP             0x04        // dest = src & (1 << parm);
#define DECK_SET_OP             0x05        // dest |= 1 << parm;
#define DECK_CLR_OP             0x06        // dest &= ~(1 << parm);
#define DECK_FLP_OP             0x07        // dest ^= 1 << parm;

/* shift operations */
#define DECK_SHL_OP             0x0b        // dest = src << (parm|arg);
#define DECK_SHR_OP             0x0c        // dest = src >> (parm|arg);
/* DECK_SEX_BIT in parm is set for SAR */
#define DECK_SAR_OP             DECK_SHR_OP // dest = src >>> (parm|arg);

/* arithmetic operations */
/* the U-prefix unsigned operations have DECK_UNSIGNED_BIT set in parm   MSW */
#define DECK_INC_OP             0x0d        // dest = src + 1;           C
#define DECK_UINC_OP            DEC_INC_OP
#define DECK_DEC_OP             0x0e        // dest = src - 1;           C
#define DECK_UDEC_OP            DECK_DEC_OP
#define DECK_ADD_OP             0x0f        // dest = src + arg;         C, O
#define DECK_UADD_OP            DECK_ADD_OP
#define DECK_SUB_OP             0x10        // dest = src - arg;         C, O
#define DECK_USUB_OP            DECK_SUB_OP
#define DECK_MUL_OP             0x11        // dest = src * arg;         C, O
#define DECK_UMUL_OP            DECK_MUL_OP
#define DECK_MHI_OP             0x12        // dest = (src * arg) >> 32; C, O
#define DECK_UMHI_OP            DECK_MHI_OP
/* DIV, UDIV, REM, UREM, IRP, URP trigger DECK_MATH_TRAP if the divisor is 0 */
#define DECK_DIV_OP             0x13        // dest = src / arg;
#define DECK_UDIV_OP            DECK_DIV_OP
#define DECK_REM_OP             0x14        // dest = src % arg;
#define DECK_UREM_OP            DECK_REM_OP
#define DECK_IRP_OP             0x15
#define DECK_URP_OP             DECK_IRP_OP
#define DECK_HSH_OP             0x16        // compute hash value for key
#define DECK_HUN_OP             0x17        // reverse hash value to key

/* control operations */
#define DECK_CMP_OP             0x18        // I/O-port writes
#define DECK_UCMP_OP            DECK_CMP_OP // DECK_UNSIGNED_BIT set in parm
#define DECK_JMP_OP             0x19        // jump, cond=DECK_ANY_COND
#define DECK_JEQ_OP             DECK_JMP_OP // jump, cond=DECK_EQ_COND
#define DECK_JLT_OP             DECK_JMP_OP // jump, cond=DECK_LT_COND
#define DECK_JGT_OP             DECK_JMP_OP // jump, cond=DECK_GT_COND
#define DECK_JCF_OP             DECK_JMP_OP // jump, cond=DECK_CF_COND
#define DECK_JOF_OP             DECK_JMP_OP // jump, cond=DECK_OF_COND
#define DECK_JNR_OP             DECK_JMP_OP // jump, cond=DECK_NR_COND
#define DECK_SBR_OP             0x1a        // call subroutine
#define DECK_RET_OP             0x1b        // return from subroutine
#define DECK_REP_OP             0x1c        // jump to label if CNT > 0; loop

/* bit-count operations */
#define DECK_CTZ_OP             0x08        // count # of low 0-bits
#define DECK_CLZ_OP             0x09        // count # of high 0-bits
#define DECK_HAM_OP             0x0a        // count 1-bits (Hamming weight)

/*
 * memory operations
 * - low bits in parm specify source operand size
 * - DECK_SEX_BIT dictates zero/sign-extension
 */
#define DECK_STR_OP             0x1d        // *adr = src;
#define DECK_LDR_OP             0x1e        // dest = *adr;
#define DECK_MOV_OP             0x1f        // dest = src;
#define DECK_LEA_OP             0x20
#define DECK_PSH_OP             0x21        // sp -= 4; *sp = src;
#define DECK_POP_OP             0x22        // dest = *sp; sp += 4;
#define DECK_STK_OP             0x23
#define DECK_STM_OP             DECK_STK_OP // push multiple registers; write=1
#define DECK_LDM_OP             DECK_STK_OP // pop multiple registers; write=0

/* I/O operations */
#define DECK_IOP_OP             0x24        // permission control
#define DECK_IOC_OP             0x25        // I/O commands
#define DECK_IOR_OP             0x26        // I/O-port reads
#define DECK_IOW_OP             0x27        // I/O-port writes

/* system operations */
#define DECK_SYS_OP             0x2a        // start system call
#define DECK_SRT_OP             0x29        // return from system call
#define DECK_HLT_OP             0x2a        // halt to wait for interrupts
#define DECK_CLI_OP             0x2b        // disable interrupts; default all
#define DECK_STI_OP             0x2c        // enable interrupts; default all
#define DECK_INT_OP             0x2d        // processor ID in parm, -1 for self
#define DECK_IRT_OP             0x2e        // return from interrupt handler
#define DECK_MTR_OP             0x2f        // memory type range register access

#if defined(DECK_MM_EXTENSION)

/* memory management operations */
#define DECK_FCL_OP             0x30        // flush cache up level (all)
#define DECK_IPG_OP             0x32        // invalidate TLB-entry for page

#endif

#if defined(DECK_MP_EXTENSION)
/* multiprocessor support operations */

/* atomic arithmetic-logical operations */
/* ATOMIC, *adr |= src; */
#define DECK_AORR_OP            DECK_ORR_OP
/* ATOMIC, *adr ^= src; */
#define DECK_AXOR_OP            DECK_XOR_OP
/* ATOMIC, *adr &= src; */
#define DECK_AAND_OP            DECK_AND_OP
/* ATOMIC, dest = *adr++; */
#define DECK_AINC_OP            DECK_INC_OP
/* ATOMIC, dest = *adr--; */
#define DECK_ADEC_OP            DECK_DEC_OP
/* ATOMIC, ret = *adr, *adr += src; */
#define DECK_AADD_OP            DECK_ADD_OP
/* ATOMIC, ret = *adr, *adr -= src; */
#define DECK_ASUB_OP            DECK_DEC_OP

/* memory barriers */
#define DECK_BAR_OP             0x33
#define DECK_BDT_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_DATA
#define DECK_BRD_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_READ
#define DECK_BWR_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_WRITE
/* parm-field values for DECK_BAR_OP */
#define DECK_BAR_DATA           0x00
#define DECK_BAR_READ           0x01
#define DECK_BAR_WRITE          0x02

#define DECK_BIT_OP             0x34
/* ATOMIC, dest = *adr & (1 << src); *adr |= 1 << src; */
#define DECK_BTS_OP             DECK_BIT_OP // test-and-set bit
/* ATOMIC, dest = *adr & (1 << src); *adr &= ~(1 << src); */
#define DECK_BTC_OP             DECK_BIT_OP // test-and-clear bit
/* ATOMIC, dest = *adr & (1 << src); *adr ^= 1 << src; */
#define DECK_BTF_OP             DECK_BIT_OP // test-and-flip bit
/* ATOMIC, ret = *adr; if (ret == arg) *adr = src; dest = ret - arg; */
#define DECK_CAS_OP             0x35        // compare-and-swap
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
#define DECK_SIG_OP             0x33
#define DECK_SEV_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_SEV
#define DECK_WFI_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_WFI
#define DECK_WFE_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_WFE
/* parm values for DECK_SIG_OP */
#define DECK_SIG_SEV            0x00
#define DECK_SIG_WFI            0x01
#define DECK_SIG_WFE            0x02

#endif /* defined(DECK_MP_EXTENSION) */

#define DECK_COPROC_OP          0x3f        // coprocessor-defined instructions
#define DECK_OP_BITS            6           // # of instruction-ID bits

/*
 * adr-field; address-mode bits
 *
 * LDR: adr=src, ndx=arg, ofs=imm
 * STR: adr=dest, ndx=arg, ofs=imm
 */
#define DECK_IMM_BIT            (1 << 0)    // denotes argument in imm32
#define DECK_BASE_BIT           (1 << 1)    // denotes base-address in src/dest
#define DECK_NDX_BIT            (1 << 2)    // denotes address-index in arg
#define DECK_ADR_BITS           3           // number of address-mode bits

/*
 * cond-field; conditions for execution
 */
#define DECK_ANY_COND           0x00        // always execute
#defune DECK_EQ_COND            0x01        // MSW & ZF
#define DECK_LT_COND            0x02        // MSW & LT
#defune DECK_GT_COND            0x03        // MSW & GT
#define DECK_CF_COND            0x04        // MSW & CF
#define DECK_OF_COND            0x05        // MSW & OF
#define DECK_NR_COND            0x06        // !(MSW & RF)
#define DECK_NO_COND            0x07        // never execute
#define DECK_COND_BITS          3           // # of bits to represent conditions

/* the shift counts for folded instructions are stored in the parm-field */
#define DECK_NO_FOLD            0x00        // no fold-instruction
#define DECK_FOLD_SHL           0x01        // fold left shift
#define DECK_FOLD_SHR           0x02        // fold logical right shift
#define DECK_FOLD_SAR           0x03        // fold arithmetic right shift
#define DECK_FOLD_BITS          2           // # of fold-instruction ID-bits

/*
 * NOTE: the jump offsets are in 32-bit words to save space and meet alignment
 */
#define DECK_OFS_BITS           23          // PC-relative word-offset bits
struct deckjmpinst {
    unsigned                    id      : DECK_OP_BITS;     // [5:0]
    unsigned                    cond    : DECK_COND_BITS;   // [8:6]
    unsigned                    ofs     : DECK_OFS_BITS;    // [31:9]
};

#if (DECK_OP_BITS + DECK_COND_BITS + DECK_OFS_BITS != DECK_PARCEL_BITS)
#   error sizeof(struct deckjmpinst) != 32 bits in <deck/inst.h>
#endif

/*
 * - STM, LDM
 * - the map-field have 1-bit per register to list the pushed/popped registers;
 *   the lowest bit is R0, the highest (#14) is MSW; MF is static
 */
#define DECK_MAP_BITS           (DECK_TOTAL_REGS - 1)
struct deckmapinst {
    unsigned                    id      : DECK_OP_BITS;     // STM or LDM
    unsigned                    cond    : DECK_COND_BITS;   // condition
    unsigned                    parm    : 7;                // reserved
    unsigned                    map     : DECK_MAP_BITS; // reg-bitmap
    unsigned                    store   : 1;                   // 0 -> LDM
};

#if (DECK_OP_BITS + DECK_COND_BITS + 7 + DECK_TOTAL_REGS != DECK_PARCEL_BITS)
#   error sizeof(struct deckmapinst) != 32 bits in <deck/inst.h>
#endif

/* this structure is designed to have room for 16-bit immediate data */
struct deckioinst {
    unsigned                    id      : DECK_OP_BITS;      // IOC, IOR, IOW
    unsigned                    reg     : DECK_GEN_REG_BITS; // in/out register
    unsigned                    creg    : DECK_GEN_REG_BITS; // command register
    unsigned                    cond    : DECK_COND_BITS;    // condition
    unsigned                    cmd     : 1;                 // 1: imm has cmd
    unsigned                    port    : DECK_PORT_BITS;    // [12-bit] port ID
    unsigned                    imm     : 4;
};

/* parm-field flags */
#define DECK_WFE_BIT            (1 << 1)    // WFE-instruction (WFI-modifier)
#define DECK_SIZE_BITS          2           // 0 - 8-bit, 1 - 16, 2 - 32, 3 - 64
#define DECK_SEX_BIT            (1 << 2)    // 0: zero-extend, 1: sign-extend
#define DECK_SHIFT_BITS         5           // # of bits in shift counts
#define DECK_ATOMIC_BIT         (1 << 6)    // atomic [memory] operation
#define DECK_UNSIGNED_BIT       (1 << 7)    // perform unsigned operation
struct deckinst {
    unsigned                    id      : DECK_OP_BITS;         // [5:0]    (6)
    unsigned                    cond    : DECK_COND_BITS;       // [8:6]    (3)
    unsigned                    src     : DECK_GEN_REG_BITS;    // [11:9]   (3)
    unsigned                    dest    : DECK_REG_BITS;        // [15:12]  (4)
    unsigned                    arg     : DECK_GEN_REG_BITS;    // [18:16]  (3)
    unsigned                    adr     : DECK_ADR_BITS;        // [21:19]  (3)
    unsigned                    fold    : DECK_FOLD_BITS;       // [23:22]  (2)
    unsigned                    parm    : 8;                    // [31:23]  (8)
    int32_t                     imm32[C_VLA];
}

#endif /* DECK_INST_H */

