/* REFERENCE:  http://corewar.co.uk/cwg.txt */
/* REFERENCE: http://seblog.cs.uni-kassel.de/fileadmin/se/courses/SE1/WS0708/redcode-icws-88-2.pdf */

#include <corewar/conf.h>
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
#include <corewar/cw.h>
#include <corewar/rc.h>
#if defined(ZEUS)
#include <unistd.h>
#endif

static long         cwdatop(long pid, long pc);
static long         cwmovop(long pid, long pc);
static long         cwaddop(long pid, long pc);
static long         cwsubop(long pid, long pc);
static long         cwcmpop(long pid, long pc);
static long         cwjmpop(long pid, long pc);
static long         cwjmzop(long pid, long pc);
static long         cwjmnop(long pid, long pc);
static long         cwsltop(long pid, long pc);
static long         cwdjnop(long pid, long pc);
static long         cwsplop(long pid, long pc);

/* instruction operand-type verification */
static uint8_t      g_cwatvec[CW_MAX_OP + 1][16];
/* virtual machine structure */
struct cwmars       g_cwmars C_ALIGNED(MACH_PAGE_SIZE);
/* instruction handlers */
static cwinstrfunc *g_cwfunctab[CW_MAX_OP + 1]
= {
    cwdatop,
    cwmovop,
    cwaddop,
    cwsubop,
    cwcmpop,
    cwjmpop,
    cwjmzop,
    cwjmnop,
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
    "CMP",
    "JMP",
    "JMZ",
    "JMN",
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
            fprintf(stderr, "%ld\t%ld\t%s", pid, pc, name);
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
cwprintmars(struct cwmars *mars, long pid, long pidbits, long pc)
{
    long                ndx;
    long                cnt;

    fprintf(stderr, "program #1 size: %ld\n", mars->progsz[0]);
    fprintf(stderr, "program #2 size: %ld\n", mars->progsz[1]);
    cnt = mars->proccnt[0];
    fprintf(stderr, "Q0 (%ld):", cnt);
#if 0
    for (ndx = 0 ; ndx < cnt ; ndx++) {
        fprintf(stderr, " %ld", mars->runqueue[0][ndx]);
    }
#endif
    fprintf(stderr, "\n");
    cnt = mars->proccnt[1];
    fprintf(stderr, "Q1 (%ld):", cnt);
#if 0
    for (ndx = 0 ; ndx < cnt ; ndx++) {
        fprintf(stderr, " %ld", mars->runqueue[1][ndx]);
    }
#endif
    fprintf(stderr, "\n");
    cnt = mars->nturn[0];
    fprintf(stderr, "P0: %ld turns left\n", cnt);
    cnt = mars->nturn[1];
    fprintf(stderr, "P1: %ld turns left\n", cnt);
    if (pidbits & 0x03 || (pidbits & (1U << pid))) {
        cwprintinstr(g_cwmars.core[pc], pid, pc);
    }

    return;
}

static void
cwsetatvec(long op, long at, long bt)
{
    long                val = 0;
    uint8_t            *vec = g_cwatvec[op];
    uint8_t             a8c = 0;
    uint8_t             b8c = 0;

    if (at == ' ' || at == '$') {
        at = ' ';
        vec[0] = vec[1] = vec[2] = vec[3] = at;
        if (at == '#') {
            vec[4] = vec[5] = vec[6] = vec[7] = at;
        } else if (at == '@') {
            vec[8] = vec[9] = vec[10] = vec[11] = at;
            a8c = 8;
        } else if (at == '<') {
            vec[12] = vec[13] = vec[14] = vec[15] = at;
            a8c = 12;
        }
    }
    b8c = a8c;
    if (bt == ' ' || bt == '$') {
        bt = ' ';
    } if (bt == '#') {
        b8c++;
    } else if (bt == '@') {
        b8c += 2;
    } else if (bt == '<') {
        b8c += 3;
    }
    vec[b8c] = bt;

    return;
}

static long
cwchkatvec(long op, long at, long bt)
{
    long                val = 0;
    uint8_t            *vec = g_cwatvec[op];
    uint8_t             a8c = 0;
    uint8_t             b8c = 0;

    if (at == '#') {
        a8c = 4;
    } else if (at == '@') {
        a8c = 8;
    } else if (at == '<') {
        a8c = 12;
    }
    if (a8c) {
        b8c = a8c;
        a8c = vec[a8c];
        if (bt == '#') {
            b8c++;
        } else if (bt == '@') {
            b8c += 2;
        } else if (bt == '<') {
            b8c += 3;
        }
        val = b8c;
        val <<= 8;
        val |= a8c;

        return val;
    }

    return val;
}

static long
cwinitatvec(void)
{
    /* DAT */
    cwsetatvec(CW_OP_DAT, '#', '#');
    cwsetatvec(CW_OP_DAT, '#', '<');
    cwsetatvec(CW_OP_DAT, '<', '#');
    cwsetatvec(CW_OP_DAT, '<', '<');
    /* MOV */
    cwsetatvec(CW_OP_MOV, '#', ' ');
    cwsetatvec(CW_OP_MOV, '#', '@');
    cwsetatvec(CW_OP_MOV, '#', '<');
    cwsetatvec(CW_OP_MOV, ' ', ' ');
    cwsetatvec(CW_OP_MOV, ' ', '@');
    cwsetatvec(CW_OP_MOV, ' ', '<');
    cwsetatvec(CW_OP_MOV, '@', ' ');
    cwsetatvec(CW_OP_MOV, '@', '@');
    cwsetatvec(CW_OP_MOV, '@', '<');
    cwsetatvec(CW_OP_MOV, '<', ' ');
    cwsetatvec(CW_OP_MOV, '<', '@');
    cwsetatvec(CW_OP_MOV, '<', '<');
    /* ADD */
    cwsetatvec(CW_OP_ADD, '#', ' ');
    cwsetatvec(CW_OP_ADD, '#', '@');
    cwsetatvec(CW_OP_ADD, '#', '<');
    cwsetatvec(CW_OP_ADD, ' ', ' ');
    cwsetatvec(CW_OP_ADD, ' ', '@');
    cwsetatvec(CW_OP_ADD, ' ', '<');
    cwsetatvec(CW_OP_ADD, '@', ' ');
    cwsetatvec(CW_OP_ADD, '@', '@');
    cwsetatvec(CW_OP_ADD, '@', '<');
    cwsetatvec(CW_OP_ADD, '<', ' ');
    cwsetatvec(CW_OP_ADD, '<', '@');
    cwsetatvec(CW_OP_ADD, '<', '<');
    /* SUB */
    cwsetatvec(CW_OP_SUB, '#', ' ');
    cwsetatvec(CW_OP_SUB, '#', '@');
    cwsetatvec(CW_OP_SUB, '#', '<');
    cwsetatvec(CW_OP_SUB, ' ', ' ');
    cwsetatvec(CW_OP_SUB, ' ', '@');
    cwsetatvec(CW_OP_SUB, ' ', '<');
    cwsetatvec(CW_OP_SUB, '@', ' ');
    cwsetatvec(CW_OP_SUB, '@', '@');
    cwsetatvec(CW_OP_SUB, '@', '<');
    cwsetatvec(CW_OP_SUB, '<', ' ');
    cwsetatvec(CW_OP_SUB, '<', '@');
    cwsetatvec(CW_OP_SUB, '<', '<');
    /* CMP */
    cwsetatvec(CW_OP_CMP, '#', ' ');
    cwsetatvec(CW_OP_CMP, '#', '@');
    cwsetatvec(CW_OP_CMP, '#', '<');
    cwsetatvec(CW_OP_CMP, ' ', ' ');
    cwsetatvec(CW_OP_CMP, ' ', '@');
    cwsetatvec(CW_OP_CMP, ' ', '<');
    cwsetatvec(CW_OP_CMP, '@', ' ');
    cwsetatvec(CW_OP_CMP, '@', '@');
    cwsetatvec(CW_OP_CMP, '@', '<');
    cwsetatvec(CW_OP_CMP, '<', ' ');
    cwsetatvec(CW_OP_CMP, '<', '@');
    cwsetatvec(CW_OP_CMP, '<', '<');
    /* JMP */
    cwsetatvec(CW_OP_JMP, ' ', '#');
    cwsetatvec(CW_OP_JMP, ' ', ' ');
    cwsetatvec(CW_OP_JMP, ' ', '@');
    cwsetatvec(CW_OP_JMP, ' ', '<');
    cwsetatvec(CW_OP_JMP, '@', '#');
    cwsetatvec(CW_OP_JMP, '@', ' ');
    cwsetatvec(CW_OP_JMP, '@', '@');
    cwsetatvec(CW_OP_JMP, '@', '<');
    cwsetatvec(CW_OP_JMP, '<', '#');
    cwsetatvec(CW_OP_JMP, '<', ' ');
    cwsetatvec(CW_OP_JMP, '<', '@');
    cwsetatvec(CW_OP_JMP, '<', '<');
    /* JMZ */
    cwsetatvec(CW_OP_JMZ, ' ', '#');
    cwsetatvec(CW_OP_JMZ, ' ', ' ');
    cwsetatvec(CW_OP_JMZ, ' ', '@');
    cwsetatvec(CW_OP_JMZ, ' ', '<');
    cwsetatvec(CW_OP_JMZ, '@', '#');
    cwsetatvec(CW_OP_JMZ, '@', ' ');
    cwsetatvec(CW_OP_JMZ, '@', '@');
    cwsetatvec(CW_OP_JMZ, '@', '<');
    cwsetatvec(CW_OP_JMZ, '<', '#');
    cwsetatvec(CW_OP_JMZ, '<', ' ');
    cwsetatvec(CW_OP_JMZ, '<', '@');
    cwsetatvec(CW_OP_JMZ, '<', '<');
    /* JMN */
    cwsetatvec(CW_OP_JMN, ' ', '#');
    cwsetatvec(CW_OP_JMN, ' ', ' ');
    cwsetatvec(CW_OP_JMN, ' ', '@');
    cwsetatvec(CW_OP_JMN, ' ', '<');
    cwsetatvec(CW_OP_JMN, '@', '#');
    cwsetatvec(CW_OP_JMN, '@', ' ');
    cwsetatvec(CW_OP_JMN, '@', '@');
    cwsetatvec(CW_OP_JMN, '@', '<');
    cwsetatvec(CW_OP_JMN, '<', '#');
    cwsetatvec(CW_OP_JMN, '<', ' ');
    cwsetatvec(CW_OP_JMN, '<', '@');
    cwsetatvec(CW_OP_JMN, '<', '<');
    /* SLT */
    cwsetatvec(CW_OP_SLT, '#', ' ');
    cwsetatvec(CW_OP_SLT, '#', '@');
    cwsetatvec(CW_OP_SLT, '#', '<');
    cwsetatvec(CW_OP_SLT, ' ', ' ');
    cwsetatvec(CW_OP_SLT, ' ', '@');
    cwsetatvec(CW_OP_SLT, ' ', '<');
    cwsetatvec(CW_OP_SLT, '@', ' ');
    cwsetatvec(CW_OP_SLT, '@', '@');
    cwsetatvec(CW_OP_SLT, '@', '<');
    cwsetatvec(CW_OP_SLT, '<', ' ');
    cwsetatvec(CW_OP_SLT, '<', '@');
    cwsetatvec(CW_OP_SLT, '<', '<');
    /* DJN */
    cwsetatvec(CW_OP_DJN, ' ', '#');
    cwsetatvec(CW_OP_DJN, ' ', ' ');
    cwsetatvec(CW_OP_DJN, ' ', '@');
    cwsetatvec(CW_OP_DJN, ' ', '<');
    cwsetatvec(CW_OP_DJN, '@', '#');
    cwsetatvec(CW_OP_DJN, '@', ' ');
    cwsetatvec(CW_OP_DJN, '@', '@');
    cwsetatvec(CW_OP_DJN, '@', '<');
    cwsetatvec(CW_OP_DJN, '<', '#');
    cwsetatvec(CW_OP_DJN, '<', ' ');
    cwsetatvec(CW_OP_DJN, '<', '@');
    cwsetatvec(CW_OP_DJN, '<', '<');
    /* SPL */
    cwsetatvec(CW_OP_SPL, ' ', '#');
    cwsetatvec(CW_OP_SPL, ' ', ' ');
    cwsetatvec(CW_OP_SPL, ' ', '@');
    cwsetatvec(CW_OP_SPL, ' ', '<');
    cwsetatvec(CW_OP_SPL, '@', '#');
    cwsetatvec(CW_OP_SPL, '@', ' ');
    cwsetatvec(CW_OP_SPL, '@', '@');
    cwsetatvec(CW_OP_SPL, '@', '<');
    cwsetatvec(CW_OP_SPL, '<', '#');
    cwsetatvec(CW_OP_SPL, '<', ' ');
    cwsetatvec(CW_OP_SPL, '<', '@');
    cwsetatvec(CW_OP_SPL, '<', '<');
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
    uint8_t             at;
    uint8_t             bt;

    if (cwchkatvec(op.op, at, bt)) {
        fprintf(stderr, "invalid operands: ");
        cwdisasm(op, stderr);

        exit(1);
    }
    if (atype) {
        if (atype == CW_ARG_INDIR || atype == CW_ARG_PREDEC) {
            if (atype == CW_ARG_PREDEC) {
                arg1--;
                at = CW_PREDEC_ARG;
                arg1 = cwwrapval(arg1);
                instr->a = arg1;
                fprintf(stderr, "A=<%ld\t", arg1);
            } else {
                arg1 += pc;
                at = CW_INDIR_ARG;
                arg1 = cwwrapval(arg1);
                src = &g_cwmars.core[arg1];
                arg1 = src->b;
                fprintf(stderr, "A=@%ld\t", arg1);
            }
        } else {
            at = CW_IMM_ARG;
            fprintf(stderr, "A=#%ld\t", arg1);
        }
    } else {
        arg1 += pc;
        at = CW_REL_ARG;
        arg1 = cwwrapval(arg1);
        fprintf(stderr, "A=%ld\t", arg1);
    }
    if (btype) {
        if (btype == CW_ARG_INDIR || btype == CW_ARG_PREDEC) {
            if (btype == CW_ARG_PREDEC) {
                arg2--;
                bt = CW_PREDEC_ARG;
                arg2 = cwwrapval(arg2);
                instr->b = arg2;
                fprintf(stderr, "B=<%ld\n", arg1);
            } else {
                arg2 += pc;
                bt = CW_INDIR_ARG;
                arg2 = cwwrapval(arg2);
                src = &g_cwmars.core[arg2];
                arg2 = src->b;
                fprintf(stderr, "B=@%ld\n", arg1);
            }
        } else {
            bt = CW_IMM_ARG;
            fprintf(stderr, "B=#%ld\n", arg1);
        }
    } else {
        arg2 += pc;
        bt = CW_REL_ARG;
        arg2 = cwwrapval(arg2);
        fprintf(stderr, "B=%ld\n", arg2);
    }
    *argp1 = arg1;
    *argp2 = arg2;

    return;
}

/* instruction handler for DAT */
static long
cwdatop(C_UNUSED long pid, long pc)
{
    long                arg1;
    long                arg2;

#if defined(ZEUS) && defined(ZEUSSDL)
    zeusdrawsim(&g_cwmars.zeussdl);
#endif
    fprintf(stderr, "DAT\t");
    cwgetargs(pc, &arg1, &arg2);

    return -1;
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

    fprintf(stderr, "MOV\t");
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

    fprintf(stderr, "ADD\t");
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
        arg2 = cwwrapval(arg2);
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

    fprintf(stderr, "SUB\t");
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
        arg2 = cwwrapval(arg2);
        dest->b = arg2;
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

    fprintf(stderr, "CMP\t");
    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val1 = dest->a;
    val2 = dest->b;
    pc++;
    if (atype == CW_ARG_IMM) {
        if (arg1 == val2) {
            pc++;
        }
    } else if (arg1 == val1 && arg2 == val2) {
        pc++;
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

    fprintf(stderr, "JMP\t");
    cwgetargs(pc, &arg1, &arg2);
    pc = cwwrapval(arg1);

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

    fprintf(stderr, "JMZ\t");
    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (!val) {
        pc = arg1;
    } else {
        pc++;
        pc = cwwrapval(pc);
    }

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

    fprintf(stderr, "JMN\t");
    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    if (val) {
        pc = arg1;
    } else {
        pc++;
        pc = cwwrapval(pc);
    }

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

    fprintf(stderr, "SLT\t");
    cwgetargs(pc, &arg1, &arg2);
    dest = &g_cwmars.core[arg2];
    val = dest->b;
    pc++;
    if (atype == CW_ARG_IMM) {
        if (arg1 < val) {
            pc++;
        }
    } else if (arg2 < val) {
        pc++;
    }
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

    fprintf(stderr, "DJN\t");
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
    } else {
        pc++;
        pc = cwwrapval(pc);
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

    fprintf(stderr, "SPL\t");
    cwgetargs(pc, &arg1, &arg2);
    cnt = g_cwmars.proccnt[pid];
    pc++;
    if (cnt < CW_MAX_PROCS) {
        arg1 = cwwrapval(arg1);
        runq[cnt] = arg1;
        cnt++;
        g_cwmars.proccnt[pid] = cnt;
    }
    pc = cwwrapval(pc);

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

    return;
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
#if defined(ZEUS)
    static long     ref = 0;
#endif

#if defined(ZEUS) && defined(ZEUSX11)
    while (XPending(g_cwmars.zeusx11.disp)) {
        zeusprocev(&g_cwmars.zeusx11);
    }
#endif
    cur = g_cwmars.curproc[pid];
    pc = g_cwmars.runqueue[pid][cur];
    pc = cwwrapval(pc);
    op = g_cwmars.core[pc];
    func = g_cwmars.functab[op.op];
    cwprintmars(&g_cwmars, pid, 0x01, pc);
    pc = func(pid, pc);
    //    fprintf(stderr, "PC = %ld\n", pc);
    cnt = g_cwmars.proccnt[pid];
    if (pc < 0) {
        if (cnt > 1) {
            runq = &g_cwmars.runqueue[pid][0];
            for (ndx = cur ; ndx < cnt - 2 ; ndx++) {
                runq[ndx] = runq[ndx + 1];
            }
            runq[ndx] = 0;
            cnt--;
            g_cwmars.proccnt[pid] = cnt;
        } else {
#if defined(ZEUS) && defined(ZEUSX11)
            zeusdrawsim(&g_cwmars.zeusx11);
#endif
            fprintf(stderr, "program #%ld (%s) won (%ld)\n",
                    pid, g_cwmars.progpaths[pid], pc);
#if defined(ZEUS) && (defined(ZEUSX11) || defined(ZEUSSDL))
            sleep(5);
#endif

            exit(0);
        }
    } else {
        g_cwmars.runqueue[pid][cur] = pc;
        cur++;
    }
    cur %= cnt;
    g_cwmars.curproc[pid] = cur;
#if defined(ZEUS) && (defined(ZEUSX11) || defined(ZEUSSDL))
    ref++;
    if (!g_cwmars.running || ref == 32) {
        zeusdrawsim(&g_cwmars.zeusx11);
        ref = 0;
    }
#if defined(ZEUSX11)
    XSync(g_cwmars.zeusx11.disp, False);
#endif
#endif

    return;
}

/* virtual machine main loop */
C_NORETURN
void
cwrun(void)
{
    long                pid = g_cwmars.curpid;
    long                nturn = g_cwmars.nturn[pid];

    if (nturn--) {
        g_cwmars.nturn[pid] = nturn;
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
    cwinitatvec();
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
    mars->proccnt[0] = 1;
    mars->curproc[0] = 0;
    mars->nturn[0] = nturn;
    if (mars->nprog > 1) {
        mars->runqueue[1][0] = pc2;
        mars->proccnt[1] = 1;
        mars->curproc[1] = 0;
        mars->nturn[1] = nturn;
        mars->curpid = randmt32() & 0x01L;
    }
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

#if defined(ZEUS) && defined(ZEUSX11)
    zeusinitx11(&g_cwmars.zeusx11);
#endif
    if (argc < 2) {
        fprintf(stderr, "usage: %s prog1.red prog2.red\n", argv[0]);
        fprintf(stderr, "debug: %s prog1.red\n", argv[0]);

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
    g_cwmars.progpaths[0] = argv[1];
    if (argc > 2) {
#if 0
        free(g_cwmars.memmap);
        g_cwmars.memmap = NULL;
#endif
        pc2 = rcloadfile(argv[2], adr, &size);
        g_cwmars.progsz[1] = size;
        cwinitmars(&g_cwmars, pc1, pc2, CW_MAX_TURNS);
        g_cwmars.progpaths[1] = argv[2];
    } else {
        g_cwmars.nprog = 1;
        cwinitmars(&g_cwmars, pc1, 0, CW_MAX_TURNS);
    }
#if defined(ZEUS) && defined(ZEUSX11)
    while (1) {
        zeusdrawsim(&g_cwmars.zeusx11);
        zeusprocev(&g_cwmars.zeusx11);
    }
#else
    cwrun();
#endif

    /* NOTREACHED */
    exit(0);
}

