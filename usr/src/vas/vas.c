/* valhalla assembler main file */

#include <vas/conf.h>

#ifndef VASDEBUG
#define VASDEBUG 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
//#include <zero/cdefs.h>
//#include <zero/param.h>
//#include <zero/trix.h>
#include <mach/param.h>
#include <vas/vas.h>
#include <vas/opt.h>
#include <vas/io.h>
#if defined(__v0__)
#include <v0/vm/vm.h>
//#include <v0/vm/types.h>
#endif

static char   * vasreadlabel(char *str, char **retptr);
static char   * vasreadsym(char *str, char **retptr);
static long     vasreadvalue(char *str, sysword *retval,
                            char **retptr);
static char   * vasreaddef(char *str, char **retptr);
static int      vasreadchar(char *str, char **retptr);
static void     vasreadtoken(char *str, char **retptr);

struct vas {
    struct {
        struct vastoken        *head;
        struct vastoken        *tail;
    } tokenq;
    char                       *linebuf;
    char                       *strbuf;
};

struct vas g_vas;

static char *
vasreadlabel(char *str, char **retptr)
{
    char *ptr = str;
    char *name = NULL;

#if (VASDEBUG)
    fprintf(stderr, "readlabel: %s\n", str);
#endif
    if ((*str) && (isalpha(*str) || *str == '_')) {
        str++;
        while ((*str) && (isalnum(*str) || *str == '_')) {
            str++;
        }
    }
    if (*str == ':') {
        *str++ = '\0';
        name = strdup(ptr);
        *retptr = str;
    }

    return name;
}

static char *
vasreaddef(char *str, char **retptr)
{
    char *ptr = str;
    char *name = NULL;

#if (VASDEBUG)
    fprintf(stderr, "readdef: %s\n", str);
#endif
    if (isalpha(*str) || *str == '_') {
        str++;
    }
    while (isalnum(*str) || *str == '_') {
        str++;
    }
    while ((*str) && (isspace(*str))) {
        str++;
    }
    if (*str == '=') {
        *str++ = '\0';
    }
    name = strdup(ptr);
    *retptr = str;

    return name;
}

static char *
vasreadsym(char *str, char **retptr)
{
    char *ptr = str;
    char *name = NULL;

#if (VASDEBUG)
    fprintf(stderr, "readsym: %s\n", str);
#endif
    if (isalpha(*str) || *str == '_') {
        str++;
        while (isalnum(*str) || *str == '_') {
            str++;
        }
        while ((*str) && (isspace(*str))) {
            str++;
        }
        if (*str == ',') {
            *str++ = '\0';
        }
        name = strdup(ptr);
    }
    *retptr = str;

    return name;
}

static long
vasreadvalue(char *str, sysword *valret, char **retstr)
{
    long                found = 0;
    uintptr_t          uval = 0;
    sysword           val = 0;
    long                neg = 0;

#if (VASDEBUG)
    fprintf(stderr, "readvalue: %s\n", str);
#endif
    if (*str == '-') {
        neg = 1;
        str++;
    }
    if (str[0] == '0' && tolower(str[1]) == 'x') {
        str += 2;
        while ((*str) && isxdigit(*str)) {
            uval <<= 4;
            uval += (isdigit(*str)
                     ? *str - '0'
                     : (islower(*str)
                        ? *str - 'a' + 10
                        : *str - 'A' + 10));
            str++;
        }
        found = 1;
    } else if (str[0] == '0' && tolower(str[1]) == 'b') {
        str += 2;
        while ((*str) && (*str == '0' || *str == '1')) {
            uval <<= 1;
            uval += *str - '0';
            str++;
        }
        found = 1;
    } else if (*str == '0') {
        str++;
        while ((*str) && isdigit(*str)) {
            if (*str > '7') {
                fprintf(stderr, "invalid number in octal constant: %s\n", str);

                exit(1);
            }
            uval <<= 3;
            uval += *str - '0';
            str++;
        }
        found = 1;
    } else if (isdigit(*str)) {
        while ((*str) && isdigit(*str)) {
            uval *= 10;
            uval += *str - '0';
            str++;
        }
        found = 1;
    }
    if (found) {
        *retstr = str;
        if (neg) {
            val = -((sysword)uval);
            *valret = val;
        } else {
            *valret = (sysword)uval;
        }
    }

    return found;
}

