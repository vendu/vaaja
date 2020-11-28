#include <corewar/conf.h>

#if defined(ZEUS)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <corewar/cw.h>
#include <corewar/rc.h>
#include <corewar/zeus.h>
#if defined(ZEUSX11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#endif

extern struct cwmars    g_cwmars;

struct zeussel          g_zeussel;

#define ZEUSDEFLINE 256

#if 0
void
zeusfence(void)
{
    ;
}
#endif

C_NORETURN
void
zeusexit(void)
{
    exit(0);
}

char *
zeusdisasm(long pc, int *lenret)
{
    struct cwinstr *op = &g_cwmars.core[pc];
    char           *ptr = malloc(ZEUSDEFLINE * sizeof(char));
    char           *str = ptr;
    int             len = ZEUSDEFLINE;
    int             ret;
    char            ch;

    if (ptr) {
        ret = snprintf(str, len, "%ld ", pc);
        if (ret < 0) {
            free(ptr);

            return NULL;
        }
        len -= ret;
        str += ret;
        if (pc < 10000) {
            ret = snprintf(str, len, "    ");
        } else if (pc < 100000) {
            ret = snprintf(str, len, "   ");
        } else if (pc < 1000000) {
            ret = snprintf(str, len, "  ");
        } else if (pc < 10000000) {
            ret = snprintf(str, len, " ");
        }
        if (ret < 0) {
            free(ptr);

            return NULL;
        }
        len -= ret;
        str += ret;
        ret = snprintf(str, len, "%s ", g_cwmars.opnames[op->op]);
        if (ret < 0) {
            free(ptr);

            return NULL;
        }
        len -= ret;
        str += ret;
        ch = '\0';
        if (op->atype == CW_ARG_IMM) {
            ch = '#';
        } else if (op->atype == CW_ARG_INDIR) {
            ch = '@';
        } else if (op->atype == CW_ARG_PREDEC) {
            ch = '<';
        }
        if (ch) {
            if (len > 0) {
                *str++ = ch;
                len--;
            }
        }
        ret = snprintf(str, len, "%d,", op->a);
        if (ret < 0) {
            free(ptr);

            return NULL;
        }
        len -= ret;
        str += ret;
        ch = '\0';
        if (op->btype == CW_ARG_IMM) {
            ch = '#';
        } else if (op->btype == CW_ARG_INDIR) {
            ch = '@';
        } else if (op->atype == CW_ARG_PREDEC) {
            ch = '<';
        }
        if (ch) {
            if (len > 0) {
                *str++ = ch;
                len--;
            }
        } else {
            ret = snprintf(str, len, " ");
            if (ret < 0) {
                free(ptr);

                return NULL;
            }
            len -= ret;
            str += ret;
        }
        ret = snprintf(str, len, "%d", op->b);
        if (ret < 0) {
            free(ptr);

            return NULL;
        }
        len -= ret;
        str += ret;
        if (len) {
            *str = '\0';
            *lenret = ZEUSDEFLINE - len;
        } else {
            fprintf(stderr, "debug line too long\n");

            return NULL;
        }
    }

    return ptr;
}

void
zeusshowmem(void)
{
    char           *str;
    struct cwinstr *op;
    long            pc;
    int             dummy;

    for (pc = 0 ; pc < CW_CORE_SIZE ; pc++) {
        op = &g_cwmars.core[pc];
        if (*(uint64_t *)op) {
            fprintf(stderr, "%ld\t", pc);
            str = zeusdisasm(pc, &dummy);
            if (!str) {
                fprintf(stderr, "failed to allocate memory\n");

                exit(1);
            }
            fprintf(stderr, "%s\n", str);
            free(str);
        }
    }
}

#endif /* defined(ZEUS) */

