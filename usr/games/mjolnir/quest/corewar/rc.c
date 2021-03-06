//#include <corewar/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <env/trix.h>
#include <corewar/cw.h>

#define cwset1field(id) (g_cw1fieldmap |= (1 << (id)))
#define cwis1field(id)  (g_cw1fieldmap & (1 << (id)))

static long             rcxlatef(FILE *fp,
                                 long pid,
                                 long base,
                                 long *sizeret);

extern struct cwmars    g_cwmars;

static void            *g_rcparsetab[128];

#if 0
char                    g_rcargtab[CW_MAX_OP + 1][32]
= {
    CW_OP_DAT_ATVEC,  // DAT
    CW_OP_MOV_ATVEC,  // MOV
    CW_OP_ADD_ATVEC,  // ADD
    CW_OP_SUB_ATVEC,  // SUB
    CW_OP_CMP_ATVEC,  // CMP
    CW_OP_JMP_ATVEC,  // JMP
    CW_OP_JMZ_ATVEC,  // JMZ
    CW_OP_JMN_ATVEC,  // JMN
    CW_OP_SLT_ATVEC,  // SLT
    CW_OP_DJN_ATVEC,  // DJN
    CW_OP_SPL_ATVEC   // SPL
};
#endif

static uint32_t         g_cw1fieldmap;

/* register supported operation */
void
rcaddop(const char *str, long id)
{
    long                ch = *str;
    void               *ptr1;
    void               *ptr2;

    if (ch) {
        ch = *str;
        if (isalpha(ch)) {
            str++;
            ch = toupper(ch);
            /* 1st level table */
            ptr1 = g_rcparsetab[ch];
            if (!ptr1) {
                ptr1 = calloc(128, sizeof(void *));
                if (!ptr1) {
                    fprintf(stderr, "failed to allocate 1st level parse table\n");

                    exit(1);
                }
                g_rcparsetab[ch] = ptr1;
            }
            ch = *str;
            if (isalpha(ch)) {
                str++;
                ch = toupper(ch);
                /* 2nd level table */
                ptr2 = ((void **)ptr1)[ch];
                if (!ptr2) {
                    ((void **)ptr1)[ch] = ptr2;
                    ptr2 = calloc(128, sizeof(void *));
                    if (!ptr2) {
                        fprintf(stderr, "failed to allocate 2nd level parse table\n");

                        exit(1);
                    }
                    ((void **)ptr1)[ch] = ptr2;
                }
                ch = *str++;
                if (isalpha(ch)) {
                    ch = toupper(ch);
                    ptr1 = ((void **)ptr2)[ch];
                    if (!ptr1) {
                        ptr1 = calloc(128, sizeof(long));
                        if (!ptr1) {
                            fprintf(stderr, "failed to allocate 3rd level parse table\n");

                            exit(1);
                        }
                        ((void **)ptr2)[ch] = ptr1;
                        ((long *)ptr1)[ch] = id;
                    }
                }
            }
        }
    }

    return;
}

/* look instruction up */
static long
rcfindop(char *str, long *lenret)
{
    long                op = CW_NO_OP;
    char               *cp = str;
    void               *ptr1 = NULL;
    void               *ptr2 = NULL;
    long                len = 0;
    long                ch;

    ch = *cp;
    ch = toupper(ch);
    if (isalpha(ch)) {
        cp++;
        /* 1st level table */
        ptr1 = g_rcparsetab[ch];
        if (!ptr1) {

            return -1;
        }
        ch = *cp;
        if (isalpha(ch)) {
            ch = toupper(ch);
            cp++;
            /* 2nd level table */
            ptr2 = ((void **)ptr1)[ch];
        }
        if (!ptr2) {

            return -1;
        }
        ch = *cp;
        if (isalpha(ch)) {
            ch = toupper(ch);
            cp++;
            /* 3rd level table */
            ptr1 = ((void **)ptr2)[ch];
            if (!ptr1) {

                return -1;
            }
            op = ((long *)ptr1)[ch];
            len = cp - str;
        } else {

            return -1;
        }
    }
    if (lenret) {
        *lenret = len;
    }

    return op;
}

/* register supported instructions */
void
rcinitop(void)
{
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
    cwset1field(CW_OP_JMP);
    cwset1field(CW_OP_SPL);

    return;
}