static char *
vasreadadr(char *str, char **retptr)
{
    char *ptr = str;
    char *name = NULL;

#if (VASDEBUG)
    fprintf(stderr, "readadr: %s\n", str);
#endif
    while (isalpha(*str) || *str == '_') {
        str++;
    }
    if (*str == ',') {
        *str++ = '\0';
    }
    name = strdup(ptr);
    *retptr = str;

    return name;
}

static uintptr_t
vasreadindex(char *str, sysword *retndx, char **retptr)
{
    sysword  reg = -1;
    sysword  val = 0;
    sysword  ndx = 0;
    sysword  sft = 0;
    long     neg = 0;

#if (VASDEBUG)
    fprintf(stderr, "readindex: %s\n", str);
#endif
    if (*str == '-') {
        neg = 1;
        str++;
    }
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str += 2;
        while ((*str) && isxdigit(*str)) {
            val <<= 4;
            val += (isdigit(*str)
                    ? *str - '0'
                    : tolower(*str) - 'a' + 10);
            str++;
        }
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        str += 2;
        while ((*str) && (*str == '0' || *str == '1')) {
            val <<= 1;
            val += *str - '0';
            str++;
        }
    } else if (*str == '0') {
        str++;
        while ((*str) && isdigit(*str)) {
            if (*str > '7') {
                fprintf(stderr, "invalid number in octal constant: %s\n", str);

                exit(1);
            }
            val <<= 3;
            val += *str - '0';
            str++;
        }
    } else if (isdigit(*str)) {
        while ((*str) && isdigit(*str)) {
            val *= 10;
            val += *str - '0';
            str++;
        }
    }
    if (*str == '(') {
        ndx = 1;
        str++;
    }
    if (ndx) {
        if (*str == '%' && str[1]) {
            str++;
        }
        reg = vasreadreg(str, &sft, &str);
#if 0
        if (reg >= VASNREG) {
            fprintf(stderr, "invalid register name %s\n", str);

            exit(1);
        }
#endif
        if (*str == ')') {
            *retptr = str;
        }
    }
    if (sft) {
        val &= (CHAR_BIT << sft) - 1;;
    }
    if (neg) {
        val = -val;
    }
    *retndx = val;

    return reg;
}

static int
vasreadchar(char *str, char **retptr)
{
    char      *name = str;
    sysword  val = 0;

#if (VASDEBUG)
    fprintf(stderr,"readchar: %s\n", str);
#endif
    str++;
    if (*str == '\\') {
        str++;
        if (isalpha(*str)) {
            switch (*str) {
                case 'n':
                    val = '\n';
                    str++;

                    break;
                case 't':
                    val = '\t';
                    str++;

                    break;
                case 'r':
                    val = '\r';
                    str++;

                    break;
                default:
                    fprintf(stderr, "invalid character literal: %s\n", name);
            }
        } else {
            if (*str == '0') {
                str++;
            }
            while ((*str) && isdigit(*str)) {
                if (*str > '7') {
                    fprintf(stderr, "invalid number in octal constant: %s\n",
                            name);

                    exit(1);
                }
                val <<= 3;
                val += *str - '0';
                str++;
            }
        }
    } else if (isalpha(*str) || isspace(*str)) {
        val = *str;
        str++;
    } else {
        while ((*str) && isdigit(*str)) {
            val *= 10;
            val += *str - '0';
            str++;
        }
    }
    if (val > 0xff) {
        fprintf(stderr, "oversize character literal %s (%lx)\n",
                name, (long)val);

        exit(1);
    }
    if (*str == '\'') {
        str++;
    }
    if (*str == ',') {
        str++;
    }
    *retptr = str;

    return (unsigned char)val;
}

