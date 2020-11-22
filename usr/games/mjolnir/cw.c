/* REFERENCE:  http://corewar.co.uk/cwg.txt */
/* REFERENCE: http://seblog.cs.uni-kassel.de/fileadmin/se/courses/SE1/WS0708/redcode-icws-88-2.pdf */

#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <mach/param.h>
#include <prng/randmt32.h>
#include <mjolnir/cw.h>
#include <mjolnir/rc.h>
#if defined(ZEUS)
#include <unistd.h>
#endif

static long         cwdatop(long pid, long pc);
static long         cwmovop(long pid, long pc);
static long         cwaddop(long pid, long pc);
static long         cwsubop(long pid, long pc);
static long         cwjmpop(long pid, long pc);
static long         cwjmzop(long pid, long pc);
static long         cwjmnop(long pid, long pc);
static long         cwcmpop(long pid, long pc);
static long         cwsltop(long pid, long pc);
static long         cwdjnop(long pid, long pc);
static long         cwsplop(long pid, long pc);

/* virtual machine structure */
struct cwmars       g_cwmars C_ALIGNED(MACH_PAGE_SIZE);
/* instruction handlers */
static cwinstrfunc *g_cwfunctab[CW_MAX_OP + 1]
= {
    cwdatop,
    cwmovop,
    cwaddop,
    cwsubop,
    cwjmpop,
    cwjmzop,
    cwjmnop,
    cwcmpop,
    cwsltop,
    cwdjnop,
    cwsplop
};
/* instruction names */
const char     *g_cwopnametab[CW_MAX_OP + 1]
= {
    "DAT",
    "MOV",
    "ADD",
    "SUB",
    "JMP",
    "JMZ",
    "JMN",
    "CMP",
    "SLT",
    "DJN",
    "SPL",
};

/* disassemble instruction */
void
cwdisasm(struct cwinstr op, FILE *fp)
{
    long                    aflg = op.aflg;
    long                    bflg = op.bflg;
    long                    ndx = op.op;
    long                    ch;

    fprintf(fp, "%s\t", g_cwopnametab[ndx]);
    ch = 0;
    if (aflg & CW_ARG_IMM) {
        ch = '#';
    } else if (aflg & CW_ARG_INDIR) {
        ch = '@';
    } else if (aflg & CW_ARG_PREDEC) {
        ch = '<';
#if 0
    } else {
        ch = '$';
#endif
    }
    if (ch) {
        fprintf(fp, "%c", (char)ch);
    }
    fprintf(fp, "%d\t", op.a);
    ch = 0;
    if (bflg & CW_ARG_IMM) {
        ch = '#';
    } else if (bflg & CW_ARG_INDIR) {
        ch = '@';
    } else if (bflg & CW_ARG_PREDEC) {
        ch = '<';
#if 0
    } else {
        ch = '$';
#endif
    }
    if (ch) {
        fprintf(fp, "%c", (char)ch);
    }
    fprintf(fp, "%d\n", op.b);

    return;
}

/* print instruction to stderr */
void
cwprintinstr(struct cwinstr op, long pid, long pc)
{
    long                val = op.op;
    const char         *name = g_cwmars.opnames[val];
    long                aflg = 0;
    long                bflg = 0;

    if (pid >= 0) {
        fprintf(stderr, "%ld\%ld\t%s", pid, pc, name);
    } else {
        fprintf(stderr, "%ld\t%s", pc, name);
    }
    val = op.a;
    if (aflg & CW_ARG_IMM) {
        fprintf(stderr, "\t#%ld", val);
    } else if (aflg & CW_ARG_INDIR) {
        fprintf(stderr, "\t@%ld", val);
    } else if (aflg & CW_ARG_PREDEC) {
        fprintf(stderr, "\t<%ld", val);
    } else {
        fprintf(stderr, "\t$%ld", val);
    }
    if (op.arg2) {
        val = op.b;
        if (bflg & CW_ARG_IMM) {
            fprintf(stderr, "\t#%ld", val);
        } else if (bflg & CW_ARG_INDIR) {
            fprintf(stderr, "\t@%ld", val);
        } else if (bflg & CW_ARG_PREDEC) {
            fprintf(stderr, "\t<%ld", val);
        } else {
            fprintf(stderr, "\t$%ld", val);
        }
    }
    fprintf(stderr, "\n");

    return;
}

