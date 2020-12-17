#ifndef DECK_INST_H
#define DECK_INST_H

/*
 * NOTES
 * -----
 * - ORR, XOR, AND, INC, DEC, ADD, SUB may be used with the ATOMIC-bit set; see
 *   AORR etc. under DECK_MP_EXTENSION
 */

/* instructions are 32-bit */
#define DECK_INST_BITS          32
/* NOP is a 32-bit parcel of all 1-bits */
#define DECK_NOP                (~UINT32_C(0))

/*
 * logical operations; struct deckinst
 * - DECK_NOT_OP:   parm=DECK_NOT
 * - DECK_ORR_OP:   parm=DECK_ORR
 * - DECK_XOR_OP:   parm=DECK_XOR
 * - DECK_AND_OP:   parm=DECK_AND
 * - DECK_NOR_OP:   parm=DECK_NOR
 * - DECK_XNOR_OP:  parm=DECK_XNOR
 * - DECK_NAND_OP:  parm=DECK_NAND
 */
#define DECK_LGF_OP             0x00
/* 0x07 is reserved */
#define DECK_NOT_OP             DECK_LGF_OP // dest = ~src;
#define DECK_ORR_OP             DECK_LGF_OP // dest = src | arg;
#define DECK_XOR_OP             DECK_LGF_OP // dest = src ^ arg;
#define DECK_AND_OP             DECK_LGF_OP // dest = src & arg;
#define DECK_NOR_OP             DECK_LGF_OP // dest = src ~| arg;
#define DECK_XNOR_OP            DECK_LGF_OP // dest = src ^~ arg;
#define DECK_NAND_OP            DECK_LGF_OP // dest = src ~& arg;
/* parm-field bits for DECK_LGF_OP */
#define DECK_NOT                0x00
#define DECK_ORR                0x01
#define DECK_XOR                0x02
#define DECK_AND                0x03
#define DECK_NOR                0x04
#define DECK_XNOR               0x05
#define DECK_NAND               0X06

/*
 * bit-operations; struct deckinst
 * - DECK_TST_OP:   parm=DECK_TST
 * - DECK_SET_OP:   parm=DECK_SET
 * - DECK_CLR_OP:   parm=DECK_CLR
 * - DECK_FLP_OP:   parm=DECK_FLP
 */
#define DECK_BIT_OP             0x01
#define DECK_TST_OP             DECK_BIT_OP // dest = src & (1 << arg);
#define DECK_SET_OP             DECK_BIT_OP // dest |= 1 << arg;
#define DECK_CLR_OP             DECK_BIT_OP // dest &= ~(1 << arg);
#define DECK_FLP_OP             DECK_BIT_OP // dest ^= 1 << arg;
/* parm-field bits for DECK_BIT_OP */
#define DECK_TST                0x00
#define DECK_SET                0x01
#define DECK_CLR                0x02
#define DECK_FLP                0x03

/*
 * shift operations
 * - DECK_SHL_OP:   parm=DECK_SHL
 * - DECK_SHR_OP:   parm=DECK_SHR
 * - DECK_SAR_OP:   parm=DECK_SAR
 */
#define DECK_SFT_OP             0x02
#define DECK_SHL_OP             DECK_SFT_OP // dest = src << arg;
#define DECK_SHR_OP             DECK_SFT_OP // dest = src >> arg;
#define DECK_SAR_OP             DECK_SFT_OP // dest = src >>> arg;
/* parm-field bits for DECK_SFT_OP */
#define DECK_SHL                0x00
#define DECK_SHR                0x01
#define DECK_SAR                0x02

/* arithmetic operations */
/* the U-prefix unsigned operations have (parm & DECK_UNSIGNED_BIT)   MS */
#define DECK_INC_OP             0x03        // dest = src + 1;           C
#define DECK_UINC_OP            DEC_INC_OP
#define DECK_DEC_OP             0x04        // dest = src - 1;           C
#define DECK_UDEC_OP            DECK_DEC_OP
#define DECK_ADD_OP             0x05        // dest = src + arg;         C, O
#define DECK_UADD_OP            DECK_ADD_OP
#define DECK_SUB_OP             0x06        // dest = src - arg;         C, O
#define DECK_USUB_OP            DECK_SUB_OP
#define DECK_MUL_OP             0x07        // dest = src * arg;         C, O
#define DECK_UMUL_OP            DECK_MUL_OP
#define DECK_MHI_OP             0x08        // dest = (src * arg) >> 32; C, O
#define DECK_UMHI_OP            DECK_MHI_OP
/* DIV, UDIV, REM, UREM, IRP, URP trigger DECK_MATH_TRAP if the divisor is 0 */
#define DECK_DIV_OP             0x09        // dest = src / arg;
#define DECK_UDIV_OP            DECK_DIV_OP
#define DECK_REM_OP             0x0a        // dest = src % arg;
#define DECK_UREM_OP            DECK_REM_OP
#define DECK_IRP_OP             0x0b        // dest = irp(src);
#define DECK_URP_OP             DECK_IRP_OP // dest = uirp(src);
/*
 * hash operations
 * - DECK_HSH_OP:   !(parm & DECK_REV_BIT)
 * - DECK_HUN_OP:   (parm & DECK_REV_BIT)
 */
