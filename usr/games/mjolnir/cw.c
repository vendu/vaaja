/* REFERENCE:  http://corewar.co.uk/cwg.txt */
/* REFERENCE: http://seblog.cs.uni-kassel.de/fileadmin/se/courses/SE1/WS0708/redcode-icws-88-2.pdf */

//#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(CW_RANDMT32)
#include <zero/randmt32.h>
#endif
#include <mjolnir/cw.h>
#include <mjolnir/rc.h>
#if defined(ZEUS)
#include <unistd.h>
#include <mjolnir/zeus.h>
#endif

extern long     g_rcnargtab[CWNOP];

struct cwmars   g_cwmars C_ALIGNED(MACH_PAGE_SIZE); // virtual machine structure
const char     *g_cwopnametab[CWNOP]                // instruction name table
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

/* disassembe instruction */
void
cwdisasm(struct cwinstr *op, FILE *fp)
{
    char                ch;

    if (op) {
        fprintf(fp, "\t%s\t", g_cwopnametab[op->op]);
        if  (g_rcnargtab[op->op] == 2) {
            ch = '\0';
            if (op->aflg & CW_ADR_IMM) {
                ch = '#';
            } else if (op->aflg & CW_ADR_INDIR) {
                ch = '@';
            } else if (op->aflg & CW_ADR_PREDEC) {
                ch = '<';
            }
            if (ch) {
                fprintf(fp, "%c", ch);
            }
            if (op->sign) {
                fprintf(fp, "%d", op->a - CW_CORE_SIZE);
            } else {
                fprintf(fp, "%d", op->a);
            }
        }
        ch = '\0';
        if (op->bflg & CW_ADR_IMM) {
            ch = '#';
        } else if (op->bflg & CW_ADR_INDIR) {
            ch = '@';
        } else if (op->aflg & CW_ADR_PREDEC) {
            ch = '<';
        }
        if (ch) {
            fprintf(fp, "\t%c", ch);
        } else {
            fprintf(fp, "\t");
        }
        if (op->sign) {
            fprintf(fp, "%d\n", op->b - CW_CORE_SIZE);
        } else {
            fprintf(stderr, "%d\n", op->b);
        }
    }

    return;
}

