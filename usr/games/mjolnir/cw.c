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
#include <mach/param.h>
#include <zero/trix.h>
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
    long                    atype = op.atype;
    long                    btype = op.btype;
    long                    ndx = op.op;
    long                    ch;

    fprintf(fp, "%s\t", g_cwopnametab[ndx]);
    ch = 0;
    if (atype == CW_ARG_IMM) {
        ch = '#';
    } else if (atype == CW_ARG_INDIR) {
        ch = '@';
    } else if (atype == CW_ARG_PREDEC) {
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
    if (btype == CW_ARG_IMM) {
        ch = '#';
    } else if (btype == CW_ARG_INDIR) {
        ch = '@';
    } else if (btype == CW_ARG_PREDEC) {
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
    long                atype = 0;
    long                btype = 0;

    if (pc >= 0) {
        if (pid >= 0) {
            fprintf(stderr, "%ld\%ld\t%s", pid, pc, name);
        } else {
            fprintf(stderr, "%ld\t%s", pc, name);
        }
    } else {
        fprintf(stderr, "%s", name);
    }
    val = op.a;
    if (atype == CW_ARG_IMM) {
        fprintf(stderr, "\t#%ld", val);
    } else if (atype == CW_ARG_INDIR) {
        fprintf(stderr, "\t@%ld", val);
    } else if (atype == CW_ARG_PREDEC) {
        fprintf(stderr, "\t<%ld", val);
    } else {
        fprintf(stderr, "\t$%ld", val);
    }
    val = op.b;
    if (btype == CW_ARG_IMM) {
        fprintf(stderr, "\t#%ld", val);
    } else if (btype == CW_ARG_INDIR) {
        fprintf(stderr, "\t@%ld", val);
    } else if (btype == CW_ARG_PREDEC) {
        fprintf(stderr, "\t<%ld", val);
    } else {
        fprintf(stderr, "\t$%ld", val);
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
cwgetargs(long pc, long *argp1, long *argp2)
{
    struct cwinstr     *instr = &g_cwmars.core[pc];
    struct cwinstr      op = *instr;
    struct cwinstr     *src;
    long                atype = op.atype;
    long                btype = op.btype;
    long                arg1 = op.a;
    long                arg2 = op.b;

    if (atype) {
        if (atype == CW_ARG_INDIR || atype == CW_ARG_PREDEC) {
            if (atype == CW_ARG_PREDEC) {
                arg1--;
                arg1 = cwwrapval(arg1);
                instr->a = arg1;
            } else {
                arg1 += pc;
                arg1 = cwwrapval(arg1);
                src = &g_cwmars.core[arg1];
                arg1 = src->b;
            }
        }
    } else {
        arg1 += pc;
        arg1 = cwwrapval(arg1);
    }
    if (btype) {
        if (btype == CW_ARG_INDIR || btype == CW_ARG_PREDEC) {
            if (btype & CW_ARG_PREDEC) {
                arg2--;
                arg2 = cwwrapval(arg2);
                instr->b = arg2;
            } else {
                arg2 += pc;
                arg2 = cwwrapval(arg2);
                src = &g_cwmars.core[arg2];
                arg2 = src->b;
            }
        }
    } else {
        arg2 += pc;
        arg2 = cwwrapval(arg2);
    }
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(long pid, long pc)
{
    long                arg1;
    long                arg2;

#if defined(ZEUS) && defined(ZEUSSDL)
    zeusdrawsim(&g_cwmars.zeussdl);
#endif
    cwgetargs(pc, &arg1, &arg2);
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
    struct cwinstr  op = g_cwmars.core[pc];
    struct cwinstr  src;
    struct cwinstr *dest;
    long            atype = op.atype;
    long            arg1;
    long            arg2;

    cwgetargs(pc, &arg1, &arg2);
    pc++;
    dest = &g_cwmars.core[arg2];
    if (atype == CW_ARG_IMM) {
        dest->b = arg1;
    } else {
        src = g_cwmars.core[arg1];
        *dest = src;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for ADD */
static long
cwaddop(C_UNUSED long pid, long pc)
{
    struct cwinstr  op = g_cwmars.core[pc];
    struct cwinstr *dest;
    long            atype = op.atype;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    pc++;
    dest = &g_cwmars.core[arg2];
    if (atype == CW_ARG_IMM) {
        val = dest->b;
        val += arg1;
        val = cwwrapval(val);
        dest->b = val;
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
    struct cwinstr  op = g_cwmars.core[pc];
    struct cwinstr *dest;
    long            atype = op.atype;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    pc++;
    dest = &g_cwmars.core[arg2];
    if (atype == CW_ARG_IMM) {
        val = dest->b;
        val -= arg1;
        val = cwwrapval(val);
        dest->b = val;
    } else {
        val = dest->a;
        arg1 -= val;
        arg1 = cwwrapval(arg1);
        dest->a = arg1;
        val = dest->b;
        arg2 -= val;
        arg2 = cwwrapval(arg1);
        dest->b = arg2;
    }
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for JMP */
static long
cwjmpop(C_UNUSED long pid, long pc)
{
    long            arg1;
    long            arg2;

    cwgetargs(pc, &arg1, &arg2);
    pc = arg1;
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for JMZ */
static long
cwjmzop(C_UNUSED long pid, long pc)
{
    struct cwinstr *dest;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (!val) {
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
    struct cwinstr *dest;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (val) {
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
    struct cwinstr  op = g_cwmars.core[pc];
    struct cwinstr *dest;
    long            atype = op.atype;
    long            arg1;
    long            arg2;
    long            val1;
    long            val2;

    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val1 = dest->a;
    val2 = dest->b;
    if (atype == CW_ARG_IMM) {
        if (arg1 == val2) {
            pc++;
        }
    } else if (arg1 == val1 && arg2 == val2) {
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
    struct cwinstr  op = g_cwmars.core[pc];
    struct cwinstr *dest;
    long            atype = op.atype;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (atype == CW_ARG_IMM) {
        if (arg1 < val) {
            pc++;
        }
    } else if (arg2 < val) {
        pc++;
    }
    pc++;
    pc = cwwrapval(pc);

    return pc;
}

/* instruction handler for DJN */
static long
cwdjnop(C_UNUSED long pid, long pc)
{
    struct cwinstr *instr = &g_cwmars.core[pc];
    struct cwinstr  op = *instr;
    struct cwinstr *dest;
    long            btype = op.btype;
    long            arg1;
    long            arg2;
    long            val;

    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    if (btype == CW_ARG_IMM) {
        val = op.b;
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
    long           *runq = &g_cwmars.runqueue[pid][0];
    long            cnt;
    long            arg1;
    long            arg2;

    cwgetargs(pc, &arg1, &arg2);
    cnt = g_cwmars.proccnt[pid];
    pc++;
    pc = cwwrapval(pc);
    if (cnt < CW_MAX_PROCS) {
        runq[cnt - 1] = pc;
        arg1 = cwwrapval(arg1);
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
    long           *runq;
    long            cur;
    long            cnt;
    long            pc;
    long            ndx;
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
    pc = cwwrapval(pc);
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
    pc++;
    cnt = g_cwmars.proccnt[pid];
    pc = cwwrapval(pc);
    if (op.op == CW_OP_DAT) {
        if (cnt > 1) {
            runq = &g_cwmars.runqueue[pid][0];
            if (cur == cnt - 1) {
                runq[cur] = 0;
                cur = 0;
            } else {
                for (ndx = cur ; ndx < cnt - 1 ; ndx++) {
                    runq[ndx] = runq[ndx + 1];
                }
                runq[ndx] = 0;
                cur++;
                if (cur == cnt) {
                    cur = 0;
                }
            }
            cnt--;
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
    } else if (op.op == CW_OP_SPL) {
        cnt = g_cwmars.proccnt[pid];
        g_cwmars.runqueue[pid][cnt - 1] = pc;
        cur++;
    } else {
        g_cwmars.runqueue[pid][cur] = pc;
        cur++;
        cur %= CW_MAX_PROCS;
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
cwrun(long nturn)
{
    long                pid = g_cwmars.curpid;

    while (nturn--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    }
    fprintf(stderr, "DRAW\n");
#if defined(ZEUS) && defined(ZEUSSDL)
    sleep(5);
#endif

    exit(0);
}

/* initialise virtual machine */
void
cwinit(void)
{
    void               *ptr;

    time_t seed32 = (((time(NULL) & 0xff) << 24)
                     | ((time(NULL) & 0xff) << 16)
                     | ((time(NULL) & 0xff) << 8)
                     | (time(NULL) & 0xff));

    srandmt32(seed32);
    cwinitop();
    rcinitop();
    //    ptr = malloc(csize);
    ptr = calloc(CW_CORE_SIZE, sizeof(struct cwinstr));
    if (!ptr) {
        fprintf(stderr, "failed to allocate core\n");

        exit(1);
    }
    //    memset(ptr, -1, csize);
    g_cwmars.core = ptr;

    return;
}

void
cwinitmars(struct cwmars *mars, long pc1, long pc2, long nturn)
{
    size_t              csize;

    mars->runqueue[0][0] = pc1;
    mars->runqueue[1][0] = pc2;
    mars->proccnt[0] = 1;
    mars->proccnt[1] = 1;
    mars->curproc[0] = 0;
    mars->curproc[1] = 0;
    mars->nturn[0] = nturn;
    mars->nturn[1] = nturn;
    mars->curpid = randmt32() & 0x01L;
    csize = CW_CORE_SIZE;
    if (csize & (CHAR_BIT - 1)) {
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

    return;
}

int
main(int argc, char *argv[])
{
    long                place = 0;
    long                base;
    long                size;
    long                adr;
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
    pc1 = rcloadfile(argv[1], base, &size);
    lim = base + size;
    g_cwmars.progsz[0] = size;
    adr = base;
    while (adr < lim) {
        setbit(&g_cwmars.memmap, adr);
        adr++;
    }
    do {
        adr = randmt32();
        adr = cwwrapval(adr);
        lim = adr + CW_MIN_DIST;
        while (adr < lim) {
            if (bitset(&g_cwmars.memmap, adr)) {

                break;
            }
            adr++;
        }
        place = 0;
    } while (place);
    free(g_cwmars.memmap);
    g_cwmars.memmap = NULL;
    pc2 = rcloadfile(argv[2], adr, &size);
    g_cwmars.progsz[1] = size;
    cwinitmars(&g_cwmars, pc1, pc2, CW_MAX_TURNS);
    g_cwmars.progpaths[0] = argv[1];
    g_cwmars.progpaths[1] = argv[2];
#if defined(ZEUS) && defined(ZEUSSDL)
    while (1) {
        zeusdrawsim(&g_cwmars.zeussdl);
        zeusprocev(&g_cwmars.zeussdl);
    }
#else
    cwrun(CW_MAX_TURNS);
#endif

    /* NOTREACHED */
    exit(0);
}

