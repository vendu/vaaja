//#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <zero/trix.h>
#include <mjolnir/cw.h>

extern struct cwmars    g_cwmars;

static void            *g_rcparsetab[128];

/* register supported operation */
void
rcaddop(const char *str, long id)
{
    long                ch = *str;
    void               *ptr1;
    void               *ptr2;

    if (ch) {
        ch = *str;
        if (ch) {
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
            if (ch) {
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
                if (ch) {
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
    void               *ptr = NULL;
    void               *tab = NULL;
    long                len = 0;
    long                ch;

    ch = *cp;
    ch = toupper(ch);
    if (isalpha(ch)) {
        cp++;
        /* 1st level table */
        ptr = g_rcparsetab[ch];
        if (!ptr) {
            return -1;
        }
        ch = *cp;
        if (isalpha(ch)) {
            ch = toupper(ch);
            cp++;
            /* 2nd level table */
            tab = ((void **)ptr)[ch];
            ch = *cp;
            if (tab) {
                ch = toupper(ch);
                cp++;
                /* 3rd level table */
                ptr = ((void **)tab)[ch];
                if (ptr) {
                    op = ((long *)ptr)[ch];
                }
            }
        }
        len = cp - str;
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

    return;
}

/* read instruction from source string */
static struct cwinstr
rcgetinstr(char *str)
{
    char               *cp = str;
    long                op = CW_NO_OP;
    struct cwinstr      instr = { CW_NO_OP, 0, 0, 0, 0, 0, 0 };
    long                sign;
    long                val = 0;
    int                 ch;

    instr.op = CW_NO_OP;
    ch = *cp;
    if (ch) {
        while (isspace(ch)) {
            ch = *cp++;
        }
        if (isalpha(ch)) {
            op = rcfindop(cp, &val);
            cp += val;
        }
        if (op != CW_NO_OP) {
            instr.op = op;
        } else {
            fprintf(stderr, "invalid mnemonic: %s\n", cp);

            exit(1);
        }
        instr.aflg = 0;
        instr.bflg = 0;
        instr.a = 0;
        instr.b = 0;
        ch = *cp++;
        if (ch) {
            while (isspace(ch)) {
                ch = *cp++;
            }
            if (ch) {
                sign = 0;
                instr.aflg = 0;
                if (ch == '#') {
                    instr.aflg |= CW_ARG_IMM;
                    ch = *cp++;
                } else if (ch == '@') {
                    instr.aflg |= CW_ARG_INDIR;
                    ch = *cp++;
                } else if (ch == '<') {
                    instr.aflg |= CW_ARG_PREDEC;
                    ch = *cp++;
                } else if (ch == '$') {
                    ch = *cp++;
                } else if (ch == ';') {
                    fprintf(stderr, "missing A-field: %s (%c)\n", str, ch);

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
                    instr.a = val;
                } else {
                    fprintf(stderr, "missing A-field: %s (%ld)\n",
                            str, val);

                    exit(1);
                }
                if (!ch) {

                    return instr;
                }
                while (isspace(ch) && ch != '\n') {
                    ch = *cp++;
                }
                if (!ch || ch == ';') {

                    return instr;
                }
                if (ch == ',') {
                    ch = *cp++;
                    while (isspace(ch) && ch != '\n') {
                        ch = *cp++;
                    }
                    if (ch == '\n') {
                        fprintf(stderr, "invalid B-field: %s\n",
                                str);
                    }
                    sign = 0;
                    instr.bflg = 0;
                    if (ch == '#') {
                        instr.bflg |= CW_ARG_IMM;
                        ch = *cp++;
                    } else if (ch == '@') {
                        instr.bflg |= CW_ARG_INDIR;
                        ch = *cp++;
                    } else if (ch == '<') {
                        instr.bflg |= CW_ARG_PREDEC;
                        ch = *cp++;
                    } else if (ch == '$') {
                        ch = *cp++;
                        } else if (ch == ';') {
                        fprintf(stderr, "invalid B-field: %s\n",
                                str);
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
                                instr.arg2 = 1;
                                instr.b = val;
                            } else {
                                fprintf(stderr, "invalid B-field: %s\n",
                                        str);
                            }
                    } else {
                        fprintf(stderr, "junk at end of line: %s\n",
                                str);
                    }
                }
            } else {
                fprintf(stderr, "invalid A-field: %s\n",
                        str);
            }
        } else {
            fprintf(stderr, "invalid A-field: %s\n",
                    str);
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

    return buf;
}

/* translate source file to bytecode */
long
rcxlatef(FILE *fp, long pid, long base, long *baseret, long *limret)
{
    struct cwinstr  op;
    char           *linebuf = NULL;
    long            adr = base;
    long            wrap = 0;
    long            entry = 0;
    char           *cp = NULL;
    long            pc = CW_NO_OP;
    long            n = 0;

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
                free(linebuf);
                linebuf = NULL;

                continue;
            }
            if (isalpha(*cp)) {
                op = rcgetinstr(cp);
                if (op.op != CW_NO_OP) {
                    if (!entry && op.op != CW_OP_DAT) {
                        pc = adr;
                        entry = 1;
                    }
                    n++;
                    g_cwmars.core[adr] = op;
                    if (pid) {
                        setbit(g_cwmars.pidmap, adr);
                    }
                    adr++;
                    adr = cwwrapval(adr);
                } else {
                    fprintf(stderr, "invalid instruction: %s\n", linebuf);

                    exit(1);
                }
                cwprintinstr(op, -1, -1);
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
    *baseret = base;
    *limret = adr;

    return pc;
}

