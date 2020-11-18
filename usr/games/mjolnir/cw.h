#ifndef __MJOLNIR_CW_H__
#define __MJOLNIR_CW_H__

#define CW_CORE_SIZE    1024            // fits on a 80x24 terminal in 64 * 16

#include <mjolnir/conf.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(ZEUS) && defined(ZEUSSDL)
#include <mjolnir/sdl.h>
#endif

#define cwpow2(x)       (!((x) & ((x) - 1)))
#if (cwpow2(CW_CORE_SIZE))
#define cwwrapcore(a)   ((a) & (CW_CORE_SIZE - 1))
#else
#define cwwrapcore(a)   ((a) % CW_CORE_SIZE)
#endif

long                    cwinit(int argc, char *argv[]);
void                    cwexec(long pid);

#define CW_TURNS        (128 * 1024 * 1024)
#define CW_PROCS        1024
#define CW_NO_OP        0
#define CW_INVAL        ((struct cwinstr){ 0 })

/* opcodes */
#define CW_OP_DAT       1
#define CW_OP_MOV       2
#define CW_OP_ADD       3
#define CW_OP_SUB       4
#define CW_OP_JMP       5
#define CW_OP_JMZ       6
#define CW_OP_JMN       7
#define CW_OP_CMP       8
#define CW_OP_SLT       9
#define CW_OP_DJN       10
#define CW_OP_SPL       11
#define CW_MAX_OP       11

/* flags */
/* addressing modes, default is direct (relative) */
#define CW_ARG_IMM      (1 << 0)    // immediate
#define CW_ARG_INDIR    (1 << 1)    // indirect
#define CW_ARG_PREDEC   (1 << 2)    // predecrement

#define cwisdat(ins)    ((ins).op == CW_OP_DAT)

#if defined(CW_32BIT_INSTRUCTIONS)

typedef int32_t         cwintop_t;

/* signed value */
#define CW_OPERAND_BITS 11

struct cwinstr {
    unsigned            op     : 4;    // maximum # of instructions is 16
    unsigned            aflg   : 3;    // operand a flags
    unsigned            bflg   : 3;    // operand b flags
    signed              a      : 11;   // operand a; max 0x3ff (1023)
    signed              b      : 11;   // operand b; max 0x3ff (1023)
};

#else

typedef int64_t         cwintop_t;

#define CW_OPERAND_BITS 26

/* 64-bit instruction structure */
struct cwinstr {
    unsigned            op      : 4;    // operation ID
    unsigned            aflg    : 3;    // operand #1 flags
    unsigned            bflg    : 3;    // operand #2 flags
    unsigned            pid     : 1;    // 0 = program #1
    unsigned            sign    : 1;    // signed-operation flag
    unsigned            a       : 26;   // operand #1
    unsigned            b       : 26;   // operand #2
};

#endif

typedef long            cwinstrfunc(long, long);

/* virtual machine structure */
struct cwmars {
    long                runqtab[2][CW_PROCS];   // process run queues
    cwinstrfunc        *functab[CW_MAX_OP + 1]; // instruction handler table
    long                proccnt[2];             // process counts
    long                curproc[2];             // current running process IDs
    long                nturn[2];               // number of turns available
    long                running;                // flag to indicate if running
    long                curpid;                 // current program ID
    char               *prog1name;              // program one path
    char               *prog2name;              // program two path
    char               *pidmap;                 // core owner bitmap
    struct cwinstr     *core;                   // operation memory
    const char        **opnames;                // operation mnemonic table
#if defined(ZEUS) && defined(ZEUSSDL)
    struct zeussdl      zeussdl;
#endif
};

#if defined(ZEUS)
struct zeussel {
    long                last;
    uint8_t            *bmap[CW_CORE_SIZE >> 3];
};
#endif

#endif /* __MJOLNIR_CW_H__ */