static char *
rcgetlabel(long pid, char *str, char **retstr)
{
    char               *lbl = calloc(16, sizeof(char));
    char               *cp = str;
    long                len = 0;
    long                entry;

    if (!lbl) {
        fprintf(stderr, "failed to allocate symbol name\n");

        exit(1);
    }
    while (isspace(*cp)) {
        cp++;
    }
    str = cp;
    fprintf(stderr, "GETLBL: %s\n", cp);
    if (toupper(cp[0]) == 'E'
        && toupper(cp[1]) == 'N'
        && toupper(cp[2]) == 'D'
        && isspace(cp[3])) {
        cp += 4;
        while (isspace(*cp)) {
            cp++;
        }
        while (isalnum(*cp) && len < 16) {
            lbl[len] = *cp++;
            len++;
        }
        if (len > 15) {
            lbl[15] = '\0';
            len = 15;
        }
        entry = cwfindsym(lbl);
        if (entry >= 0) {
            fprintf(stderr, "ENTRY: %s (%ld)\n", lbl, entry);
            g_cwmars.runqueue[pid][0] = entry;
        } else {
            fprintf(stderr, "ENTRY: %s NOT FOUND\n", lbl);

            exit(1);
        }
        while (isspace(*cp)) {
            cp++;
        }
        free(lbl);
        str = cp;
        lbl = NULL;
    } else if (isalnum(*cp)) {
        while (isalnum(*cp) && len < 16) {
            lbl[len] = *cp++;
            len++;
        }
        if (len >= 16) {
            lbl[15] = '\0';
            len = 15;
        }
        fprintf(stderr, "LABEL: %s\n", lbl);
        str = cp;
    }
    *retstr = str;

    return lbl;
}

/* read instruction from source string */
static struct cwinstr
rcgetinstr(long pid, long adr, char *str)
{
    char               *lbl;
    char               *cp = str;
    long                op = CW_NO_OP;
    struct cwinstr      instr = { CW_NO_OP, 0, 0, 0, 0, 0, 0, 0 };
    long                atype;
    long                a;
    long                sign;
    long                val = 0;
    long                ch;

    instr.op = CW_NO_OP;
    ch = *cp;
    if (ch) {
        while (isspace(ch)) {
            ch = *cp++;
        }
        if (isalpha(ch)) {
            op = rcfindop(cp, &val);
            if (op < 0) {
                lbl = rcgetlabel(pid, cp, &cp);
                if (lbl) {
                    cwaddsym(lbl, adr);
                } else {

                    return instr;
                }
                while (isspace(*cp)) {
                    cp++;
                }
                op = rcfindop(cp, &val);
            }
            if (op >= 0) {
                cp += val;
            }
        }
        while (isspace(*cp)) {
            cp++;
        }
        if (op != CW_NO_OP) {
            instr.op = op;
        } else {
            fprintf(stderr, "invalid mnemonic: %s\n", cp);

            exit(1);
        }
        atype = 0;
        a = 0;
        instr.btype = 0;
        instr.b = 0;
        ch = *cp++;
        if (ch) {
            sign = 0;
            atype = 0;
            if (ch == '#') {
                atype = CW_ARG_IMM;
                ch = *cp++;
            } else if (ch == '@') {
                atype = CW_ARG_INDIR;
                ch = *cp++;
            } else if (ch == '<') {
                atype = CW_ARG_PREDEC;
                ch = *cp++;
            } else if (ch == '$') {
                ch = *cp++;
            } else if (ch == ';') {
                fprintf(stderr, "missing A-field: %s (%c)\n", str, (int)ch);

                exit(1);
            }
            if (ch == '-') {
                sign = 1;
                ch = *cp++;
            }
            val = -1;
            if (isdigit(ch)) {
                val = 0;
                while (isdigit(ch)) {
                    val *= 10;
                    val += ch - '0';
                    ch = *cp++;
                }
            }
            if (val >= 0) {
                if (sign) {
                    val = -val;
                }
                //                val = cwwrapval(val);
                a = val;
            } else {
                fprintf(stderr, "missing A-field: %s (%ld)\n",
                        str, val);

                exit(1);
            }
            while (isspace(ch) && ch != '\n') {
                ch = *cp++;
            }
            if (!ch || ch == '\n' || ch == ';') {
                if (cwis1field(instr.op)) {
                    instr.atype = atype;
                    instr.a = a;
                    instr.btype = 0;
                    instr.b = 0;
                } else {
                    instr.btype = atype;
                    instr.b = a;
                    instr.atype = 0;
                    instr.a = 0;
                }
            } else if (ch == ',') {
                ch = *cp++;
                while (isspace(ch) && ch != '\n') {
                    ch = *cp++;
                }
                if (ch == '\n') {
                    fprintf(stderr, "invalid B-field: %s\n",
                            str);

                    exit(1);
                }
                sign = 0;
                instr.btype = 0;
                if (ch == '#') {
                    instr.btype = CW_ARG_IMM;
                    ch = *cp++;
                } else if (ch == '@') {
                    instr.btype = CW_ARG_INDIR;
                    ch = *cp++;
                } else if (ch == '<') {
                    instr.btype = CW_ARG_PREDEC;
                    ch = *cp++;
                } else if (ch == '$') {
                    ch = *cp++;
                } else if (ch == ';') {
                    fprintf(stderr, "invalid B-field: %s\n",
                            str);

                    exit(1);
                }
                if (ch) {
                    if (ch == '-') {
                        sign = 1;
                        ch = *cp++;
                    }
                    val = -1;
                    if (isdigit(ch)) {
                        val = 0;
                        while (isdigit(ch)) {
                            val *= 10;
                            val += ch - '0';
                            ch = *cp++;
                        }
                    }
                    if (val >= 0) {
                        if (sign) {
                            val = -val;
                        }
                        //                        val = cwwrapval(val);
                        instr.b = val;
                    } else {
                        fprintf(stderr, "invalid B-field: %s\n",
                                str);

                        exit(1);
                    }
                }
            }
        }
    }

    return instr;
}

