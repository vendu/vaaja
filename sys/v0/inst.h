#ifndef SYS_V0_INST_H
#define SYS_V0_INST_H

/*
 * NOTES
 * -----
 * - ORR, XOR, AND, INC, DEC, ADD, SUB may be used with the ATOMIC-bit set; see
 *   AORR etc. under V0_MP_EXTENSION
 */

#define V0_RET_BITS             64 // normally 32, 64-bit internal
/* instructions are 32-bit */
#define V0_INST_BITS            32
/* NOP is a 32-bit parcel of all 1-bits */
#define V0_NOP                  (~UINT32_C(0))

/*
 * logical operations; struct v0inst
 * - V0_NOT_OP:   parm=V0_NOT
 * - V0_ORR_OP:   parm=V0_ORR
 * - V0_XOR_OP:   parm=V0_XOR
 * - V0_AND_OP:   parm=V0_AND
 * - V0_NOR_OP:   parm=V0_NOR
 * - V0_XNOR_OP:  parm=V0_XNOR
 * - V0_NAND_OP:  parm=V0_NAND
 */
#define V0_LGF_OP               0x00
#define V0_NOT_OP               V0_LGF_OP // dest = ~src;
#define V0_ORR_OP               V0_LGF_OP // dest = src | arg;
#define V0_XOR_OP               V0_LGF_OP // dest = src ^ arg;
#define V0_AND_OP               V0_LGF_OP // dest = src & arg;
#define V0_NOR_OP               V0_LGF_OP // dest = src ~| arg;
#define V0_XNOR_OP              V0_LGF_OP // dest = src ^~ arg;
#define V0_NAND_OP              V0_LGF_OP // dest = src ~& arg;
/* parm-field bits for V0_LGF_OP */
#define V0_NOT                  0x00
#define V0_ORR                  0x01
#define V0_XOR                  0x02
#define V0_AND                  0x03
#define V0_NOR                  0x04
#define V0_XNOR                 0x05
#define V0_NAND                 0X06
/* 0x07 is reserved */

/*
 * bit-operations; struct v0inst
 * - V0_TST_OP:   parm=V0_TST
 * - V0_SET_OP:   parm=V0_SET
 * - V0_CLR_OP:   parm=V0_CLR
 * - V0_FLP_OP:   parm=V0_FLP
 */
#define V0_BIT_OP               0x01
#define V0_TST_OP               V0_BIT_OP // dest = src & (1 << arg);
#define V0_SET_OP               V0_BIT_OP // dest |= 1 << arg;
#define V0_CLR_OP               V0_BIT_OP // dest &= ~(1 << arg);
#define V0_FLP_OP               V0_BIT_OP // dest ^= 1 << arg;
/* parm-field bits for V0_BIT_OP */
#define V0_TST                  0x00
#define V0_SET                  0x01
#define V0_CLR                  0x02
#define V0_FLP                  0x03

/*
 * shift operations
 * - V0_SHL_OP:   parm=V0_SHL
 * - V0_SHR_OP:   parm=V0_SHR
 * - V0_SAR_OP:   parm=V0_SAR
 */
#define V0_SFT_OP               0x02
#define V0_SHL_OP               V0_SFT_OP // dest = src << arg;
#define V0_SHR_OP               V0_SFT_OP // dest = src >> arg;
#define V0_SAR_OP               V0_SFT_OP // dest = src >>> arg;
/* parm-field bits for V0_SFT_OP */
#define V0_SHL                  0x00
#define V0_SHR                  0x01
#define V0_SAR                  0x02

/* arithmetic operations */
/* the U-prefix unsigned operations have (parm & V0_UNSIGNED_BIT)   MS */
#define V0_INC_OP               0x03        // dest = src + 1;           C
#define V0_UINC_OP              DEC_INC_OP
#define V0_DEC_OP               0x04        // dest = src - 1;           C
#define V0_UDEC_OP              V0_DEC_OP
#define V0_ADD_OP               0x05        // dest = src + arg;         C, O
#define V0_UADD_OP              V0_ADD_OP
#define V0_SUB_OP               0x06        // dest = src - arg;         C, O
#define V0_USUB_OP              V0_SUB_OP
#define V0_MUL_OP               0x07        // dest = src * arg;         C, O
#define V0_UMUL_OP              V0_MUL_OP
#define V0_MHI_OP               0x08        // dest = (src * arg) >> 32; C, O
#define V0_UMHI_OP              V0_MHI_OP
/* DIV, UDIV, REM, UREM, IRP, URP trigger V0_MATH_TRAP if the divisor is 0 */
#define V0_DIV_OP               0x09        // dest = src / arg;
#define V0_UDIV_OP              V0_DIV_OP
#define V0_REM_OP               0x0a        // dest = src % arg;
#define V0_UREM_OP              V0_REM_OP
#define V0_IRP_OP               0x0b        // dest = irp(src);
#define V0_URP_OP               V0_IRP_OP // dest = uirp(src);
/*
 * hash operations
 * - V0_HSH_OP:   !(parm & V0_REV_BIT)
 * - V0_HUN_OP:   (parm & V0_REV_BIT)
 */
