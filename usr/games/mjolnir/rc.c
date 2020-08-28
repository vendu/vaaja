//#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <zero/trix.h>
#include <mjolnir/cw.h>

extern struct cwmars    g_cwmars;

static void            *g_rcparsetab[128];  // instruction lookup table
long                    g_rcnargtab[CWNOP]  // per-instruction argument counts
= {
    1, /* DAT */
    2, /* MOV */
    2, /* ADD */
    2, /* SUB */
    1, /* JMP */
    2, /* JMZ */
    2, /* JMN */
    2, /* CMP */
    2, /* SLT */
    2, /* DJN */
    1  /* SPL */
};

/* register supported operation */
void
rcaddop(const char *name, long id)
{
    void               *ptr1;
    void               *ptr2;
    long                ndx = toupper(*name);

    name++;
    if (ndx) {
        ptr1 = g_rcparsetab[ndx];
        if (!ptr1) {
            ptr1 = calloc(128, sizeof(void *));
            g_rcparsetab[ndx] = ptr1;
        }
        if (!ptr1) {
            fprintf(stderr, "failed to allocate operation\n");

            exit(1);
        }
        ndx = toupper(*name);
        name++;
        if (ndx) {
            ptr2 = ((void **)ptr1)[ndx];
            if (!ptr2) {
                ptr2 = calloc(128, sizeof(void *));
                ((void **)ptr1)[ndx] = ptr2;
            }
            if (!ptr2) {
                fprintf(stderr, "failed to allocate operation\n");

                exit(1);
            }
            ndx = toupper(*name);
            name++;
            if (ndx) {
                ptr1 = ((long **)ptr2)[ndx];
                if (!ptr1) {
                    ptr1 = calloc(128, sizeof(long));
                    ((long **)ptr2)[ndx] = ptr1;
                }
                if (!ptr1) {
                    fprintf(stderr, "failed to allocate operation\n");

                    exit(1);
                }
                *((long *)ptr1) = id;
            }
        }
    }

    return;
}

/* look instruction up */
long
rcfindop(char *str, long *narg)
{
    long                op = CW_NONE;
    char               *cp = str;
    void               *ptr;

    ptr = g_rcparsetab[toupper(*cp)];
    cp++;
    if ((ptr) && isalpha(*cp)) {
        ptr = ((void **)ptr)[toupper(*cp)];
        cp++;
        if ((ptr) && isalpha(*cp)) {
            ptr = ((void **)ptr)[toupper(*cp)];
            cp++;
            if (isspace(*cp)) {
                op = *((long *)ptr);
            }
        }
    }
    if (op != CW_NONE) {
        *narg = g_rcnargtab[op];
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

/* read instruction from source file */
struct cwinstr *
rcgetop(char *str)
{
    char               *cp = str;
    struct cwinstr     *instr = NULL;
    long                op;
    long                sign;
    long                val;
    long                narg;

    if (cp) {
        instr = calloc(1, sizeof(struct cwinstr));
        if (!instr) {
            fprintf(stderr, "failed to allocate instruction\n");

            exit(1);
        }
        while (isspace(*cp)) {
            cp++;
        }
        op = CW_NONE;
        if (isalpha(*cp)) {
            op = rcfindop(cp, &narg);
        }
        if (op != CW_NONE) {
            instr->op = op;
            while (isalpha(*cp)) {
                cp++;
            }
        } else {
            fprintf(stderr, "invalid mnemonic: %s\n", str);

            exit(1);
        }
        instr->aflg = 0;
        instr->bflg = 0;
        instr->a = 0;
        instr->b = 0;
        if (*cp) {
            while (isspace(*cp)) {
                cp++;
            }
            if (*cp) {
                if (narg == 2) {
                    if (*cp == '#') {
                        instr->aflg |= CW_ADR_IMM;
                        cp++;
                    } else if (*cp == '@') {
                        instr->aflg |= CW_ADR_INDIR;
                        cp++;
                    } else if (*cp == '<') {
                        instr->aflg |= CW_ADR_PREDEC;
                        cp++;
                    } else if (*cp == '$') {
                        cp++;
                    }
                    val = CW_NONE;
                    sign = 0;
                    if (*cp == '-') {
                        sign = 1;
                        cp++;
                    }
                    if (isdigit(*cp)) {
                        val = 0;
                        while (isdigit(*cp)) {
                            val *= 10;
                            val += *cp - '0';
                            cp++;
                        }
                    } else {
                        fprintf(stderr, "invalid A-field: %s\n", str);

                        exit(1);
                    }
                    if (sign) {
                        instr->sign = 1;
                        val = CW_CORE_SIZE - val;
                    }
                    instr->a = val;
                }
                while (isspace(*cp)) {
                    cp++;
                }
                if (*cp == ',') {
                    cp++;
                    while (isspace(*cp)) {
                        cp++;
                    }
                }
                if (*cp == '#') {
                    instr->bflg |= CW_ADR_IMM;
                    cp++;
                } else if (*cp == '@') {
                    instr->bflg |= CW_ADR_INDIR;
                    cp++;
                } else if (*cp == '<') {
                    instr->bflg |= CW_ADR_PREDEC;
                    cp++;
                } else if (*cp == '$') {
                    cp++;
                }
                val = CW_NONE;
                sign = 0;
                if (*cp == '-') {
                    sign = 1;
                    cp++;
                }
                if (isdigit(*cp)) {
                    val = 0;
                    while (isdigit(*cp)) {
                        val *= 10;
                        val += *cp - '0';
                        cp++;
                    }
                } else {
                    fprintf(stderr, "invalid B-field: %s\n", str);

                    exit(1);
                }
                if (sign) {
                    instr->sign = 1;
                    val = CW_CORE_SIZE - val;
                }
                instr->b = val;
            }
        }
    }

    return instr;
}

/* read input line */
char *
rcgetline(FILE *fp)
{
    char               *buf = NULL;
    char               *ptr;
    long                n = 32;
    long                ndx = 0;
    int                 ch;

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
            buf[ndx] = '\n';
        }
    }

    return buf;
}

/* translate source file to bytecode */
long
rcxlate(FILE *fp, long pid, long base, long *baseret, long *limret)
{
    char               *linebuf = NULL;
    char               *cp;
    struct cwinstr     *op;
    struct cwinstr      instr;
    long                pc = base;
    long                ret = -1;
    long                n = 0;
    long                wrap = 0;

    *baseret = pc;
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

                continue;
            }
            if (isalpha(*cp)) {
                op = rcgetop(cp);
                if (op) {
                    op->pid = pid;
                    n++;
                    *((uint64_t *)&instr) = *((uint64_t *)&g_cwmars.optab[pc]);
                    if (*((uint64_t *)&instr)) {
                        fprintf(stderr, "programs overlap\n");

                        exit(1);
                    }
                    *((uint64_t *)(&g_cwmars.optab[pc])) = *((uint64_t *)op);
                    if (ret < 0 && op->op != CW_OP_DAT) {
                        /* execution starts at first non-DAT instruction */
                        ret = pc;
                    }
#if (CWPIDMAP)
                    if (pid) {
                        setbit(g_cwmars.pidmap, pc);
                    } else {
                        clrbit(g_cwmars.pidmap, pc);
                    }
#endif
                    pc++;
                    pc = cwwrapcore(pc);
                } else {
                    fprintf(stderr, "invalid instruction: %s\n", linebuf);

                    exit(1);
                }
            } else if (wrap) {
                free(linebuf);

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
    *limret = pc;

    return ret;
}

