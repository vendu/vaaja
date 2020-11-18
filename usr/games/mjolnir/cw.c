/* REFERENCE:  http://corewar.co.uk/cwg.txt */
/* REFERENCE: http://seblog.cs.uni-kassel.de/fileadmin/se/courses/SE1/WS0708/redcode-icws-88-2.pdf */

#define CW_ZEUS         1

//#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <mjolnir/util.h>
#include <mjolnir/cw.h>
#include <mjolnir/rc.h>
#if defined(ZEUS) && defined(ZEUSSDL)
#include <mjolnir/sdl.h>
#endif
#if defined(CW_DEBUG)
#include <assert.h>
#endif

extern long             g_rcnargtab[CW_MAX_OP + 1];

struct cwmars           g_cwmars C_ALIGNED(MACH_PAGE_SIZE); // virtual machine
const char             *g_cwopnametab[CW_MAX_OP + 1]        // instruction names
= {
    "---",
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
#if defined(ZEUS)
struct zeussel          g_zeussel;
#endif

/* disassemble instruction */
void
cwdisasm(struct cwinstr *op, FILE *fp)
{
    char                ch;

    if (*(cwintop_t *)op) {
        fprintf(fp, "\t%s\t", g_cwopnametab[op->op]);
        ch = '\0';
        if (op->aflg & CW_ARG_IMM) {
            ch = '#';
        } else if (op->aflg & CW_ARG_INDIR) {
            ch = '@';
        } else if (op->aflg & CW_ARG_PREDEC) {
            ch = '<';
        }
        if (ch) {
            fprintf(fp, "%c", ch);
        }
        fprintf(fp, "%d", op->a);
        if  (g_rcnargtab[op->op] == 2) {
            ch = '\0';
            if (op->bflg & CW_ARG_IMM) {
                ch = '#';
            } else if (op->bflg & CW_ARG_INDIR) {
                ch = '@';
            } else if (op->aflg & CW_ARG_PREDEC) {
                ch = '<';
            }
            if (ch) {
                fprintf(fp, "\t%c", ch);
            } else {
                fprintf(fp, "\t");
            }
            fprintf(stderr, "%d\n", op->b);
        }
    }

    return;
}

/* read instruction operands */
static void
cwgetargs(struct cwinstr op, long pc, long *argp1, long *argp2)
{
    struct cwinstr     *ptr = NULL;
    long                narg = g_rcnargtab[op.op];
    long                ofs = pc;
    long                arg1 = 0;
    long                arg2 = 0;

    arg1 = op.a;
    if (op.aflg) {
        arg1 += ofs;
        arg1 = cwwrapcore(arg1);
        if (op.aflg & (CW_ARG_INDIR | CW_ARG_PREDEC)) {
            ptr = &g_cwmars.core[arg1];
            arg2 = ptr->b;
            ofs = arg1;
            if (op.aflg & CW_ARG_PREDEC) {
                arg2--;
                ptr->b = arg2;
            }
        }
    } else {
        arg1 = pc;
    }
    arg1 = cwwrapcore(arg1);
    if (narg == 2) {
        if (op.bflg) {
            arg2 += ofs;
            arg2 = cwwrapcore(arg2);
            if (op.bflg & (CW_ARG_INDIR | CW_ARG_PREDEC)) {
                ptr = &g_cwmars.core[arg2];
                arg2 = ptr->b;
                if (op.bflg & CW_ARG_PREDEC) {
                    arg2--;
                    ptr->b = arg2;
                }
            }
        }
    } else {
        arg2 = pc;
    }
    arg2 = cwwrapcore(arg2);
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long                arg1;
    long                arg2;

    cwgetargs(op, pc, &arg1, &arg2);

    return -1;
}

/* instruction handler for MOV */
static long
cwmovop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    if (op.aflg & CW_ARG_IMM) {
        ptr->b = arg1;
    } else {
        g_cwmars.core[arg2] = g_cwmars.core[arg1];
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for ADD */
static long
cwaddop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;
    long                a;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    a = op.a;
    if (op.aflg & CW_ARG_IMM) {
        ptr->b += a;
    } else {
        b = op.b;
        ptr->a += a;
        ptr->b += b;
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for SUB */
static long
cwsubop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;
    long                a;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    a = op.a;
    if (op.aflg & CW_ARG_IMM) {
        ptr->b -= a;
    } else {
        b = op.b;
        ptr->a -= a;
        ptr->b -= b;
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for JMP */
static long
cwjmpop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long                arg1;
    long                arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc = arg1;

    return pc;
}

/* instruction handler for JMZ */
static long
cwjmzop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long                arg1;
    long                arg2;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = op.b;
    if (!b) {
        b = g_cwmars.core[arg1].b;
        pc = arg1;
        pc = cwwrapcore(pc);
    } else {
        pc++;
        pc = cwwrapcore(pc);
    }

    return pc;
}

/* instruction handler for JMN */
static long
cwjmnop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long                arg1;
    long                arg2;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = op.b;
    if (b) {
        pc = arg1;
        pc = cwwrapcore(pc);
    } else {
        pc++;
        pc = cwwrapcore(pc);
    }

    return pc;
}

/* instruction handler for CMP */
static long
cwcmpop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;
    long                a;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    pc++;
    a = ptr->a;
    b = ptr->b;
    pc = cwwrapcore(pc);
    if (op.aflg & CW_ARG_IMM) {
        if (arg1 == b) {
            pc++;
        }
    } else {
        if (op.a == a && op.b == b) {
            pc++;
        }
    }

    return pc;
}