#define V0_HSH_OP               0x0c        // dest = hsh(src);
#define V0_HUN_OP               V0_HSH_OP // dest = hun(src);

/*
 * bit-count operations
 * - V0_CTZ_OP:   parm=V0_CTZ
 * - V0_CLZ_OP:   parm=V0_CLZ
 * - V0_HAM_OP:   parm=V0_HAM
 * - V0_PAR_OP:   parm=V0_PAR
 */
#define V0_CNT_OP               0x0d
#define V0_CTZ_OP               V0_CNT_OP // low 0-bits; dest = ctz(src);
#define V0_CLZ_OP               V0_CNT_OP // high 0-bits; dest = clz(src);
#define V0_HAM_OP               V0_CNT_OP // 1-bits; dest=ham(src);
#define V0_PAR_OP               V0_PAR_OP // odd/even 1-bits = clz(src) & 1;
/* parm-field bits for V0_CNT_OP */
#define V0_CTZ                  0x00
#define V0_CLZ                  0x01
#define V0_HAM                  0x02
#define V0_PAR                  0x03

/*
 * control operations
 * - JMP, JAL:      struct v0jmpinst
 * - JEQ, JLT, JGT, JCF, JOF, JNR:  struct v0brainst
 */
#define V0_CMP_OP               0x0e        // compare; set flags in MS
#define V0_UCMP_OP              V0_CMP_OP // parm & V0_UNSIGNED_BIT
#define V0_JMP_OP               0x0f        // jump, cond=V0_ANY_COND
/* jump, cond=V0_ANY_COND, (parm & V0_LINK_BIT) */
#define V0_JAL_OP               0_JMP_OP
#define V0_BRA_OP               0x10
#define V0_JEQ_OP               V0_BRA_OP // jump, cond=V0_EQ_COND
#define V0_JLT_OP               V0_BRA_OP // jump, cond=V0_LT_COND
#define V0_JGT_OP               V0_BRA_OP // jump, cond=V0_GT_COND
#define V0_JCF_OP               V0_BRA_OP // jump, cond=V0_CF_COND
#define V0_JOF_OP               V0_BRA_OP // jump, cond=V0_OF_COND
#define V0_JNR_OP               V0_BRA_OP // jump, cond=V0_NR_COND
#define V0_REP_OP               V0_BRA_OP // jump, cond=V0_CR_COND (CNT > 1)
#define V0_SBR_OP               0x11        // call subroutine
#define V0_RET_OP               0x12        // return from subroutine
#define V0_SYS_OP               0x13        // start system call
#define V0_SRT_OP               0x14        // return from system call

/*
 * memory operations
 * - LEA:           struct v0inst
 * - STR, LDR, MOV: struct v0inst
 * - PSH, POP:      struct v0stkinst, (parm & V0_STORE_BIT) for PSH
 * - STM, LDM:      struct v0mapinst, (op.stm = 1) = store, 0 for load
 * - low bits in parm specify source operand size
 * - V0_SEX_BIT dictates zero/sign-extension
 */
#define V0_LEA_OP               0x15        // dest = arg(src) + imm
#define V0_STR_OP               0x16        // *(arg(dest) + imm) = src;
#define V0_LDR_OP               0x17        // dest = *(arg(src) + imm);
#define V0_MOV_OP               0x18        // dest = src; (reg to reg)
#define V0_STK_OP               0x19        // PSH, POP
#define V0_PSH_OP               V0_STK_OP // sp -= 4; *sp = src;
#define V0_POP_OP               V0_STK_OP // dest = *sp; sp += 4;
#define V0_MAP_OP               0x1a        // STM, LDM
#define V0_STM_OP               V0_MAP_OP // push multiple registers; write=1
#define V0_LDM_OP               V0_MAP_OP // pop multiple registers; write=0

/* I/O operations */
#define V0_IOP_OP               0x1b        // permission control
#define V0_IOC_OP               0x1c        // I/O commands
#define V0_IOR_OP               0x1d        // I/O-port reads
#define V0_IOW_OP               0x1e        // I/O-port writes
/* 0x1f is reserved */

/* system operations */
#define V0_HLT_OP               0x20        // halt to wait for interrupts
#define V0_CLI_OP               0x21        // disable interrupts; default all
#define V0_STI_OP               0x22        // enable interrupts; default all
#define V0_INT_OP               0x23        // processor ID in parm, -1 for self
#define V0_IRT_OP               0x24        // return from interrupt handler
#define V0_WFE_OP               0x25        // wait for event
#define V0_SEV_OP               0x26        // signal event