void
cwprintmars(struct cwmars *mars, long pid, long pc)
{
    long                ndx;
    long                cnt;

    fprintf(stderr, "program #1 size: %ld\n", mars->progsz[0]);
    fprintf(stderr, "program #2 size: %ld\n", mars->progsz[1]);
    cnt = mars->proccnt[0];
    fprintf(stderr, "Q0 (%ld):", cnt);
    for (ndx = 0 ; ndx < cnt ; ndx++) {
        fprintf(stderr, " %ld", mars->runqueue[0][ndx]);
    }
    fprintf(stderr, "\n");
    cnt = mars->proccnt[1];
    fprintf(stderr, "Q1 (%ld):", cnt);
    for (ndx = 0 ; ndx < cnt ; ndx++) {
        fprintf(stderr, " %ld", mars->runqueue[1][ndx]);
    }
    fprintf(stderr, "\n");
    cnt = mars->nturn[0];
    fprintf(stderr, "P0: %ld turns left\n", cnt);
    cnt = mars->nturn[1];
    fprintf(stderr, "P1: %ld turns left\n", cnt);
    if (pid >= 0) {
        cwprintinstr(g_cwmars.core[pc], pid, pc);
    }

    return;
}

/* read instruction operands */
static void
cwgetargs(struct cwinstr op, long pc, long *argp1, long *argp2)
{
    struct cwinstr     *instr = &g_cwmars.core[pc];
    struct cwinstr     *src;
    long                aflg = op.aflg;
    long                bflg = op.bflg;
    long                arg1 = op.a;
    long                arg2 = op.b;

    if (aflg) {
        if (aflg & (CW_ARG_INDIR | CW_ARG_PREDEC)) {
            if (aflg & CW_ARG_PREDEC) {
                arg1--;
                arg1 = cwwrapval(arg1);
                instr->a = arg1;
            } else {
                arg1 += pc;
                arg1 = cwwrapval(arg1);
                src = &g_cwmars.core[arg1];
                if (src->arg2) {
                    arg1 = src->b;
                } else {
                    arg1 = src->a;
                }
            }
        }
    } else {
        arg1 += pc;
    }
    if (bflg) {
        if (bflg & (CW_ARG_INDIR | CW_ARG_PREDEC)) {
            if (bflg & CW_ARG_PREDEC) {
                arg2--;
                arg2 = cwwrapval(arg2);
                instr->b = arg1;
            } else {
                arg2 += pc;
                arg2 = cwwrapval(arg2);
                src = &g_cwmars.core[arg2];
                if (src->arg2) {
                    arg2 = src->b;
                } else {
                    arg2 = src->a;
                }
            }
        }
    } else {
        arg2 += pc;
    }
    arg1 = cwwrapval(arg1);
    arg2 = cwwrapval(arg2);
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long                arg1;
    long                arg2;

#if defined(ZEUS) && defined(ZEUSSDL)
    zeusdrawsim(&g_cwmars.zeussdl);
#endif
    cwgetargs(op, pc, &arg1, &arg2);
    if (!pid) {
        fprintf(stderr, "program #2 (%s) won (%ld)\n",
                g_cwmars.progpaths[1], pc);
    } else {
        fprintf(stderr, "program #1 (%s) won (%ld)\n",
                g_cwmars.progpaths[0], pc);
    }
#if defined(ZEUS)
    sleep(5);
#endif

    /* NOTREACHED */
    return pid;
}