/* instruction handler for SLT */
static long
cwsltop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;
    long                a;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    pc++;
    a = op.a;
    b = ptr->b;
    pc = cwwrapcore(pc);
    if ((op.aflg & CW_ARG_IMM)
        && arg1 < b) {
        pc++;
    } else if (a < b) {
        pc++;
    }

    return pc;
}

/* instruction handler for DJN */
static long
cwdjnop(C_UNUSED long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    struct cwinstr     *ptr;
    long                arg1;
    long                arg2;
    long                b;

    cwgetargs(op, pc, &arg1, &arg2);
    ptr = &g_cwmars.core[arg2];
    if (op.bflg & CW_ARG_IMM) {
        b = op.b;
        b--;
        g_cwmars.core[pc].b = b;
    } else {
        b = ptr->b;
        b--;
        ptr->b = b;
    }
    if (b) {
        pc = cwwrapcore(arg1);
    }

    return pc;
}

/* instruction handler for SPL */
static long
cwsplop(long pid, long pc)
{
    struct cwinstr      op = g_cwmars.core[pc];
    long               *runq;
    long                cnt;
    long                ndx;
    long                arg1;
    long                arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    cnt = g_cwmars.proccnt[pid];
    runq = &g_cwmars.runqtab[pid][0];
    pc = cwwrapcore(pc);
    for (ndx = 0 ; ndx < cnt - 1 ; ndx++) {
        runq[ndx] = runq[ndx + 1];
    }
    runq[ndx] = arg1;

    return pc;
}

/* initialise instruction handling */
static void
cwinitop(void)
{
    g_cwmars.opnames = g_cwopnametab;
    g_cwmars.functab[0] = NULL;
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
    rcinitop();

    return;
}

/* execute program pid for one instruction */
void
cwexec(long pid)
{
    struct cwinstr      op;
    cwinstrfunc        *func;
    long                pc;
    long                ret;
    long               *runq;
    long                cur;
    long                cnt;
    long                ndx;
#if defined(ZEUS)
    static long         nstep = 0;
#endif

#if defined(ZEUS) && defined(ZEUSSDL) && 0
    while (XEventsQueued(g_cwmars.zeusx11.disp, QueuedAfterFlush)) {
        zeusprocev(&g_cwmars.zeusx11);
    }
#endif
    cur = g_cwmars.curproc[pid];
    cnt = g_cwmars.proccnt[pid];
    pc = g_cwmars.runqtab[pid][cur];
    op = g_cwmars.core[pc];
    assert(pc >= 0 && pc <= CW_CORE_SIZE);
    fprintf(stderr, "%ld\t%ld\t", pid, pc);
    if (cwisdat(op)) {
#if defined(ZEUS) && defined(ZEUSWINX11)
        zeusdrawsim(&g_cwmars.zeusx11);
#endif
        if (!pid) {
            fprintf(stderr, "program #2 (%s) won (%ld)\n", g_cwmars.prog2name, pc);
        } else {
            fprintf(stderr, "program #1 (%s) won (%ld)\n", g_cwmars.prog1name, pc);
        }
#if defined(ZEUS)
        sleep(5);
#endif

        exit(0);
    }
    func = g_cwmars.functab[op.op];
    ret = func(pid, pc);
    if (ret < 0) {
        cnt--;
        if (!cnt) {
#if defined(ZEUS) && defined(ZEUSWINX11)
            zeusdrawsim(&g_cwmars.zeusx11);
#endif
            if (!pid) {
                fprintf(stderr, "program #2 (%s) won (%ld)\n", g_cwmars.prog2name, pc);
            } else {
                fprintf(stderr, "program #1 (%s) won (%ld)\n", g_cwmars.prog1name, pc);
            }
#if defined(ZEUS) && defined(ZEUSWINX11)
            sleep(5);
#endif

            exit(0);
            g_cwmars.proccnt[pid] = cnt;
        }
    } else if (op.op != CW_OP_SPL) {
        runq = &g_cwmars.runqtab[pid][0];
        if (cnt > 1) {
            for (ndx = 0 ; ndx < cnt - 1 ; ndx++) {
                runq[ndx] = runq[ndx + 1];
            }
        }
        runq[cnt - 1] = ret;
    }
    cnt = g_cwmars.proccnt[pid];
    if (cnt > 1) {
        cur = g_cwmars.curproc[pid];;
        cur++;
        if (cur == cnt) {
            cur = 0;
        }
        g_cwmars.curproc[pid] = cur;
    }
#if defined(ZEUS) && defined(ZEUSSDL)
    nstep++;
    if (!g_cwmars.running || nstep == 32) {
        zeusdrawsim(&g_cwmars.zeussdl);
        nstep = 0;
    }
#endif

    return;
}

