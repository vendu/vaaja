#ifndef __COREWAR_CW_H__
#define __COREWAR_CW_H__

#define CW_32BIT_INSTRUCTIONS

#include <corewar/conf.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(ZEUS)
#include <corewar/zeus.h>
#if defined(ZEUSWINSDL)
#include <corewar/sdl.h>
#endif
#endif

#define cwpow2(x)       (!((x) & ((x) - 1)))
#if (cwpow2(CW_CORE_SIZE))
#define cwwrapcore(a)   ((a) & (CW_CORE_SIZE - 1))
#else
#define cwwrapcore(a)   ((a) % CW_CORE_SIZE)
#endif

void                    cwinit(void);

#define CW_TURNS        (128 * 1024)
#define CW_PROCS        1024
//#define CWINVAL     0xff
//#define CWCORESIZE  8000
#define CW_CORE_SIZE    1024            // fits on a 80x24 terminal in 64 * 16
#define CW_NONE         (~0L)
//#define CW_EMPTY        NULL

/* opcodes */
#define CW_OP_DAT       0
#define CW_OP_MOV       1
#define CW_OP_ADD       2
#define CW_OP_SUB       3
#define CW_OP_JMP       4
#define CW_OP_JMZ       5
#define CW_OP_JMN       6
#define CW_OP_CMP       7
#define CW_OP_SLT       8
#define CW_OP_DJN       9
#define CW_OP_SPL       10
#define CWNOP           11

/* flags */
/* signed value */
/* addressing modes, default is direct (relative) */
#define CW_ADR_IMM      (1 << 0)    // immediate
#define CW_ADR_INDIR    (1 << 1)    // indirect
#define CW_ADR_PREDEC   (1 << 2)    // predecrement

#define cwisdat(ins)    ((ins)->op == CW_OP_DAT)

#if defined(CW_32BIT_INSTRUCTIONS)

#define CW_OPERAND_BITS 10
typedef int16_t         cwarg;

struct cwinstr {
    unsigned op     : 4;    // maximum # of instructions is 16
    unsigned aflg   : 3;    // operand a flags
    unsigned bflg   : 3;    // operand b flags
    unsigned pid    : 1;    // 0 for program #1, 1 for program #2
    unsigned sign   : 1;    // signed-operation flag
    unsigned a      : 10;   // operand a; max 0x3ff (1023)
    unsigned b      : 10;   // operand b; max 0x3ff (1023)
};

#else

#define CW_OPERAND_BITS 26
typedef int32_t         cwarg;

/* 64-bit instruction structure */
struct cwinstr {
    unsigned op     : 4;        // operation ID
    unsigned aflg   : 3;        // operand #1 flags
    unsigned bflg   : 3;        // operand #2 flags
    unsigned pid    : 1;        // 0 for program #1, 1 for program #2
    unsigned sign   : 1;        // signed-operation flag
    unsigned a      : 26;       // operand #1
    unsigned b      : 26;       // operand #2
};

#endif

typedef long            cwinstrfunc(long, long);

/* virtual machine structure */
struct cwmars {
    long                runqtab[2][CW_PROCS];   // process run queues
    cwinstrfunc        *functab[CWNOP];         // instruction handler table
    long                proccnt[2];             // process counts
    long                curproc[2];             // current running process IDs
    long                nturn[2];               // number of turns available
    long                running;                // flag to indicate if running
    long                curpid;                 // current program ID
    char               *prog1name;              // program one path
    char               *prog2name;              // program two path
#if defined(ZEUS) && defined(ZEUSSDL)
    struct zeussdl      zeussdl;
#endif
    char               *pidmap;
    struct cwinstr     *optab;                  // operation lookup structure
    const char        **opnames;                // operation mnemonic table
};

void                    cwexec(long pid);

#endif /* __COREWAR_CW_H__ */