static void
vasreadtoken(char *str, char **retptr)
{
    long             buflen = VAS_LINE_BUFSIZE;
    long             len;
    char            *buf = g_vas.strbuf;
    struct vastoken *token1 = malloc(sizeof(struct vastoken));
    struct vastoken *token2 = NULL;
    struct vasop    *op = NULL;
    char            *name = str;
    sysword          val = VASRESOLVE;
    sysword          ndx;
    sysword          sft = 0;
    int              ch;
    char            *ptr;

    while (*str && isspace(*str)) {
        str++;
    }
    if (*str == ',') {
        str++;
    }
    while (*str && isspace(*str)) {
        str++;
    }
#if (VASDEBUG)
    fprintf(stderr, "readtoken: %s\n", str);
#endif
    if ((*str) && (isdigit(*str) || *str == '-')) {
        val = vasreadindex(str, &ndx, &str);
        if (val >= 0) {
            token2 = malloc(sizeof(struct vastoken));
            token1->type = VASTOKENINDEX;
            token2->type = VASTOKENREG;
            token2->data.ndx.reg = val;
            token2->data.ndx.val = ndx;
        } else if (vasreadvalue(str, &val, &str)) {
            token1->type = VASTOKENVALUE;
            token1->data.value.val = val;
        } else {
            fprintf(stderr, "invalid token %s\n", g_vas.linebuf);

            exit(1);
        }
        vasqueuetoken(token1);
        if (token2) {
            vasqueuetoken(token2);
        }
    } else if ((*str) && *str == '"') {
        str++;
        len = 0;
        while (*str) {
            *buf++ = *str++;
            len++;
            if (len == buflen) {
                buflen <<= 1;
                ptr = g_vas.linebuf;
                g_vas.linebuf = realloc(ptr, buflen);
                if (!g_vas.linebuf) {
                    fprintf(stderr, "overlong line (%ld == %ld)\n",
                            len, buflen);

                    exit(1);
                }
                if (g_vas.linebuf != ptr) {
                    buf = &g_vas.linebuf[len];
                    str = buf;
                }
            }
        }
        while (*str != '"') {
            ch = *str++;
            if (ch == '\\') {
                switch (*str) {
                    case 'n':
                        *buf++ = (uint8_t)'\n';
                        str++;

                        break;
                    case 'r':
                        *buf++ = (uint8_t)'\r';
                        str++;

                        break;
                    case 't':
                        *buf++ = (uint8_t)'\t';
                        str++;

                        break;
                }
            } else {
                *buf++ = (uint8_t)ch;
            }
            len--;
        }
        if (*str == '"') {
            *buf++ = '\0';
            str++;
            token1->type = VASTOKENSTRING;
            token1->data.str = strdup(g_vas.strbuf);
        }
        vasqueuetoken(token1);
    } else if ((*str) && *str == '%') {
        str++;
        val = vasreadreg(str, &sft, &str);
        token1->type = VASTOKENREG;
        token1->sft = sft;
        token1->data.reg = val;
        vasqueuetoken(token1);
    } else if ((*str) && (isalpha(*str) || *str == '_')) {
#if (VASDB)
        ptr = str;
#endif
        name = vasreadlabel(str, &str);
        if (name) {
            token1->type = VASTOKENLABEL;
            token1->data.label.name = name;
            token1->data.label.adr = VASRESOLVE;
        } else {
            op = vasreadop(str, &sft, &str);
            if (op) {
                token1->type = VASTOKENINST;
                token1->data.inst.name = op->name;
                token1->data.inst.code = op->code;
                token1->data.inst.narg = op->narg;
                token1->data.inst.sft = sft;
#if (VASDB)
                token1->data.inst.data = strdup(ptr);
#endif
            } else {
                name = vasreadsym(str, &str);
                if (name) {
                    token1->type = VASTOKENSYM;
                    token1->data.sym.name = name;
                    token1->data.sym.adr = VASRESOLVE;
                }
                if (!name) {
                    name = vasreadadr(str, &str);
                    if (name) {
                        token1->type = VASTOKENSYM;
                        token1->data.sym.name = name;
                        token1->data.sym.adr = VASRESOLVE;
                    } else {
                        fprintf(stderr, "invalid token %s\n", g_vas.linebuf);

                        exit(1);
                    }
                }
            }
        }
        vasqueuetoken(token1);
    } else if ((*str) && *str == '$') {
        str++;
        if (isalpha(*str) || *str == '_') {
            if (vasfinddef(str, &val, &str)) {
                token1->type = VASTOKENDEF;
                token1->data.value.val = val;
            } else if ((*str) && (isalpha(*str)
                                  || *str == '_' || *str == '-')) {
                if (vasfindval(str, &val, &str)) {
                    token1->type = VASTOKENIMMED;
                    token1->data.value.val = val;
                } else if (vasreadvalue(str, &val, &str)) {
                    token1->type = VASTOKENIMMED;
                    token1->data.value.val = val;
                } else {
                    name = vasreadsym(str, &str);
                    if (name) {
                        token1->type = VASTOKENADR;
                        token1->data.adr.name = name;
                        token1->data.adr.val = VASRESOLVE;
                    } else {
                        fprintf(stderr, "invalid token %s\n", g_vas.linebuf);

                        exit(1);
                    }
                }
            }
            vasqueuetoken(token1);
        } else if ((*str) && isdigit(*str)) {
            if (vasreadvalue(str, &val, &str)) {
                token1->type = VASTOKENIMMED;
                token1->data.value.val = val;
                while (isspace(*str)) {
                    str++;
                }
                if (*str == ',') {
                    str++;
                }
            } else {
                fprintf(stderr, "invalid immediate %s\n", str);

                exit(1);
            }
            vasqueuetoken(token1);
        }
    } else if (*str == '\'') {
        val = vasreadchar(str, &str);
        token1->type = VASTOKENCHAR;
        token1->data.ch = val;
    } else if (*str == '.') {
        str++;
        sft = 0;
        if (!strncmp(str, "quad", 4)) {
            str += 4;
            token1->type = VASTOKENDATA;
            sft = token1->data.sft = 3;
        } else if (!strncmp(str, "long", 4)) {
            str += 4;
            token1->type = VASTOKENDATA;
            sft = token1->data.sft = 2;
        } else if (!strncmp(str, "byte", 4)) {
            str += 4;
            token1->type = VASTOKENDATA;
            sft = token1->data.sft = 0;
        } else if (!strncmp(str, "short", 5)) {
            str += 5;
            token1->type = VASTOKENDATA;
            sft = token1->data.sft = 1;
        } else if (!strncmp(str, "globl", 5)) {
            str += 5;
            token1->type = VASTOKENGLOBL;
        } else if (!strncmp(str, "space", 5)) {
            str += 5;
            token1->type = VASTOKENSPACE;
        } else if (!strncmp(str, "org", 3)) {
            str += 3;
            token1->type = VASTOKENORG;
        } else if (!strncmp(str, "align", 5)) {
            str += 5;
            token1->type = VASTOKENALIGN;
        } else if (!strncmp(str, "asciz", 5)) {
            str += 5;
            token1->type = VASTOKENASCIZ;
        }
        vasqueuetoken(token1);
    } else if (*str == '*') {
        str++;
        token1->type = VASTOKENINDIR;
        struct vastoken *token2 = malloc(sizeof(struct vastoken));
        if (*str == '%') {
            str++;
            val = vasreadreg(str, &sft, &str);
            token2->type = VASTOKENREG;
            token2->sft = sft;
            token2->data.reg = val;
        } else if ((*str) && (isalpha(*str) || *str == '_')) {
            name = vasreadsym(str, &str);
            if (name) {
                token1->type = VASTOKENSYM;
                token1->data.sym.name = name;
            } else {
                fprintf(stderr, "invalid token %s\n", g_vas.linebuf);

                exit(1);
            }
        } else {
            fprintf(stderr, "invalid token %s\n", g_vas.linebuf);

            exit(1);
        }
        vasqueuetoken(token1);
        vasqueuetoken(token2);
    } else if ((*str) && (isalpha(*str) || *str == '_')) {
        name = vasreaddef(str, &str);
        if (name) {
            while (isspace(*str)) {
                str++;
            }
            if (vasreadvalue(str, &val, &str)) {
                token1->type = VASTOKENDEF;
                token1->data.def.name = name;
                token1->data.def.val = val;
            }
            vasqueuetoken(token1);
        }
    } else {
        vasfreetoken(token1);
    }
    *retptr = str;

    return;
}

