#ifndef __VOIMA_VM_INST_H__
#define __VOIMA_VM_INST_H__

#include <stdint.h>

/* macros */

#define v0ophasbrk(op)          ((int32_t)*(op) & (INT32_C(1) << 8))
#define v0ophasimm(op)          ((int32_t)*(op) & (INT32_C(1) << 9))

/* unit IDs */

#define V0_SYSTEM_UNIT          0x00
#define V0_ALUBIT_UNIT          0x01
#define V0_SHIFT_UNIT           0x02
#define V0_JUMP_UNIT            0x03
#define V0_BRANCH_UNIT          0x04
#define V0_MEMORY_UNIT          0x05
#define V0_FPX_UNIT             0x06
#define V0_DSP_UNIT             0x07

/* instruction set */

/* instruction data structures */

#define V0_OP_TRAP_BIT          (1 << 0)
#define V0_OP_IMM_BIT           (1 << 1)
#define V0_OP_LOCK_BIT          (1 << 2)
#define V0_OP_FLAG_BITS         3
#define V0_OP_IMM_SIZE_BITS     2
#define V0_OP_IMM_SIZE_MASK     0x0c
#define V0_OP_ADR_BASE_BIT      (1 << 0)
#define V0_OP_ADR_NDX_BIT       (1 << 1)
#define V0_OP_ADR_OFS_BIT       (1 << 2)

struct v0op {
    unsigned                    unit    :   3;
    unsigned                    inst    :   5;
    unsigned                    trap    :   1;
    unsigned                    foot    :   23;
};

struct v0sysop1 {
    unsigned                    unit    :   3;
    unsigned                    inst    :   5;
    unsigned                    trap    :   1;
    unsigned                    imm     :   1;
    unsigned                    _pad    :   1;
    unsigned                    immsize :   2;
    unsigned                    adrbits :   3;
    unsigned                    base    :   4;
    unsigned                    ndx     :   4;
    unsigned                    reg     :   4;
    unsigned                    cond    :   4;
    int32_t                     imm32[];
};

struct sysop2 {
    unsigned                    unit    :   3;
    unsigned                    inst    :   5;
    unsigned                    flags   :   3;
    unsigned                    _pad    :   1;
    unsigned                    reg     :   4;
    uint16_t                    imm16;
};

/* LDx */
#define V0_INT_REG_FILE         0x00
#define V0_SYS_REG_FILE         0x01
#define V0_FPX_REG_FILE         0x02
#define V0_DSP_REG_FILE         0x03
struct v0riop {
    unsigned                    unit    :   3;
    unsigned                    inst    :   5;
    unsigned                    trap    :   1;
    unsigned                    imm     :   1;
    unsigned                    reg     :   4;  // register ID
    unsigned                    immsz   :   2;  // immediate size
    int16_t                     imm16;          // 8- or 16-bit operand
    int32_t                     imm32[C_VLA];   // 32- or aligned 64-bit operand
};

struct v02rop {
    unsigned                    unit    :   3;
    unsigned                    inst    :   5;
    unsigned                    trap    :   1;
    unsigned                    imm     :   1;
    unsigned                    reg1    :   4;  // register ID
    unsigned                    reg2    :   4;  // register ID
    unsigned                    cond    :   4;
    unsigned                    parm    :   4;
    unsigned                    imm6    :   6;  // shift count, ...
    int32_t                     imm32[C_VLA];   // 32- or aligned 64-bit operand
};

/* offsets in 32-bit words */
union v0jmpadr {
    struct {
        unsigned                base    :   4;
        unsigned                ndx     :   4;
        signed                  ofs     :   14;
    } val;
    signed                      ofs     :   22;
};

#define v0jmpadr(vm, op)                                                \
    (!(op)->imm                                                         \
     ? ((void *)&vm->mem[((op)->adr.val.base                            \
                          + (op)->adr.val.ndx                           \
                          + (op).val.ofs)])                             \
     : ((void *)((uintptr_t)(op)                                        \
                 + sizeof(struct v0jmpinst) \
                 + sizeof(int32_t) * (op)->ofs)))
struct v0jmpop {
    unsigned                    unit    :   3;  // unit ID V0_JUMP_UNIT
    unsigned                    inst    :   5;  // JMP, JMI, JMR, JAL
    unsigned                    trap    :   1;  // function has breakpoint set
    unsigned                    imm     :   1;  // immediate address present
    union v0jmpadr              adr;
};

/* system unit #0 */

/*
 * assembly syntax
 * ---------------
 * hlt  r/i                     - hlt(intrmask);            - sysop1
 * rst  r/i                     - rst(initflg);             - sysop1
 * brk  i, r, a                 - brk(cmd, parm, tab);      - sysop3
 * - brk(V0_BRK_INIT, nbrk, tab);
 * - brk(V0_BRK_SET, adr);
 * - bkr(V0_BRK_CLR, adr);
 * int  i                       - int(id);                  - sysop1
 * nmi  i                       - nmi(id);                  - sysop1
 * cli
 * cli  r/i                     - cli(intrmask);            - sysop1
 * sti
 * cli  r/i                     - sti(intrmask);            - sysop1
 * mtr  i, r, a                 - mtr(id, low, high);       - sysop3
 * iop  i, r                    - iop(cmd, port);           - sysop2
 * - iop(V0_IOP_SET, port);
 * - iop(V0_IOP_CLR, port);
 * - iop(V0_IOP_CHK, port);
 * ior  i, r, a                 - ior(id, nb, adr);         - sysop3
 * iow  i, r, a                 - ior(id, nb, adr);         - sysop3
 * ioc  i, r, a                 - ioc(cmd, nb, adr);        - sysop3
 * - ioc(V0_IO_INIT, nb, adr);
 * - ioc(V0_IO_BUF, id, adr);
 * - ioc(V0_IO_FLSH, id, adr);
 * flc  a                       - flc(adr);                 - sysop1
 * fpg  a                       - fpg(adr);                 - sysop1
 */