#define DECK_HSH_OP             0x0c        // dest = hsh(src);
#define DECK_HUN_OP             DECK_HSH_OP // dest = hun(src);

/*
 * bit-count operations
 * - DECK_CTZ_OP:   parm=DECK_CTZ
 * - DECK_CLZ_OP:   parm=DECK_CLZ
 * - DECK_HAM_OP:   parm=DECK_HAM
 * - DECK_PAR_OP:   parm=DECK_PAR
 */
#define DECK_CNT_OP             0x0d
#define DECK_CTZ_OP             DECK_CNT_OP // low 0-bits; dest = ctz(src);
#define DECK_CLZ_OP             DECK_CNT_OP // high 0-bits; dest = clz(src);
#define DECK_HAM_OP             DECK_CNT_OP // 1-bits; dest=ham(src);
#define DECK_PAR_OP             DECK_PAR_OP // odd/even 1-bits = clz(src) & 1;
/* parm-field bits for DECK_CNT_OP */
#define DECK_CTZ                0x00
#define DECK_CLZ                0x01
#define DECK_HAM                0x02
#define DECK_PAR                0x03

/*
 * control operations
 * - JMP, JAL:      struct deckjmpinst
 * - JEQ, JLT, JGT, JCF, JOF, JNR:  struct deckbrainst
 */
#define DECK_CMP_OP             0x0e        // compare; set flags in MS
#define DECK_UCMP_OP            DECK_CMP_OP // parm & DECK_UNSIGNED_BIT
#define DECK_JMP_OP             0x0f        // jump, cond=DECK_ANY_COND
/* jump, cond=DECK_ANY_COND, (parm & DECK_LINK_BIT) */
#define DECK_JAL_OP             DECK_JMP_OP
#define DECK_BRA_OP             0x10
#define DECK_JEQ_OP             DECK_BRA_OP // jump, cond=DECK_EQ_COND
#define DECK_JLT_OP             DECK_BRA_OP // jump, cond=DECK_LT_COND
#define DECK_JGT_OP             DECK_BRA_OP // jump, cond=DECK_GT_COND
#define DECK_JCF_OP             DECK_BRA_OP // jump, cond=DECK_CF_COND
#define DECK_JOF_OP             DECK_BRA_OP // jump, cond=DECK_OF_COND
#define DECK_JNR_OP             DECK_BRA_OP // jump, cond=DECK_NR_COND
#define DECK_REP_OP             DECK_BRA_OP // jump, cond=DECK_CR_COND (CNT > 1)
#define DECK_SBR_OP             0x11        // call subroutine
#define DECK_RET_OP             0x12        // return from subroutine
#define DECK_SYS_OP             0x13        // start system call
#define DECK_SRT_OP             0x14        // return from system call

/*
 * memory operations
 * - LEA:           struct deckinst
 * - STR, LDR, MOV: struct deckinst
 * - PSH, POP:      struct deckstkinst, (parm & DECK_STORE_BIT) for PSH
 * - STM, LDM:      struct deckmapinst
 * - low bits in parm specify source operand size
 * - DECK_SEX_BIT dictates zero/sign-extension
 */
#define DECK_LEA_OP             0x15        // dest = arg(src) + imm
#define DECK_STR_OP             0x16        // *(arg(dest) + imm) = src;
#define DECK_LDR_OP             0x17        // dest = *(arg(src) + imm);
#define DECK_MOV_OP             0x18        // dest = src;
#define DECK_STK_OP             0x19        // PSH, POP
#define DECK_PSH_OP             DECK_STK_OP // sp -= 4; *sp = src;
#define DECK_POP_OP             DECK_STK_OP // dest = *sp; sp += 4;
#define DECK_MAP_OP             0x1a        // STM, LDM
#define DECK_STM_OP             DECK_MAP_OP // push multiple registers; write=1
#define DECK_LDM_OP             DECK_MAP_OP // pop multiple registers; write=0

