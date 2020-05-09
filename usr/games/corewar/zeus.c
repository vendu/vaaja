#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <corewar/cw.h>
#include <corewar/rc.h>
#include <corewar/zeus.h>
#if defined(ZEUSWINEFL)

#elif defined(ZEUSWINX11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#endif

extern struct cwmars    g_cwmars;
extern long             g_rcnargtab[CWNOP];

struct zeussel          g_zeussel;

#define ZEUSDEFLINE 256

char *
zeusdisasm(long pc, int *lenret)
{
    struct cwinstr *op = &g_cwmars.optab[pc];
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
        if (g_rcnargtab[op->op] == 2) {
            ch = '\0';
            if (op->aflg & CWIMMBIT) {
                ch = '#';
            } else if (op->aflg & CWINDIRBIT) {
                ch = '@';
            } else if (op->aflg & CWPREDECBIT) {
                ch = '<';
            }
            if (ch) {
                if (len > 0) {
                    *str++ = ch;
                    len--;
                }
            }
            if (op->aflg & CWSIGNBIT) {
                ret = snprintf(str, len, "%d,", op->a - CWCORESIZE);
                if (ret < 0) {
                    free(ptr);

                    return NULL;
                }
                len -= ret;
                str += ret;
            } else {
                ret = snprintf(str, len, "%d,", op->a);
                if (ret < 0) {
                    free(ptr);

                    return NULL;
                }
                len -= ret;
                str += ret;
            }
        }
        ch = '\0';
        if (op->bflg & CWIMMBIT) {
            ch = '#';
        } else if (op->bflg & CWINDIRBIT) {
            ch = '@';
        } else if (op->aflg & CWPREDECBIT) {
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
        if (op->bflg & CWSIGNBIT) {
            ret = snprintf(str, len, "%d", op->b - CWCORESIZE);
            if (ret < 0) {
                free(ptr);

                return NULL;
            }
            len -= ret;
            str += ret;
        } else {
            ret = snprintf(str, len, "%d", op->b);
            if (ret < 0) {
                free(ptr);

                return NULL;
            }
            len -= ret;
            str += ret;
        }
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

#if 0
void
zeusshowmem(void)
{
    char           *str;
    struct cwinstr *op;
    long            pc;
    int             dummy;

    for (pc = 0 ; pc < CWCORESIZE ; pc++) {
        op = &g_cwmars.optab[pc];
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
#endif /* 0 */

