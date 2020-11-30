#ifndef __MJOLNIR_CW_H__
#define __MJOLNIR_CW_H__

#if defined(CW_BIG_CORE) && !defined(CW_CORE_SIZE)
#define CW_CORE_SIZE        8000
#else
#define CW_CORE_SIZE        1024    // fits on a 80x24 terminal in 64 * 16
#endif

#include <corewar/conf.h>
#include <stdio.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(ZEUS) && defined(ZEUSSDL)
#include <corewar/sdl.h>
#elif defined(ZEUS) && defined(ZEUSX11)
#include <corewar/x11.h>
#endif

#define cwwrapval(x)        (((x) < 0)                                  \
                             ? (CW_CORE_SIZE + (x))                     \
                             : ((x) % CW_CORE_SIZE))

#define CW_MAX_TURNS        80000
#define CW_MAX_PROCS        8000
#define CW_MAX_SIZE         100
#define CW_MIN_DIST         100
#define CW_NO_OP            ((1 << CW_OP_BITS) - 1)
#define CW_INVAL            ((struct cwinstr){ 0 })

/* opcodes */
#define CW_OP_DAT           0
#define CW_OP_MOV           1
#define CW_OP_ADD           2
#define CW_OP_SUB           3
#define CW_OP_JMP           4
#define CW_OP_JMZ           5
#define CW_OP_JMN           6
#define CW_OP_CMP           7
#define CW_OP_SLT           8
#define CW_OP_DJN           9
#define CW_OP_SPL           10
#define CW_MAX_OP           10
#define CW_OP_BITS          6

/* flags */
/* addressing modes, default is direct (relative) */
#define CW_ARG_REL          0           // direct (default)
#define CW_ARG_IMM          1           // immediate
#define CW_ARG_INDIR        2           // indirect
#define CW_ARG_PREDEC       3           // predecrement
#define CW_TYPE_BITS        4

#define cwisdat(ins)        ((ins).op == CW_OP_DAT)

#if defined (CW_BIG_CORE)

#define CW_OPERAND_BITS     16

struct cwinstr {
    unsigned                op      : CW_OP_BITS;
    unsigned                red     : 1;
    unsigned                pid     : 1;
    unsigned                atype   : CW_TYPE_BITS;
    unsigned                btype   : CW_TYPE_BITS;
    uint16_t                info;           // reserved; debugger information?
    unsigned                a       : CW_OPERAND_BITS;
    unsigned                b       : CW_OPERAND_BITS;
};

#elif defined(CW_32BIT_INSTRUCTIONS)

typedef int32_t             cwintop_t;

/* signed value */
#define CW_OPERAND_BITS 11

struct cwinstr {
    unsigned                op      : 4;    // maximum # of instructions is 16
    unsigned                atype   : 3;    // operand a flags
    unsigned                btype   : 3;    // operand b flags
    signed                  a       : 11;   // operand a; max 0x3ff (1023)
    signed                  b       : 11;   // operand b; max 0x3ff (1023)
};

#else

typedef int64_t         cwintop_t;

#define CW_OPERAND_BITS 26

/* 64-bit instruction structure */
struct cwinstr {
    unsigned                op      : 4;    // operation ID
    unsigned                aflg    : 3;    // operand #1 flags
    unsigned                bflg    : 3;    // operand #2 flags
    unsigned                pid     : 1;    // 0 = program #1
    unsigned                sign    : 1;    // signed-operation flag
    unsigned                a       : 26;   // operand #1
    unsigned                b       : 26;   // operand #2
};

#endif

typedef long            cwinstrfunc(long, long);

/* virtual machine structure */
struct cwmars {
    cwinstrfunc       **functab;                    // instruction handler table
    long                runqueue[2][CW_MAX_PROCS];  // process run queues
    long                proccnt[2];                 // numbers of processes
    long                progsz[2];                  // process sizes
    long                curproc[2];                 // running process IDs
    long                nturn[2];                   // number of turns left
    long                running;                    // running-state flag
    long                curpid;                     // current program ID
    char               *progpaths[2];               // paths to warrior files
    char               *pidmap;                     // core owner bitmap
    char               *memmap;                     // allocation bitmap
    struct cwinstr     *core;                       // operation memory
    const char        **opnames;                    // operation mnemonic table
#if defined(ZEUS) && defined(ZEUSSDL)
    struct zeussdl      zeussdl;
#elif defined(ZEUS) && defined(ZEUSX11)
    struct zeusx11      zeusx11;
#endif
};

void                        cwinit(void);
void                        cwexec(long pid);
void                        cwdisasm(struct cwinstr op, FILE *fp);
void                        cwprintmars(struct cwmars *mars,
                                        long pid,
                                        long pc);
void                        cwprintinstr(struct cwinstr op,
                                         long pid,
                                         long pc);

#if (CW_CORE_SIZE > (1 << CW_OPERAND_BITS))
#error fix CW_CORE_SIZE or CW_OPERAND_BITS in <corewar/cw.h>
#endif

#endif /* __MJOLNIR_CW_H__ */