/* I/O operations */
#define DECK_IOP_OP             0x1b        // permission control
#define DECK_IOC_OP             0x1c        // I/O commands
#define DECK_IOR_OP             0x1d        // I/O-port reads
#define DECK_IOW_OP             0x1e        // I/O-port writes
/* 0x1f is reserved */

/* system operations */
#define DECK_HLT_OP             0x20        // halt to wait for interrupts
#define DECK_CLI_OP             0x21        // disable interrupts; default all
#define DECK_STI_OP             0x22        // enable interrupts; default all
#define DECK_INT_OP             0x23        // processor ID in parm, -1 for self
#define DECK_IRT_OP             0x24        // return from interrupt handler

/* instruction IDs 0x2c..0x3d are currently reserved */

#define DECK_COPROC_OP          0x3e        // coprocessor-defined instructions
#define DECK_NOP_OP             0x3f        // NOP is 32-bit, all 1-bits
#define DECK_OP_BITS            6           // # of instruction-ID bits

/*
 * adr-field; address-mode bits
 *
 * LDR: adr=src, ndx=arg, ofs=imm
 * STR: adr=dest, ndx=arg, ofs=imm
 */
#define DECK_REG_ADR            0x00        // all 0-bits
#define DECK_IMM_ADR            (1 << 0)    // denotes argument in imm32
#define DECK_BASE_ADR           (1 << 1)    // denotes base-address in src/dest
#define DECK_OFS_ADR            (1 << 2)    // denotes address-offset in arg
#define DECK_ADR_BITS           3           // number of address-mode bits

/*
 * cond-field; conditions for execution
 */
#define DECK_ANY_COND           0x00        // always execute
#define DECK_EQ_COND            0x01        // MS & ZF
#define DECK_LT_COND            0x02        // MS & LT
#define DECK_GT_COND            0x03        // MS & GT
#define DECK_CF_COND            0x04        // MS & CF
#define DECK_OF_COND            0x05        // MS & OF
#define DECK_NR_COND            0x06        // !(MS & RF)
#define DECK_CR_COND            0x07        // CNT > 0
#define DECK_COND_BITS          3           // # of bits to represent conditions

/* the shift counts for folded instructions are stored in the parm-field */
#define DECK_NO_FOLD            0x00        // no fold-instruction
#define DECK_FOLD_SHL           0x01        // fold left shift
#define DECK_FOLD_SHR           0x02        // fold logical right shift
#define DECK_FOLD_SAR           0x03        // fold arithmetic right shift
#define DECK_FOLD_BITS          2           // # of fold-instruction ID-bits

/* parm-field flags */
#define DECK_WFE_BIT            (1 << 0)    // WFE-instruction (WFI-modifier)
#define DECK_SIZE_BITS          2           // 0 - 8-bit, 1 - 16, 2 - 32, 3 - 64
#define DECK_SEX_BIT            (1 << 2)    // 0: zero-extend, 1: sign-extend
#define DECK_STORE_BIT          (1 << 3)    // PSH, STM
#define DECK_SHIFT_BITS         5           // # of bits in shift counts
#define DECK_ATOMIC_BIT         (1 << 6)    // atomic [memory] operation
#define DECK_REV_BIT            (1 << 6)    // reverse bits; HUN
#define DECK_LINK_BIT           (1 << 6)    // JAL
#define DECK_UNSIGNED_BIT       (1 << 7)    // perform unsigned operation

#define DECK_STD_INST           0
#define DECK_CMP_INST           1
#define DECK_JMP_INST           2
#define DECK_BRA_INST           3
#define DECK_STK_INST           4
#define DECK_MAP_INST           5
#define DECK_IO_INST            6
#define DECK_NOP_INST           7
#define DECK_INST_TYPES         8

#define DECK_STD_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_NOP_OP,                                                    \
        DECK_ANY_COND,                                                  \
        DECK_R0_REG,                                                    \
        DECK_R0_REG,                                                    \
        DECK_R1_REG,                                                    \
        DECK_REG_ADR,                                                   \
        DECK_NO_FOLD,                                                   \
        0                                                               \
    }