/* instruction handler for MOV */
static long
cwmovop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            aflg = op.aflg;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    dest = &g_cwmars.core[arg2];
    if (aflg & CW_ARG_IMM) {
        dest->b = arg1;
    } else {
        *dest = op;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for ADD */
static long
cwaddop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            aflg = op.aflg;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    dest = &g_cwmars.core[arg2];
    if (aflg & CW_ARG_IMM) {
        val = dest->b;
        arg2 += val;
        arg2 = cwwrapval(arg2);
        dest->b = arg2;
    } else {
        val = dest->a;
        arg1 += val;
        arg1 = cwwrapval(arg1);
        dest->a = arg1;
        val = dest->b;
        arg2 += val;
        arg2 = cwwrapval(arg1);
        dest->b = arg2;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for SUB */
static long
cwsubop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            aflg = op.aflg;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    pc = cwwrapval(pc);
    dest = &g_cwmars.core[arg2];
    if (aflg & CW_ARG_IMM) {
        val = dest->b;
        arg2 -= val;
        arg2 = cwwrapval(arg2);
        dest->b = arg2;
    } else {
        val = dest->a;
        arg1 -= val;
        arg1 = cwwrapval(arg2);
        dest->a = arg1;
        val = dest->b;
        arg2 -= val;
        arg2 = cwwrapval(arg2);
        dest->b = arg2;
    }

    return pc;
}

/* instruction handler for JMP */
static long
cwjmpop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc = arg1;
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for JMZ */
static long
cwjmzop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    if (!arg2) {
        pc = arg1;
    } else {
        pc++;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for JMN */
static long
cwjmnop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    if (arg2) {
        pc = arg1;
    } else {
        pc++;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for CMP */
static long
cwcmpop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            aflg = op.aflg;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    if (aflg & CW_ARG_IMM) {
        if (arg2 == dest->b) {
            pc++;
        }
    } else if (arg1 == dest->a && arg2 == dest->b) {
        pc++;
    }
    pc++;
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for SLT */
static long
cwsltop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            aflg = op.aflg;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(op, pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (aflg & CW_ARG_IMM) {
        if (arg1 < val) {
            pc++;
        }
    } else if (arg2 < val) {
        pc++;
    }
    pc++;
    pc = cwwrapval(pc);;

    return pc;
}

/* instruction handler for DJN */
static long
cwdjnop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            bflg = op.bflg;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(op, pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    if (bflg & CW_ARG_IMM) {
        val = instr->b;
        val--;
        val = cwwrapval(val);
        instr->b = val;
    } else {
        val = dest->b;
        val--;
        val = cwwrapval(val);
        dest->b = val;
    }
    if (!val) {
        pc = arg1;
    }

    return pc;
}

/* instruction handler for SPL */
static long
cwsplop(long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    long           *runq = &g_cwmars.runqueue[pid][0];
    long            cnt;
    long            arg1;
    long            arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    cnt = g_cwmars.proccnt[pid];
    pc = cwwrapval(pc);
    if (cnt < CW_PROCS) {
        arg1 = cwwrapval(arg1);
        runq[cnt] = arg1;
        cnt++;
        pc = arg1;
        g_cwmars.proccnt[pid] = cnt;
    }

    return pc;
}

/* initialise instruction handling */
static void
cwinitop(void)
{
    g_cwmars.functab = g_cwfunctab;
    g_cwmars.opnames = g_cwopnametab;
    g_cwmars.functab[CW_OP_DAT] = cwdatop;
    g_cwmars.functab[CW_OP_MOV] = cwmovop;
    g_cwmars.functab[CW_OP_ADD] = cwaddop;
    g_cwmars.functab[CW_OP_SUB] = cwsubop;
    g_cwmars.functab[CW_OP_JMP] = cwjmpop;
    g_cwmars.functab[CW_OP_JMZ] = cwjmzop;
    g_cwmars.functab[CW_OP_JMN] = cwjmnop;
    g_cwmars.functab[CW_OP_CMP] = cwcmpop;
    g_cwmars.functab[CW_OP_SLT] = cwsltop;
    g_cwmars.functab[CW_OP_DJN] = cwdjnop;
    g_cwmars.functab[CW_OP_SPL] = cwsplop;
    rcaddop("DAT", CW_OP_DAT);
    rcaddop("MOV", CW_OP_MOV);
    rcaddop("ADD", CW_OP_ADD);
    rcaddop("SUB", CW_OP_SUB);
    rcaddop("JMP", CW_OP_JMP);
    rcaddop("JMZ", CW_OP_JMZ);
    rcaddop("JMN", CW_OP_JMN);
    rcaddop("CMP", CW_OP_CMP);
    rcaddop("SLT", CW_OP_SLT);
    rcaddop("DJN", CW_OP_DJN);
    rcaddop("SPL", CW_OP_SPL);
}

/* execute program pid for one instruction */
void
cwexec(long pid)
{
    struct cwinstr  op;
    cwinstrfunc    *func;
    long            cur;
    long            cnt;
    long            pc;
    long            l;
#if defined(ZEUS) && 0
    static long     ref = 0;
#endif

#if defined(ZEUS) && defined(ZEUSSDL) && 0
    while (XPending(g_cwmars.zeusx11.disp)) {
        zeusprocev(&g_cwmars.zeusx11.disp);
    }
#endif
    cur = g_cwmars.curproc[pid];
    pc = g_cwmars.runqueue[pid][cur];
    op = g_cwmars.core[pc];
    if (cwisdat(op)) {
#if defined(ZEUS) && defined(ZEUSSDL)
        zeusdrawsim(&g_cwmars.zeusx11);
#endif
        if (!pid) {
            fprintf(stderr, "program #2 (%s) won (%ld)\n",
                    g_cwmars.progpaths[1], pc);
        } else {
            fprintf(stderr, "program #1 (%s) won (%ld)\n",
                    g_cwmars.progpaths[0], pc);
        }
#if defined(ZEUS)
        sleep(5);
#endif

        exit(0);
    }
    func = g_cwmars.functab[op.op];
    pc = func(pid, pc);
    cnt = g_cwmars.proccnt[pid];
    if (op.op == CW_NO_OP) {
        if (cnt > 1) {
            for (l = cur ; l < cnt - 1 ; l++) {
                g_cwmars.runqueue[pid][l] = g_cwmars.runqueue[pid][l + 1];
            }
        } else {
#if defined(ZEUS) && defined(ZEUSSDL)
            zeusdrawsim(&g_cwmars.zeusx11);
#endif
            fprintf(stderr, "program #%ld (%s) won (%ld)\n",
                    pid, g_cwmars.progpaths[pid], pc);
#if defined(ZEUS) && defined(ZEUSSDL)
            sleep(5);
#endif

            exit(0);
        }
        cnt--;
        g_cwmars.proccnt[pid] = cnt;
    } else if (op.op != CW_OP_SPL) {
        g_cwmars.runqueue[pid][cur] = pc;
        cur++;
    }
    cnt = g_cwmars.proccnt[pid];
    if (cur == cnt) {
        cur = 0;
    }
    g_cwmars.curproc[pid] = cur;
#if defined(ZEUS) && defined(ZEUSSDL)
    //    ref++;
    //    if (!g_cwmars.running || ref == 32) {
    zeusdrawsim(&g_cwmars.zeusx11);
    //        ref = 0;
    //    }
#endif

    return;
}

/* virtual machine main loop */
C_NORETURN
void
cwloop(long nturn)
{
    long                pid = g_cwmars.curpid;

    while (nturn--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    }
    fprintf(stderr, "TIE\n");
#if defined(ZEUS) && defined(ZEUSSDL)
    sleep(5);
#endif

    exit(0);
}

/* initialise virtual machine */
void
cwinit(void)
{
    size_t              csize = CW_CORE_SIZE * sizeof(struct cwinstr);
    void               *ptr;

    time_t seed32 = (((time(NULL) & 0xff) << 24)
                     | ((time(NULL) & 0xff) << 16)
                     | ((time(NULL) & 0xff) << 8)
                     | (time(NULL) & 0xff));

#if defined(CWRANDMT32)
    srandmt32(seed32);
#else
    srand(time(&seed32));
#endif
    cwinitop();
    rcinitop();
    ptr = malloc(csize);
    if (!ptr) {
        fprintf(stderr, "failed to allocate core\n");

        exit(1);
    }
    memset(ptr, -1, csize);
    g_cwmars.core = ptr;
    ptr = calloc(CW_CORE_SIZE / CHAR_BIT, sizeof(char));
    if (!ptr) {
        fprintf(stderr, "failed to allocate pidmap\n");

        exit(1);
    }
    g_cwmars.pidmap = ptr;

    return;
}

void
cwinitmars(struct cwmars *mars, long pc1, long pc2, long nturn)
{
    mars->runqueue[0][0] = pc1;
    mars->runqueue[1][0] = pc2;
    mars->proccnt[0] = 1;
    mars->proccnt[1] = 1;
    mars->curproc[0] = 0;
    mars->curproc[1] = 0;
    mars->nturn[0] = nturn;
    mars->nturn[1] = nturn;
    mars->curpid = randmt32() & 0x01L;

    return;
}

long
cwloadfile(const char *name, long base, long *sizeret)
{
    FILE               *fp;
    long                pc;
    long                adr;
    long                lim;
    long                size;
    long                csize;

    fp = fopen(name, "r");
    if (!fp) {
        fprintf(stderr, "failed to open file %s\n", name);

        exit(1);
    }
    pc = rcxlatef(fp, 0, base, &adr, &lim);
    size = lim - adr;
    if (g_cwmars.memmap) {
        csize = CW_CORE_SIZE;
        if (!powerof2(CW_CORE_SIZE)) {
            csize /= CHAR_BIT;
            csize++;
        } else {
            csize >>= 3;
        }
        g_cwmars.memmap = calloc(csize, sizeof(char));
        if (!g_cwmars.memmap) {
            fprintf(stderr, "failed to allocate memory bitmap\n");

            exit(1);
        }
        while (adr < lim) {
            setbit(&g_cwmars.memmap, adr);
            adr++;
        }
    } else {
        while (adr < lim) {
            if (bitset(&g_cwmars.memmap, adr)) {
                fprintf(stderr, "programs overlap at address %ld\n",
                        adr);

                exit(1);
            }
            adr++;
        }
        free(g_cwmars.memmap);
        g_cwmars.memmap = NULL;
    }
    if (sizeret) {
        *sizeret = size;
    }
    fclose(fp);

    return pc;
}

int
main(int argc, char *argv[])
{
    long                base;
    long                size;
    long                lim;
    long                pc1;
    long                pc2;

#if defined(ZEUS)
#if defined(ZEUSSDL)
    zeusinitx11(&g_cwmars.zeusx11);
#elif defined(ZEUSWINEFL)
    zeusinitefl(&g_cwmars.zeusefl);
#endif
#endif
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.red prog2.red\n", argv[0]);

        exit(1);
    }
    cwinit();
    base = randmt32();
    base = cwwrapval(base);
    pc1 = cwloadfile(argv[1], base, &size);
    g_cwmars.progsz[0] = size;
    do {
        base = randmt32();
        base = cwwrapval(base);
        lim = base + size;
    } while (base >= pc1 && base < lim);
    pc2 = cwloadfile(argv[2], base, &size);
    g_cwmars.progsz[1] = size;
    cwinitmars(&g_cwmars, pc1, pc2, CW_TURNS);
    g_cwmars.progpaths[0] = argv[1];
    g_cwmars.progpaths[1] = argv[2];
#if defined(ZEUS) && defined(ZEUSSDL)
    while (1) {
        zeusdrawsim(&g_cwmars.zeussdl);
        zeusprocev(&g_cwmars.zeussdl);
    }
#else
    cwloop(CW_TURNS);
#endif

    /* NOTREACHED */
    exit(0);
}