static struct vastoken *
vasprocvalue(struct vastoken *token, uintptr_t adr, void *retadr)
{
    uintptr_t        ret = adr + token->data.value.sft;
    char            *valptr = vasadrtoptr(adr);
    struct vastoken *retval;

    switch (token->data.value.sft) {
        case 1:
            *valptr = token->data.value.val;

            break;
        case 2:
            *((uint16_t *)valptr) = token->data.value.val;

            break;
        case 4:
            *((uint32_t *)valptr) = token->data.value.val;

            break;
        case 8:
            *((uint64_t *)valptr) = token->data.value.val;

            break;
    }
    *retadr = ret;
    retval = token->next;
    vasfreetoken(token);

    return retval;
}

static struct vastoken *
vasproclabel(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vassymrec *sym;
    struct vastoken  *retval;

    if (!_startset && !strncmp(token->data.label.name, "_start", 6)) {
#if 0
        if (adr & (sizeof(vasop_t) - 1)) {
            adr = roundup2(adr, sizeof(vasop_t));
        }
#endif
        _startadr = adr;
        _startset = 1;
    }
    sym = vasfindsym(token->data.label.name);
    if (sym) {
        sym->adr = adr;
    } else {
        sym = malloc(sizeof(struct vassymrec));
        sym->name = token->data.label.name;
        sym->adr = adr;
        vasaddsym(sym);
    }
    *retadr = adr;
    retval = token->next;
    vasfreetoken(token);

    return retval;
}