struct deckstdinst {
    unsigned                    op      : DECK_OP_BITS;         // [5:0]    (6)
    unsigned                    cond    : DECK_COND_BITS;       // [8:6]    (3)
    unsigned                    src     : DECK_GEN_REG_BITS;    // [11:9]   (3)
    unsigned                    dest    : DECK_REG_BITS;        // [15:12]  (4)
    unsigned                    arg     : DECK_GEN_REG_BITS;    // [18:16]  (3)
    unsigned                    adr     : DECK_ADR_BITS;        // [21:19]  (3)
    unsigned                    fold    : DECK_FOLD_BITS;       // [23:22]  (2)
    int8_t                      parm;                           // [31:24]
    int32_t                     imm32[C_VLA];
};

#define DECK_CMP_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_CMP_OP,                                                    \
        DECK_ANY_COND,                                                  \
        DECK_R0_REG,                                                    \
        DECK_R0_REG,                                                    \
        0                                                               \
    }
struct deckcmpinst {
    unsigned                    op      : DECK_OP_BITS;         // (6)
    unsigned                    cond    : DECK_COND_BITS;       // (3)
    unsigned                    src     : DECK_GEN_REG_BITS;    // (3)
    unsigned                    dest    : DECK_REG_BITS;        // (4)
    unsigned                    parm    : 3;                    // SIZE, SEX
    signed                      imm13   : 13; // [-8192, 8191]
};

#define DECK_JMP_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_JMP_OP,                                                    \
        0                                                               \
    }
/*
 * NOTE: the jump offsets are in 32-bit words to save space and meet alignment
 */
#define DECK_OFS_BITS           26          // PC-relative word-offset bits
struct deckjmpinst {
    unsigned                    op      : DECK_OP_BITS;     // [5:0]
    unsigned                    ofs     : DECK_OFS_BITS;    // [31:6]
};

#define DECK_BRA_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_BRA_OP,                                                    \
        DECK_ANY_COND,                                                  \
        0                                                               \
    }
#define DECK_BRA_BITS           23          // PC-relative word-offset bits
struct deckbrainst {
    unsigned                    op      : DECK_OP_BITS;
    unsigned                    cond    : DECK_COND_BITS;   // [8:6]
    unsigned                    ofs     : DECK_BRA_BITS;    // [31:9]
};

#define DECK_STK_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_NOP_OP,                                                    \
        DECK_ANY_COND,                                                  \
        2,                                                              \
        0,                                                              \
        0,                                                              \
    }
/*
 * parm: SIZE[14:13], RING[15]
 */
struct deckstkinst {
    unsigned                    op      : DECK_OP_BITS;     // PSH or POP
    unsigned                    cond    : DECK_COND_BITS;   // condition
    unsigned                    reg     : DECK_REG_BITS;    // any register
    unsigned                    parm    : 3;                // see above
    int16_t                     imm;
};

#define DECK_MAP_INST_INITIALIZER                                       \
    {                                                                   \
        DECK_NOP_OP,                                                    \
        DECK_ANY_COND,                                                  \
        0,                                                              \
        0,                                                              \
    }
/*
 * - STM, LDM; struct deckmapinst
 * - (parm & DECK_STORE_BIT) for STM
 * - the map-field have 1-bit per register to list the pushed/popped registers;
 *   the lowest bit is R0, the highest (#15) is MF which is static (POP invalid)
 */
struct deckmapinst {
    unsigned                    op      : DECK_OP_BITS;     // STM or LDM
    unsigned                    cond    : DECK_COND_BITS;   // condition
    unsigned                    parm    : 7;                // reserved
    unsigned                    bits    : DECK_INT_REGS - 1; // reg-bitmap
    unsigned                    stm     : 1;                 // 0=ldm, 1=stm
};

#define DECK_IO_INST_INITIALIZER                                        \
    {                                                                   \
        DECK_NOP_OP,                                                    \
        DECK_ANY_COND,                                                  \
        DECK_R0_REG,                                                    \
        DECK_R1_REG,                                                    \
        0,                                                              \
        0,                                                              \
        1,                                                              \
    }
struct deckioinst {
    unsigned                    op      : DECK_OP_BITS;     // IOC, IOR, IOW
    unsigned                    cond    : DECK_COND_BITS;   // condition
    unsigned                    reg     : DECK_GEN_REG_BITS; // in/out register
    unsigned                    port    : DECK_GEN_REG_BITS; // port register
    unsigned                    ring    : 1;                // 1=system-ring
    uint16_t                    cmd;                        // 16-bit command
};

#endif /* DECK_INST_H */