/* read input line */
char *
rcgetline(FILE *fp)
{
    char *buf = NULL;
    char *ptr;
    long  n = 32;
    long  ndx = 0;
    int   ch;

    ch = fgetc(fp);
    if (ch != EOF) {
        buf = malloc(n * sizeof(char));
        if (buf) {
            while (ch != EOF) {
                if (ndx == n) {
                    n <<= 1;
                    ptr = realloc(buf, n * sizeof(char));
                    if (!ptr) {
                        free(buf);
                        fprintf(stderr, "failed to allocate line buffer\n");

                        exit(1);
                    }
                    buf = ptr;
                }
                if (ch == '\n') {

                    break;
                }
                buf[ndx] = ch;
                ndx++;
                ch = fgetc(fp);
            }
            buf[ndx] = '\0';
        }
    }
    fprintf(stderr, "BUF: %s\n", buf);

    return buf;
}

long
rcloadfile(const char *name, long base, long *sizeret)
{
    FILE               *fp;
    long                pc;
    long                lim;
    long                size;

    fp = fopen(name, "r");
    if (!fp) {
        fprintf(stderr, "failed to open file %s\n", name);

        exit(1);
    }
    pc = rcxlatef(fp, 0, base, &lim);
    size = lim - base;
    if (sizeret) {
        *sizeret = size;
    }
    fclose(fp);

    return pc;
}

/* translate source file to bytecode */
long
rcxlatef(FILE *fp, long pid, long base, long *limret)
{
    struct cwinstr  op;
    char           *linebuf = NULL;
    long            adr = base;
    long            wrap = 0;
    long            entry = 0;
    char           *cp = NULL;
    long            pc = base;

    while (1) {
        linebuf = rcgetline(fp);
        if (linebuf) {
            cp = linebuf;
            while (isspace(*cp)) {
                if (*cp == '\n') {
                    wrap = 1;
                }
                cp++;
            }
            if (*cp == ';') {
                /* TODO: add "; assert" and such functionality here */
                free(linebuf);
                linebuf = NULL;

                continue;
            }
            if (isalpha(*cp)) {
                op = rcgetinstr(pid, adr, cp);
                if (op.op != CW_NO_OP) {
                    if (!entry && op.op != CW_OP_DAT) {
                        pc = adr;
                        entry = 1;
                    }
                    op.red = 1;
                    op.pid = pid;
                    g_cwmars.core[adr] = op;
                    adr++;
                    adr = cwwrapval(adr);
                } else {
                    pc = adr;
                    *limret = adr;
#if 0
                    fprintf(stderr, "invalid instruction: %s\n", linebuf);

                    exit(1);
#endif
                }
            } else if (wrap) {
                free(linebuf);
                linebuf = NULL;

                continue;
            } else {

                break;
            }
        } else {

            break;
        }
    }
    if (linebuf) {
        free(linebuf);
    }
    *limret = adr;

    return pc;
}