/* read instruction operands */
static void
cwgetargs(struct cwinstr *op, long pc, cwarg *argp1, cwarg *argp2)
{
    cwarg               arg1 = 0;
    cwarg               arg2 = 0;
    cwarg               tmp;

    if (op->aflg & CW_ADR_IMM) {
        arg1 = op->a;
    } else {
        tmp = pc + op->a;
        tmp = cwwrapcore(tmp);
        if (op->aflg & (CW_ADR_INDIR | CW_ADR_PREDEC)) {
            struct cwinstr *ptr;

            ptr = &g_cwmars.optab[tmp];
            tmp = ptr->b;
            if (op->aflg & CW_ADR_PREDEC) {
                tmp--;
                ptr->b = tmp;
            }
            arg1 = tmp;
            arg1 += pc;
        } else {
            arg1 = tmp;
        }
    }
    arg1 = cwwrapcore(arg1);
    if (op->bflg & CW_ADR_IMM) {
        arg2 = op->b;
    } else {
        tmp = pc + op->b;
        tmp = cwwrapcore(tmp);
        if (op->bflg & (CW_ADR_INDIR | CW_ADR_PREDEC)) {
            struct cwinstr *ptr;

            ptr = &g_cwmars.optab[tmp];
            tmp = ptr->b;
            if (op->bflg & CW_ADR_PREDEC) {
                tmp--;
                ptr->b = tmp;
            }
            arg2 = tmp;
            arg2 += pc;
        } else {
            arg2 = tmp;
        }
    }
    arg2 = cwwrapcore(arg2);
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(long pid, long pc)
{
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

    /* NOTREACHED */
    return CW_NONE;
}

/* instruction handler for MOV */
static long
cwmovop(C_UNUSED long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    cwarg               arg1;
    cwarg               arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CW_ADR_IMM) {
        if (op->bflg & CW_ADR_IMM) {
            g_cwmars.optab[arg2] = g_cwmars.optab[arg1];
        } else {
            g_cwmars.optab[arg2].b = arg1;
        }
    } else {
        g_cwmars.optab[arg2] = g_cwmars.optab[arg1];
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for ADD */
static long
cwaddop(C_UNUSED long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    cwarg               arg1;
    cwarg               arg2;
    cwarg               a;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CW_ADR_IMM) {
        a = arg1;
        if (op->bflg & CW_ADR_IMM) {
            b = arg2;
        } else {
            b = g_cwmars.optab[arg2].b;
        }
        b += a;
        b = cwwrapcore(b);
        if (op->bflg & CW_ADR_IMM) {
            op->sign = 0;
            op->b = b;
        } else {
            g_cwmars.optab[arg2].sign = 0;
            g_cwmars.optab[arg2].b = b;
        }
    } else if (op->bflg & CW_ADR_IMM) {
        a = arg1;
        b = arg2;
        b += a;
        b = cwwrapcore(b);
        op->b = b;
    } else {
        a = g_cwmars.optab[arg1].a;
        b = g_cwmars.optab[arg1].b;
        a += g_cwmars.optab[arg2].a;
        b += g_cwmars.optab[arg2].b;
        a = cwwrapcore(a);
        b = cwwrapcore(a);
        g_cwmars.optab[arg2].a = a;
        g_cwmars.optab[arg2].b = b;
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for SUB */
static long
cwsubop(C_UNUSED long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    cwarg               arg1;
    cwarg               arg2;
    cwarg               a;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CW_ADR_IMM) {
        a = arg1;
        b = g_cwmars.optab[arg2].b;
        b -= a;
        if (b < 0) {
            b += CW_CORE_SIZE;
        }
        g_cwmars.optab[arg2].b = b;
    } else {
        a = g_cwmars.optab[arg1].a;
        b = g_cwmars.optab[arg1].b;
        a -= g_cwmars.optab[arg2].a;
        b -= g_cwmars.optab[arg2].b;
        if (a < 0) {
            a += CW_CORE_SIZE;
        }
        if (b < 0) {
            b += CW_CORE_SIZE;
        }
        g_cwmars.optab[arg2].a = a;
        g_cwmars.optab[arg2].b = b;
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for JMP */
static long
cwjmpop(long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    long                cnt;
    cwarg               arg1;
    cwarg               arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    cnt = g_cwmars.proccnt[pid];
    if (cnt < CW_PROCS) {
        pc = arg2;
        g_cwmars.runqtab[pid][cnt - 1] = pc;
    }

    return pc;
}

/* instruction handler for JMZ */
static long
cwjmzop(long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    long                cnt;
    cwarg               arg1;
    cwarg               arg2;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = g_cwmars.optab[arg2].b;
    if (!b) {
        cnt = g_cwmars.proccnt[pid];
        pc = arg1;
        g_cwmars.runqtab[pid][cnt - 1] = pc;
    } else {
        pc++;
        pc = cwwrapcore(pc);
    }

    return pc;
}

/* instruction handler for JMN */
static long
cwjmnop(long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    long                cnt;
    cwarg               arg1;
    cwarg               arg2;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = g_cwmars.optab[arg2].b;
    if (b) {
        cnt = g_cwmars.proccnt[pid];
        pc = arg1;
        g_cwmars.runqtab[pid][cnt - 1] = pc;
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
    struct cwinstr     *op = &g_cwmars.optab[pc];
    cwarg               arg1;
    cwarg               arg2;
    cwarg               a;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->aflg & CW_ADR_IMM) {
        b = g_cwmars.optab[arg2].b;
        if (arg1 == b) {
            pc++;
        }
    } else {
        a = arg1;
        b = arg2;
        if (g_cwmars.optab[a].a == g_cwmars.optab[b].a && g_cwmars.optab[a].b == g_cwmars.optab[b].b) {
            pc++;
        }
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for SLT */
static long
cwsltop(C_UNUSED long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    cwarg               arg1;
    cwarg               arg2;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    b = g_cwmars.optab[arg2].b;
    if (op->aflg & CW_ADR_IMM) {
        if (arg1 < b) {
            pc++;
        }
    } else if (g_cwmars.optab[arg2].b < b) {
        pc++;
    }
    pc++;
    pc = cwwrapcore(pc);

    return pc;
}

/* instruction handler for DJN */
static long
cwdjnop(long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    long                cnt;
    cwarg               arg1;
    cwarg               arg2;
    cwarg               b;

    cwgetargs(op, pc, &arg1, &arg2);
    if (op->bflg & CW_ADR_IMM) {
        b = g_cwmars.optab[arg1].b;
        b--;
        if (b < 0) {
            b += CW_CORE_SIZE;
        }
        g_cwmars.optab[arg1].b = b;
    } else {
        b = g_cwmars.optab[arg2].b;
        b--;
        if (b < 0) {
            b += CW_CORE_SIZE;
        }
        g_cwmars.optab[arg2].b = b;
    }
    if (b) {
        cnt = g_cwmars.proccnt[pid];
        pc = arg1;
        g_cwmars.runqtab[pid][cnt - 1] = pc;
    }

    return pc;
}

/* instruction handler for SPL */
static long
cwsplop(long pid, long pc)
{
    struct cwinstr     *op = &g_cwmars.optab[pc];
    long                cnt;
    long                cur;
    cwarg               arg1;
    cwarg               arg2;

    cwgetargs(op, pc, &arg1, &arg2);
    pc++;
    pc = cwwrapcore(pc);
    cnt = g_cwmars.proccnt[pid];
    cur = g_cwmars.curproc[pid];
    g_cwmars.runqtab[pid][cur] = pc;
    if (cnt < CW_PROCS) {
        g_cwmars.runqtab[pid][cnt] = arg2;
        cnt++;
        g_cwmars.proccnt[pid] = cnt;
    }

    return pc;
}

/* initialise instruction handling */
static void
cwinitop(void)
{
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
    struct cwinstr     *op;
    cwinstrfunc        *func;
    long                cur;
    long                cnt;
    long                pc;
    long                l;
#if defined(ZEUS)
    static long         ref = 0;
#endif

#if defined(ZEUS) && defined(ZEUSWINX11)
    while (XEventsQueued(g_cwmars.zeusx11.disp, QueuedAfterFlush)) {
        zeusprocev(&g_cwmars.zeusx11);
    }
#endif
    cur = g_cwmars.curproc[pid];
    pc = g_cwmars.runqtab[pid][cur];
    op = &g_cwmars.optab[pc];
#if defined(ZEUS) && 0
    fprintf(stderr, "%ld\t%ld\t", pid, pc);
    cwdisasm(op, stderr);
#endif
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
    func = g_cwmars.functab[op->op];
    pc = func(pid, pc);
    cnt = g_cwmars.proccnt[pid];
    if (pc == CW_NONE) {
        if (cnt > 1) {
            for (l = cur ; l < cnt - 1 ; l++) {
                g_cwmars.runqtab[pid][l] = g_cwmars.runqtab[pid][l + 1];
            }
        } else {
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
        }
        cnt--;
        g_cwmars.proccnt[pid] = cnt;
    } else if (op->op != CW_OP_SPL) {
        g_cwmars.runqtab[pid][cur] = pc;
        cur++;
    }
    cnt = g_cwmars.proccnt[pid];
    if (cur == cnt) {
        cur = 0;
    }
    g_cwmars.curproc[pid] = cur;
#if defined(ZEUS) && defined(ZEUSWINX11)
    ref++;
    if (!g_cwmars.running || ref == 32) {
        zeusdrawsim(&g_cwmars.zeusx11);
        ref = 0;
    }
#endif

    return;
}

/* virtual machine main loop */
C_NORETURN
void
cwloop(void)
{
    long pid = g_cwmars.curpid;

    while (g_cwmars.nturn[pid]--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    }
    fprintf(stderr, "TIE\n");
#if defined(ZEUS) && defined(ZEUSWINX11)
    sleep(5);
#endif

    exit(0);
}

/* initialise virtual machine */
void
cwinit(void)
{
    time_t seed32 = (((time(NULL) & 0xff) << 24)
                     | ((time(NULL) & 0xff) << 16)
                     | ((time(NULL) & 0xff) << 8)
                     | (time(NULL) & 0xff));

#if defined(CW_RANDMT32)
    srandmt32(seed32);
#else
    srand(time(&seed32));
#endif
    cwinitop();
    rcinitop();
#if defined(CWPIDMAP)
    g_cwmars.pidmap = calloc(CW_CORE_SIZE / CHAR_BIT, sizeof(char));
#endif
    g_cwmars.optab = calloc(CW_CORE_SIZE, sizeof(struct cwinstr));
    if (!g_cwmars.optab) {
        fprintf(stderr, "failed to allocate core\n");

        exit(1);
    }

    return;
}

int
main(int argc, char *argv[])
{
    FILE               *fp;
    long                base;
    long                lim;
    long                pc1;
    long                pc2;

#if defined(ZEUS) && defined(ZEUSSDL)
    zeusinitxsdl(&g_cwmars.zeussdl);
#endif
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.rc prog2.rc\n", argv[0]);

        exit(1);
    }
    cwinit();
#if defined(CW_RANDMT32)
    base = randmt32();
    base = cwwrapcore(base);
#else
    base = rand();
    base = cwwrapcore(base);
#endif
    fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[1]);

        exit(1);
    }
    pc1 = rcxlate(fp, 0, base, &base, &lim);
    if (pc1 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
#if defined(CW_RANDMT32)
    base = randmt32() % CW_CORE_SIZE;
#else
    base = rand() % CW_CORE_SIZE;
#endif
    fp = fopen(argv[2], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[2]);

        exit(1);
    }
    pc2 = rcxlate(fp, 1, base, &base, &lim);
    if (pc2 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
    g_cwmars.proccnt[0] = 1;
    g_cwmars.proccnt[1] = 1;
    g_cwmars.curproc[0] = 0;
    g_cwmars.curproc[1] = 0;
    g_cwmars.nturn[0] = CW_TURNS;
    g_cwmars.nturn[1] = CW_TURNS;
    g_cwmars.runqtab[0][0] = pc1;
    g_cwmars.runqtab[1][0] = pc2;
#if defined(CW_RANDMT32)
    g_cwmars.curpid = randmt32() & 0x01;
#else
    g_cwmars.curpid = rand() & 0x01;
#endif
    g_cwmars.prog1name = argv[1];
    g_cwmars.prog2name = argv[2];
    cwloop();

    /* NOTREACHED */
    exit(0);
}

