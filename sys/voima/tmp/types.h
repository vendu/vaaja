#ifndef __VOIMA_TYPES_H__
#define __VOIMA_TYPES_H__

#define VOIMA_CL_VALID_TAG      (1 << 0)
#define VOIMA_CL_DIRTY_TAG      (1 << 1)
#define VOIMA_CL_DISCARD_TAG    (1 << 2)
struct tagcl {
    uintptr_t   desc;   // cacheline-descriptor; address + low tag-bits
};

#define VOIMA_INS1_L1_BIT       (1 << 0)
#define VOIMA_INS1_L2_BIT       (1 << 1)
#define VOIMA_INS1_WR_BIT       (1 << 2)
#define VOIMA_INS1_STI_BIT      (1 << 1)
#define VOIMA_INS1_CLI_BIT      (1 << 2)
#define VOIMA_INS1_QINT_BIT     (1 << 0)
#define VOIMA_INS1_BMAP_BIT     (1 << 0)
#define VOIMA_INS1_SPIN_BIT     (1 << 0)
struct voimains1 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  reg : 4;            // register ID
    int8_t  flg : 4;            // flag-bits
};

struct voimains2 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  dst : 4;            // destination register ID
    int8_t  src : 4;            // source register ID
};

struct voimains3 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  dst : 4;            // destination register ID
    int8_t  src : 4;            // source register ID
    int8_t  flg : 8;            // flag-bits
};

struct voimains4 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  dst : 4;            // destination register ID
    int8_t  src : 4;            // source register ID
    int8_t  flg : 8;            // flag-bits
};

struct voimains5 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  dst : 4;            // destination register ID
    int8_t  src : 4;            // source register ID
    int8_t  flg : 8;            // flag-bits
    int8_t  pad : 8;            // padding to align imm32
    int32_t imm32[C_VLA];       // optional 32-bit immediate operand
};

struct voimains5 {
    int8_t  op;                 // operation ID + ALN-bit
    int8_t  dst : 4;            // destination register ID
    int8_t  cmd : 4;            // command register ID
    int16_t port;               // 16-bit port ID
};

struct stkframe {
    int32_t argregs[4]; // R1..R4
    int32_t lr;         // link-register (return address)
    int32_t oldfp;      // caller frame-pointer             <= fp
    int32_t tmpregs[7]; // R5..R11
    int32_t autovars[C_VLA];
};

struct trapframe {
    int32_t err;
    int32_t msw;
    int32_t pc;
    int32_t lr;
    int32_t sp;
    int32_t fp;
    int32_t intregs[12];    // R0..R11
};

/*
 * flg
 * ---
 * - RAW_BIT            // raw [character] device
 * - BLK_BIT            // block-device
 * - MAP_BIT            // map MTR-ranges with specified flags
 * - CONN_BIT           // connect device
 * - POST_BIT           // perform device power-on self-test
 * - FSCK_BIT           // perform physical filesystem check
 * - TAP_BIT            // attach, e.g., a debugger tap to a network connection
 */
struct devatr {
    uint8_t    *buf;
    off_t       ofs;
    size_t      len;
    int32_t     dev;
    int32_t     flg;
};

#define voimagetpc(vm)  ((vm)->intregs[VOIMA_PC_REG])
#define voimagetlr(vm)  ((vm)->intregs[VOIMA_LR_REG])
#define voimagetsp(vm)  ((vm)->intregs[VOIMA_SP_REG])
#define voimagetfp(vm)  ((vm)->intregs[VOIMA_FP_REG])
struct voimavm {
    int8_t     *mem;
    size_t      memsize;
    int32_t     intregs[VOIMA_INTEGER_REGISTERS];
    double      fpuregs[VOIMA_FPU_REGISTERS];
    int32_t     sysregs[VOIMA_HIDDEN_REGISTERS];
};

#endif /* __VOIMA_TYPES_H__ */