/* instruction IDs 0x2c..0x3d are currently reserved */

#define V0_NOP_OP               0x3e        // NOP is 32-bit, all but low 1-bits
#define V0_COPROC_OP            0x3f        // coprocessor-operation, all 1-bits
#define V0_OP_BITS              6           // # of instruction-ID bits

/*
 * adr-field; address-mode bits
 *
 * LDR: adr=src, ndx=arg, ofs=imm
 * STR: adr=dest, ndx=arg, ofs=imm
 */
#define V0_REG_ADR              0x00        // all 0-bits
#define V0_IMM_ADR              (1 << 0)    // denotes argument in imm32
#define V0_BASE_ADR             (1 << 1)    // denotes base-address in src/dest
#define V0_OFS_ADR              (1 << 2)    // denotes address-offset in arg
#define V0_ADR_BITS             3           // number of address-mode bits

/*
 * cond-field; conditions for execution
 */
#define V0_ANY_COND             0x00        // always execute
#define V0_EQ_COND              0x01        // MS & ZF
#define V0_LT_COND              0x02        // MS & LT
#define V0_GT_COND              0x03        // MS & GT
#define V0_CF_COND              0x04        // MS & CF
#define V0_OF_COND              0x05        // MS & OF
#define V0_NR_COND              0x06        // !(MS & RF)
#define V0_CR_COND              0x07        // CNT > 0
#define V0_COND_BITS            3           // # of bits to represent conditions

/* the shift counts for folded instructions are stored in the parm-field */
#define V0_NO_FOLD              0x00        // no fold-instruction
#define V0_FOLD_SHL             0x01        // fold left shift
#define V0_FOLD_SHR             0x02        // fold logical right shift
#define V0_FOLD_SAR             0x03        // fold arithmetic right shift
#define V0_FOLD_BITS            2           // # of fold-instruction ID-bits

/* parm-field flags */
#define V0_WFE_BIT              (1 << 0)    // WFE-instruction (WFI-modifier)
#define V0_SIZE_BITS            2           // 0 - 8-bit, 1 - 16, 2 - 32, 3 - 64
#define V0_SEX_BIT              (1 << 2)    // 0: zero-extend, 1: sign-extend
#define V0_STORE_BIT            (1 << 3)    // PSH, STM
#define V0_SHIFT_BITS           6           // # of bits in shift counts
#define V0_ATOMIC_BIT           (1 << 6)    // atomic [memory] operation
#define V0_REV_BIT              (1 << 6)    // reverse bits; HUN
#define V0_LINK_BIT             (1 << 6)    // JAL
#define V0_UNSIGNED_BIT         (1 << 7)    // perform unsigned operation

#define V0_STD_INST             0
#define V0_CMP_INST             1
#define V0_JMP_INST             2
#define V0_BRA_INST             3
#define V0_STK_INST             4
#define V0_MAP_INST             5
#define V0_IO_INST              6
#define V0_NOP_INST             7
#define V0_INST_TYPES           8

#define V0_STD_INST_INITIALIZER                                         \
    {                                                                   \
        V0_NOP_OP, V0_ANY_COND, V0_R0_REG, V0_R0_REG, V0_R1_REG, V0_REG_ADR, V0_NO_FOLD, 0 \
    }

struct v0stdinst {
    unsigned                    op      : V0_OP_BITS;         // [5:0]    (6)
    unsigned                    cond    : V0_COND_BITS;       // [8:6]    (3)
    unsigned                    src     : V0_GEN_REG_BITS;    // [11:9]   (3)
    unsigned                    dest    : V0_REG_BITS;        // [15:12]  (4)
    unsigned                    arg     : V0_GEN_REG_BITS;    // [18:16]  (3)
    unsigned                    adr     : V0_ADR_BITS;        // [21:19]  (3)
    unsigned                    fold    : V0_FOLD_BITS;       // [23:22]  (2)
    uint8_t                     parm;                           // [31:24]
    int32_t                     imm32[C_VLA];
};
#define V0_STDINST_SIZE         (V0_OP_BITS                           \
                                 + V0_COND_BITS                       \
                                 + V0_GEN_REG_BITS                    \
                                 + V0_REG_BITS                        \
                                 + V0_GEN_REG_BITS                    \
                                 + V0_ADR_BITS                        \
                                 + V0_FOLD_BITS                       \
                                 + 8)

#define V0_CMP_INST_INITIALIZER                                       \
    {                                                                 \
        V0_CMP_OP, V0_ANY_COND, V0_R0_REG, V0_R0_REG, 0               \
    }