static struct vastoken *
vasprocchar(struct vastoken *token, uintptr_t adr, void *retadr)
{
    char            *valptr = vasadrtoptr(adr);
    struct vastoken *retval;

    *valptr = token->data.ch;
    adr++;
    *retadr = adr;
    retval = token->next;
    vasfreetoken(token);

    return retval;
}

static struct vastoken *
vasprocdata(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1 = token->next;
    uintptr_t        valadr = adr;

    while ((token1) && token1->type == VASTOKENVALUE) {
        token1->data.value.sft = token->data.sft;
        token1 = vasprocvalue(token1, valadr, &valadr);
    }
    *retadr = valadr;

    return token1;
}

static struct vastoken *
vasprocglobl(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1;
    struct vaslabel *label;

    token1 = token->next;
    while ((token1) && token1->type == VASTOKENSYM) {
        token1 = vasproclabel(token1, adr, &adr);
        label = malloc(sizeof(label));
        label->name = strdup(token1->data.label.name);
        label->adr = adr;
        vasaddglob(label);
    }
    *retadr = adr;

    return token1;
}

static struct vastoken *
vasprocspace(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1;
    struct vastoken *token2;
    uintptr_t      spcadr;
    char            *ptr;
    uint8_t          val;

    token1 = token->next;
    if ((token1) && token1->type == VASTOKENVALUE) {
        spcadr = token1->data.value.val;
        token2 = token1->next;
        if ((token2) && token2->type == VASTOKENVALUE) {
            ptr = vasadrtoptr(spcadr);
            val = token2->data.value.val;
            while (adr < spcadr) {
                ptr[0] = val;
                adr++;
                ptr++;
            }
            token1 = token2->next;
        } else {
            adr = spcadr;
        }
    } else {
        fprintf(stderr, "invalid .space attribute token type %lx\n",
                token1->type);

        exit(1);
    }
    *retadr = adr;

    return token1;
}

static struct vastoken *
vasprocorg(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1;
    uintptr_t      orgadr;
    char            *ptr;
    uint8_t          val;

    token1 = token->next;
    if ((token) && token->type == VASTOKENVALUE) {
        ptr = vasadrtoptr(adr);
        orgadr = token->data.value.val;
        val = token->data.value.val;
        while (adr < orgadr) {
            *ptr++ = val;
            adr++;
        }
        *retadr = adr;
    }

    return token1;
}