/* virtual machine main loop */
static void
cwloop(void)
{
    long                pid = g_cwmars.curpid;

    //    while (g_cwmars.nturn[pid]--) {
    while (1) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    }
    fprintf(stderr, "TIE\n");
#if defined(ZEUS)
    sleep(5);
#endif

    return;
}

/* initialise virtual machine */
static void
cwinitvm(void)
{
    time_t              seed32 = (((time(NULL) & 0xff) << 24)
                                  | ((time(NULL) & 0xff) << 16)
                                  | ((time(NULL) & 0xff) << 8)
                                  | (time(NULL) & 0xff));

    seedrand(seed32);
    cwinitop();
    g_cwmars.pidmap = calloc(CW_CORE_SIZE / CHAR_BIT, sizeof(char));
    g_cwmars.core = calloc(CW_CORE_SIZE, sizeof(struct cwinstr));
    if (!g_cwmars.core) {
        fprintf(stderr, "failed to allocate core\n");

        exit(1);
    }

    return;
}

static long
cwinitprog(const char *filename, long pid, long *baseret, long *limret)
{
    FILE               *fp;
    long                pc;
    long                base;
    long                lim;

    base = getrand();
    base = cwwrapcore(base);
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", filename);

        exit(1);
    }
    pc = rcxlate(fp, pid, base, &base, &lim);
    fclose(fp);
    *baseret = base;
    *limret = lim;

    return pc;
}

static void
cwinitmars(struct cwmars *mars, char *prog1name, char *prog2name)
{
    long                base1;
    long                lim1;
    long                pc1;
    long                base2;
    long                lim2;
    long                pc2;

#if defined(ZEUS) && defined(ZEUSSDL)
    if (sdlinit(&mars->zeussdl)) {
        fprintf(stderr, "failed to initialize SDL\n");

        exit(1);
    }
#endif
    while (1) {
        pc1 = cwinitprog(prog1name, 0, &base1, &lim1);
        if (pc1 < 0) {
            fprintf(stderr, "failed to translate %s\n", prog1name);

            exit(1);
        }
        pc2 = cwinitprog(prog2name, 1, &base2, &lim2);
        if (pc2 < 0) {
            fprintf(stderr, "failed to translate %s\n", prog2name);

            exit(1);
        }
        if ((base2 >= base1 && base2 < lim1)
            || (lim2 >= base1 && lim2 < lim1)) {
            fprintf(stderr, "programs overlap, retrying\n");

            continue;
        }

        break;
    }
    mars->proccnt[0] = 1;
    mars->proccnt[1] = 1;
    mars->curproc[0] = 0;
    mars->curproc[1] = 0;
    mars->nturn[0] = CW_TURNS;
    mars->nturn[1] = CW_TURNS;
    mars->runqtab[0][0] = pc1;
    mars->runqtab[1][0] = pc2;
    mars->curpid = getrand() & 0x00000001;
    mars->prog1name = prog1name;
    mars->prog2name = prog2name;

    return;
}

void
cwrun(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.rc prog2.rc\n", argv[0]);

        exit(1);
    }
    cwinitvm();
    cwinitmars(&g_cwmars, argv[1], argv[2]);
    cwloop();

    return;
}

#if (TEST_CW)
int
main(int argc, char *argv[])
{
    cwrun(argc, argv);

    exit(0);
}
#endif

