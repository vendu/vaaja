#ifndef __MJOLNIR_CW_H__
#define __MJOLNIR_CW_H__

#if defined(CW_BIG_CORE) && !defined(CW_CORE_SIZE)
#define CW_CORE_SIZE        8000
#else
#define CW_CORE_SIZE        1024    // fits on a 80x24 terminal in 64 * 16
#endif
#define CW_CORE_ADR_BITS

#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(ZEUS) && defined(ZEUSSDL)
#include <mjolnir/sdl.h>
#endif

#define cwwrapval(x)        (((x) < 0)                                  \
                             ? (CW_CORE_SIZE + (x))                     \
                             : ((x) % CW_CORE_SIZE))

#define CW_TURNS            80000
#define CW_PROCS            8000
#define CW_NO_OP            0x3f
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

/* flags */
/* addressing modes, default is direct (relative) */
#define CW_ARG_IMM          (1 << 0)        // immediate
#define CW_ARG_INDIR        (1 << 1)        // indirect
#define CW_ARG_PREDEC       (1 << 2)        // predecrement

#define cwisdat(ins)        ((ins).op == CW_OP_DAT)

#if defined (CW_BIG_CORE)

typedef int8_t              cwintop_t;

#define CW_OPERAND_BITS     20

struct cwinstr {
    unsigned                op      : 6;
    //    unsigned                arg2    : 1;
    unsigned                brk     : 1;
    unsigned                _pad    : 1;
    uint8_t                 aflg;
    uint8_t                 bflg;
    unsigned                a       : 20;
    unsigned                b       : 20;
};

#elif defined(CW_32BIT_INSTRUCTIONS)

typedef int32_t             cwintop_t;

/* signed value */
#define CW_OPERAND_BITS 11

struct cwinstr {
    unsigned                op      : 4;    // maximum # of instructions is 16
    unsigned                aflg    : 3;    // operand a flags
    unsigned                bflg    : 3;    // operand b flags
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
    cwinstrfunc       **functab;                // instruction handler table
    long                runqueue[2][CW_PROCS];  // process run queues
    long                proccnt[2];             // numbers of processes
    long                progsz[2];              // process sizes in instructions
    long                curproc[2];             // current running process IDs
    long                nturn[2];               // number of turns available
    long                running;                // flag to indicate if running
    long                curpid;                 // current program ID
    char               *progpaths[2];           // paths to warrior files
    char               *pidmap;                 // core owner bitmap
    char               *memmap;                 // allocation bitmap
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

void                        cwinit(void);
void                        cwexec(long pid);
void                        cwdisasm(struct cwinstr op, FILE *fp);
void                        cwprintmars(struct cwmars *mars,
                                        long pid,
                                        long pc);
void                        cwprintinstr(struct cwinstr op,
                                         long pid,
                                         long pc);

#endif /* __MJOLNIR_CW_H__ */