struct v0cmpinst {
    unsigned                    op      : V0_OP_BITS;         // (6)
    unsigned                    cond    : V0_COND_BITS;       // (3)
    unsigned                    src     : V0_GEN_REG_BITS;    // (3)
    unsigned                    dest    : V0_REG_BITS;        // (4)
    unsigned                    parm    : 3;                    // SIZE, SEX
    signed                      imm13   : 13; // [-8192, 8191]
};
#define V0_CMPINST_SIZE         (V0_OP_BITS                           \
                                 + V0_COND_BITS                       \
                                 + V0_GEN_REG_BITS                    \
                                 + V0_REG_BITS                        \
                                 + 3                                  \
                                 + 15)

#define V0_JMP_INST_INITIALIZER                                       \
    {                                                                 \
        V0_JMP_OP, 0                                                  \
    }
/*
 * NOTE: the jump offsets are in 32-bit words to save space and meet alignment
 */
#define V0_OFS_BITS             26          // PC-relative word-offset bits
struct v0jmpinst {
    unsigned                    op      : V0_OP_BITS;     // [5:0]
    unsigned                    ofs     : V0_OFS_BITS;    // [31:6]
};
#define V0_JMPINST_SIZE         (V0_OP_BITS                            \
                                 + V0_OFS_BITS                         \
                                 + 15)

#define V0_BRA_INST_INITIALIZER                                         \
    {                                                                   \
        V0_BRA_OP, V0_ANY_COND, 0                                       \
    }
#define V0_BRA_BITS             23          // PC-relative word-offset bits
struct v0brainst {
    unsigned                    op      : V0_OP_BITS;
    unsigned                    cond    : V0_COND_BITS;   // [8:6]
    unsigned                    ofs     : V0_BRA_BITS;    // [31:9]
};
#define V0_BRAINST_SIZE         (V0_OP_BITS                            \
                                 + V0_COND_BITS                        \
                                 + V0_BRA_BITS)

#define V0_STK_INST_INITIALIZER                                        \
    {                                                                  \
        V0_NOP_OP, V0_ANY_COND, 2, 0, 0,                               \
    }
/*
 * parm: SIZE[14:13], RING[15]
 */
struct v0stkinst {
    unsigned                    op      : V0_OP_BITS;     // PSH or POP
    unsigned                    cond    : V0_COND_BITS;   // condition
    unsigned                    reg     : V0_REG_BITS;    // any register
    unsigned                    parm    : 3;                // see above
    int16_t                     imm;
};
#define V0_STKINST_SIZE         (V0_OP_BITS                           \
                                 + V0_COND_BITS                       \
                                 + V0_REG_BITS                        \
                                 + 1)

#define V0_MAP_INST_INITIALIZER                                       \
    {                                                                 \
        V0_NOP_OP, V0_ANY_COND, 0, 0,                                 \
    }
/*
 * - STM, LDM; struct v0mapinst
 * - (parm & V0_STORE_BIT) for STM
 * - the map-field have 1-bit per register to list the pushed/popped registers;
 *   the lowest bit is R0, the highest (#15) is MF which is static (POP invalid)
 */
struct v0mapinst {
    unsigned                    op      : V0_OP_BITS;      // STM or LDM
    unsigned                    cond    : V0_COND_BITS;    // condition
    unsigned                    parm    : 7;               // reserved
    unsigned                    bits    : V0_INT_REGS - 1; // reg-bitmap
    unsigned                    stm     : 1;               // 0=ldm, 1=stm
};
#define V0_MAPINST_SIZE         (V0_OP_BITS                             \
                                 + V0_COND_BITS                         \
                                 + 7                                    \
                                 + V0_INT_REGS - 1                      \
                                 + 1)

#define V0_IO_INST_INITIALIZER                                          \
    {                                                                   \
        V0_NOP_OP, V0_ANY_COND, V0_R0_REG, V0_R1_REG, 0, 0, 1,          \
    }
struct v0ioinst {
    unsigned                    op      : V0_OP_BITS;     // IOC, IOR, IOW
    unsigned                    cond    : V0_COND_BITS;   // condition
    unsigned                    reg     : V0_GEN_REG_BITS; // in/out register
    unsigned                    port    : V0_GEN_REG_BITS; // port register
    unsigned                    ring    : 1;                // 1=system-ring
    uint16_t                    cmd;                        // 16-bit command
};
#define V0_IOINST_SIZE          (V0_OP_BITS                           \
                                 + V0_COND_BITS                       \
                                 + V0_GEN_REG_BITS                      \
                                 + V0_GEN_REG_BITS                      \
                                 + 1                                    \
                                 + 16)

#endif /* SYS_V0_INST_H */