#define V0_HLT_OP               0x00
#define V0_RST_OP               0x01
#define V0_BRK_OP               0x02
#define V0_INT_OP               0x03
#define V0_NMI_OP               0x04
#define V0_CLI_OP               0x05
#define V0_STI_OP               0x06
#define V0_MTR_OP               0x07
#define V0_IOP_OP               0x08
#define V0_IOR_OP               0x09
#define V0_IOW_OP               0x0a
#define V0_IOC_OP               0x0b
#define V0_FCL_OP               0x0c
#define V0_FPG_OP               0x0d
/* multiprocessor extensions */
/*
 * assembly syntax
 * ---------------
 * bus  i, r, a                 - bus(cmd, id, adr);
 * - bus(V0_BUS_PROBE, bus, buf);
 * - bus(V0_BUS_CONF, dev, buf);
 * - bus(V0_BUS_CLOSE, dev, buf);
 * mpc  i, r, a                 - mpc(cmd, id, adr);
 * bar                          - bar();
 * brd                          - brd();
 * bwr                          - bwr();
 * bts  r, a                    - bts(pos, map);
 * btc  r, a                    - btc(pos, map);
 * btf  r, a                    - btf(pos, map);
 * xinc r, a                    - r = *a, *a++;
 * xdec r, a                    - r = *a, *a--;
 * xadd r1, r2, a               - r2 = *a, *a = ++r1;
 * xsub r1, r2, a               - r2 = *a, *a = --r1;
 * xorr r1, a                   - *a |= r1;
 * xxor r1, a                   - *a ^= r1;
 * xand r1, a                   - *a &= r1;
 * ---------------
 * cas  r1, r2, a;              - cas(want, need, adr);
 * if (*a == r1) {
 *     *a = r2;
 *
 *     return 0;
 * } else {
 *
 *     return 1;
 * }
 * ---------------
 * wfe                          - waite(MEM_WR, intrmask);  // wait for write
 * sev                          - esig(MEM_WAKE, intrmask); // wake threads up
 */
#define V0_BUS_OP               0x0e
#define V0_MPC_OP               0x0f
#define V0_BAR_OP               0x10
#define V0_BRD_OP               0x11
#define V0_BWR_OP               0x12
#define V0_BTS_OP               0x13
#define V0_BTC_OP               0x14
#define V0_BTF_OP               0x15
#define V0_XINC_OP              0x16
#define V0_XDEC_OP              0x17
#define V0_XADD_OP              0x18
#define V0_XSUB_OP              0x19
#define V0_XORR_OP              0x1a
#define V0_XXOR_OP              0x1b
#define V0_XAND_OP              0x1c
#define V0_CAS_OP               0x1d
#define V0_WFE_OP               0x1e
#define V0_SEV_OP               0x1f

/* arithmetic-logical and bit unit #1 */
#define V0_CMP_OP               0x00
#define V0_NOT_OP               0x01
#define V0_ORR_OP               0x02
#define V0_XOR_OP               0x03
#define V0_AND_OP               0x04
#define V0_NOR_OP               0x05
#define V0_XNOR_OP              0x06
#define V0_NAND_OP              0x07
#define V0_INC_OP               0x08
#define V0_DEC_OP               0x09
#define V0_ADD_OP               0x0a
#define V0_SUB_OP               0x0b
#define V0_MUL_OP               0x0c
#define V0_MHI_OP               0x0d
#define V0_DIV_OP               0x0e
#define V0_REM_OP               0x0f
#define V0_DRM_OP               0x10
#define V0_IRP_OP               0x11
#define V0_ZEX_OP               0x12
#define V0_SEX_OP               0x13
#define V0_NEG_OP               0x14
#define V0_PAR_OP               0x15
#define V0_HAM_OP               0x16
#define V0_CTZ_OP               0x17
#define V0_CLZ_OP               0x18
#define V0_HSH_OP               0x19
#define V0_HUN_OP               0x1a

/* shift unit */

#define V0_SAR_OP               0x00
#define V0_SLR_OP               0x01
#define V0_ROR_OP               0x02
#define V0_RCR_OP               0x03
#define V0_SLL_OP               0x04
#define V0_ROL_OP               0x05
#define V0_RCL_OP               0x06

/* jump unit */

#define V0_JMP_OP               0x00
#define V0_JMI_OP               0x01
#define V0_JMR_OP               0x02
#define V0_JAL_OP               0x03

#endif /* __VOIMA_VM_INST_H__ */

