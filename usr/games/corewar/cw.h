#ifndef __COREWAR_CW_H__
#define __COREWAR_CW_H__

#include <corewar/conf.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(ZEUS)
#include <corewar/zeus.h>
#if defined(ZEUSWINSDL)
#include <corewar/sdl.h>
#endif
#endif

void                            cwinit(void);

#define CW_MAX_TURNS            (128 * 1024)
#define CW_MAX_PROCS            1024
#define CW_CORE_SIZE            8192
#define CW_NONE                 (~0L)

/* opcodes */
#define CW_DAT_OP               0
#define CW_MOV_OP               1
#define CW_ADD_OP               2
#define CW_SUB_OP               3
#define CW_JMP_OP               4
#define CW_JMZ_OP               5
#define CW_JMN_OP               6
#define CW_CMP_OP               7
#define CW_SLT_OP               8
#define CW_DJN_OP               9
#define CW_SPL_OP               10
#define CW_OPS                  11

/* flags */
/* signed value */
#define CWSIGNED    			0x00
/* addressing modes, default is direct (relative) */
#define CWADRREL    			0x01        // relative (default)
#define CWADRIMM    			0x02        // immediate
#define CWADRINDIR  			0x03        // indirect
#define CWADRPREDEC 			0x04

/* instruction flags */
#define CWSIGNBIT   			(1U << CWSIGNED)
#define CWRELBIT    			(1U << CWADRREL)
#define CWIMMBIT    			(1U << CWADRIMM)
#define CWINDIRBIT  			(1U << CWADRINDIR)
#define CWPREDECBIT 			(1U << CWADRPREDEC)

/* 64-bit instruction structure */
#define cwisdat(ins)    		((ins)->op == CW_DAT_OP)
struct cwinstr {
    unsigned                    op   : 8;          // operation ID
    unsigned                    aflg : 8;          // operand #1 flags
    unsigned                    bflg : 8;          // operand #2 flags
    unsigned                    pid  : 8;          // process ID
    unsigned                    a    : 16;         // operand #1
    unsigned                    b    : 16;         // operand #2
};

typedef long cwinstrfunc(long, long);

/* virtual machine structure */
struct cwmars {
    long                        runqtab[2][CW_MAX_PROCS]; // process run queues
    cwinstrfunc                *functab[CW_OPS];        // instruction handlers
    long                        proccnt[2];             // process counts
    long                        curproc[2];             // running process IDs
    long                        nturn[2];               // number of turns left
    long                        running;                // mars running-bit
    long                        curpid;                 // current program ID
    char                       *prog1name;              // program one path
    char                       *prog2name;              // program two path
#if defined(ZEUSWINSDL)
    struct zeussdl              zeussdl;
#endif
#if defined(ZEUSWINX11)
    struct zeusx11              zeusx11;                // X11 GUI structure
#endif
    char                       *pidmap;
    struct cwinstr             *optab;                  // operation lookup
    const char                **opnames;                // operation mnemonics
};

void        cwexec(long pid);

#endif /* __COREWAR_CW_H__ */