static struct vastoken *
vasprocalign(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1 = token->next;

    if ((token1) && token1->type == VASTOKENVALUE) {
        adr = roundup2(adr, token1->data.value.val);
    } else {
        fprintf(stderr, "invalid .align attribute token type %ld\n",
                token1->type);

        exit(1);
    }
    token1 = token1->next;
    *retadr = adr;

    return token1;
}

static struct vastoken *
vasprocasciz(struct vastoken *token, uintptr_t adr, void *retadr)
{
    struct vastoken *token1 = token->next;
    long             len = 0;
    char            *ptr;
    char            *str;

    while ((token1) && token1->type == VASTOKENSTRING) {
        ptr = vasadrtoptr(adr);
        str = token1->data.str;
        while ((*str) && *str != '\"') {
            if (*str == '\\') {
                str++;
                switch (*str) {
                    case 'n':
                        *ptr++ = '\n';
                        str++;

                        break;
                    case 't':
                        *ptr++ = '\t';
                        str++;

                        break;
                    case 'r':
                        *ptr++ = '\r';
                        str++;

                        break;
                    default:
                        fprintf(stderr, "invalid character literal: \'%s\n", str);

                        exit(1);
                }
            } else {
                *ptr++ = *str++;
            }
            len++;
        }
        if (*str == '\"') {
            *ptr = '\0';
            len++;
        }
        adr += len;
        token1 = token1->next;
    }
    vasfreetoken(token);
    *retadr = adr;

    return token1;
}

void
vasinit(void)
{
    vasinitbuf();
#if (VASALIGN)
    vasinitalign();
#endif
}

uintptr_t
vastranslate(uintptr_t base)
{
    uintptr_t         adr = base;
    struct vastoken *token = g_vas.tokenq.head;
    struct vastoken *token1 = NULL;
    vastokfunc_t    *func;

    while (token) {
        vasprinttoken(token);
        func = vastokfunctab[token->type];
        if (func) {
            //            fprintf(stderr, "HANDLING token of type %s (%ld)\n", vastoknametab[token->type], token->type);
#if (VASALIGN) && 0
            adr = vasaligntok(adr, token->type);
#endif
            token1 = func(token, adr, &adr);
            if (!token1) {

                break;
            }
        } else if (token) {
            fprintf(stderr, "stray token of type %s (%ld) found\n",
                    vastoknametab[token->type], token->type);
            // vasprinttoken(token);

            exit(1);
        }
        token = token1;
    }

    return base;
}

void
vasresolve(void)
{
    struct vassymrec *sym = symqueue;
    struct vassymrec *sym1;
    struct vassymrec *item;
    struct vaslabel  *label;

    while (sym) {
        if (sym->adr == VASRESOLVE) {
            fprintf(stderr, "unresolved symbol %s\n", sym->name);

            exit(1);
        }
        item = vasfindsym(sym->name);
        if (item) {
            if (item->adr == VASRESOLVE) {
                fprintf(stderr, "invalid symbol %s\n", item->name);

                exit(1);
            }
            *((uintptr_t *)sym->adr) = item->adr;
        } else {
            label = vasfindglob(sym->name);
            if (label) {
                *((uintptr_t *)sym->adr) = label->adr;
            } else {
                fprintf(stderr, "unresolved symbol %s\n", sym->name);

                exit(1);
            }
        }
        sym1 = sym;
        sym = sym->next;
        free(sym1);
    }
    symqueue = NULL;

    return;
}

long
vasreadline(FILE *fp)
{
    long             buflen = VAS_LINE_BUFSIZE;
    long             eof = 0;
    struct vasdef   *def;
    char            *fname;
    char            *ptr;
    char            *str = g_vas.linebuf;
    char            *lim = NULL;
    long             loop = 1;
    int              ch;
    long             comm = 0;
    long             empty = 1;
    long             len = 0;
#if (VASDB)
    unsigned long    line = 0;
#endif
    sysword          val = 0;

    if (empty) {
        if (eof) {
            loop = 0;
            //                empty = 0;

            break;
        }
        str = g_vas.linebuf;
        empty = 0;
        ch = fgetc(fp);
        if (ch == EOF) {
            loop = 0;

            break;
        } else {
            len = 0;
#if (VASDB)
            line++;
#endif
            while (ch != EOF && ch != '\n') {
                *str++ = ch;
                len++;
                if (len == buflen) {
                    fprintf(stderr, "overlong line (%ld == %ld): %s\n",
                            len, buflen, g_vas.linebuf);

                    exit(1);
                }
                ch = fgetc(fp);
            }
            eof = (ch == EOF);
            *str = '\0';
            str = g_vas.linebuf;
            lim = str + len;
            while ((*str) && isspace(*str)) {
                str++;
            }
            if (str > lim) {
                empty = 1;
            }
        }
    } else if (!strncmp((char *)str, ".define", 7)) {
        str += 7;
        while ((*str) && isspace(*str)) {
            str++;
        }
        if ((*str) && (isalpha(*str) || *str == '_')) {
            ptr = str;
            str++;
            while ((*str) && (isalnum(*str) || *str == '_')) {
                str++;
            }
            *str++ = '\0';
            while ((*str) && isspace(*str)) {
                str++;
            }
            if (vasreadvalue(str, &val, &str)) {
                def = malloc(sizeof(struct vasdef));
                def->name = strdup((char *)ptr);
                def->word = val;
                vasaddval(def);
            } else {
                fprintf(stderr, "invalid .define directive %s\n", ptr);

                exit(1);
            }
        }
    } else if (!strncmp((char *)str, ".include", 8)) {
        str += 8;
        while ((*str) && isspace(*str)) {
            str++;
        }
        if (*str == '<') {
            str++;
            fname = str;
            while ((*str) && *str != '>') {
                str++;
            }
            if (*str == '>') {
                *str = '\0';
                vasreadfile((char *)fname, adr);
                vasresolve();
                vasfreesyms();
            } else {
                fprintf(stderr, "invalid .include directive %s\n",
                        str);

                exit(1);
            }
        }
        //            done = 1;
    } else if (!strncmp((char *)str, ".import", 7)) {
        str += 7;
        while ((*str) && isspace(*str)) {
            str++;
        }
        if (*str == '<') {
            str++;
            fname = str;
            while ((*str) && *str != '>') {
                str++;
            }
            if (*str == '>') {
                *str = '\0';
                vasreadfile((char *)fname, adr);
                vasresolve();
                vasfreesyms();
            } else {
                fprintf(stderr, "invalid .import directive %s\n",
                        g_vas.strbuf);

                exit(1);
            }
        }
        //            empty = 1;
    } else if (str[0] == ';'
                   || (str[0] == '/' && str[1] == '/')) {
        /* the rest of the line is comment */
        empty = 1;
    } else if (str[0] == '/' && str[1] == '*') {
        /* comment */
        comm = 1;
        while (comm) {
            ch = fgetc(fp);
            if (ch == EOF) {
                loop = 0;

                break;
#if (VASDB)
            } else if (ch == '\n') {
                line++;
#endif
            } else if (ch == '*') {
                ch = fgetc(fp);
                if (ch == '/') {

                    comm = 0;
                } else if (ch == EOF) {
                    comm = 0;
                    loop = 0;
                    eof = 1;
                }
            }
        }
        empty = 1;
    } else if (*str) {
        vasreadtoken(str, &str);
#if (VASDB)
        if (token) {
            token->file = strdup((char *)name);
            token->line = line;
            vasqueuetoken(token);
        }
#endif
        while (isspace(*str)) {
            str++;
            }
        if (str >= lim) {
            empty = 1;
        }
    } else {
        empty = 1;
    }

    return;
}

void
vasreadfile(char *name, uintptr_t adr)
{
    FILE            *fp = fopen((char *)name, "r");

    while (loop) {
        vasreadline(fp);
    }
    fclose(fp);

    return;
}

