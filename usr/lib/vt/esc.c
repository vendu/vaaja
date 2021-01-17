/*
 * REFERENCE: http://www.termsys.demon.co.uk/vtansi.htm
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <zero/cdefs.h>
#include <vt/vt.h>

#define VT_RESET_CMD        0
#define VT_BRIGHT_CMD       1
#define VT_DIM_CMD          2
#define VT_UNDERSCORE_CMD   4
#define VT_BLINK_CMD        5
#define VT_REVERSE_CMD      7
#define VT_HIDDEN_CMD       8

struct vtesctabs            vtesctabs C_ALIGNED(MACH_PAGE_SIZE);
m_word_t                    vtatrbittab[9] C_ALIGNED(MACH_CL_SIZE)
= {
    0,
    VT_BRIGHT,
    VT_DIM,
    0,
    VT_UNDERSCORE,
    VT_BLINK,
    0,
    VT_REVERSE,
    VT_HIDDEN
};

void
vtgoto(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    nrow = vt->state.nrow;
    m_word_t    ncol = vt->state.ncol;
    m_word_t    row = 0;
    m_word_t    col = 0;

    if (!narg) {
        vt->state.row = row;
        vt->state.col = col;
    } else if (narg == 2) {
        row = argtab[0];
        col = argtab[1];
        if (row >= 0 && row < nrow && col >= 0 && col < ncol) {
            vt->state.row = row;
            vt->state.col = col;
        }
    }

    return;
}

void
vtgoup(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    arg = 1;
    m_word_t    row = vt->state.row;

    if (narg == 1) {
        arg = argtab[0];
    }
    arg = min(arg, row);
    vt->state.row -= arg;

    return;
}

void
vtgodown(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    arg = 1;
    m_word_t    nrow = vt->state.nrow;
    m_word_t    row = vt->state.row;

    if (narg == 1) {
        arg = argtab[0];
    }
    arg = min(arg, nrow - row - 1);
    vt->state.row += arg;

    return;
}

void
vtgoforward(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    arg = 1;
    m_word_t    ncol = vt->state.ncol;
    m_word_t    col = vt->state.col;

    if (narg == 1) {
        arg = argtab[0];
    }
    arg = min(arg, ncol - col - 1);
    vt->state.col += arg;
    /* FIXME: line wrap? */

    return;
}

void
vtgobackward(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    arg = 1;
    m_word_t    col = vt->state.col;

    if (narg == 1) {
        arg = argtab[0];
    }
    arg = min(arg, col);
    vt->state.col -= arg;
    /* FIXME: line wrap? */

    return;
}

void
vtsavecurs(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    row = vt->state.row;
    m_word_t    col = vt->state.col;

    if (!narg) {
        vt->savecurs.row = row;
        vt->savecurs.col = col;
    }

    return;
}

void
vtunsavecurs(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    row = vt->savecurs.row;
    m_word_t    col = vt->savecurs.col;

    if (!narg) {
        vt->state.row = row;
        vt->state.col = col;
    }

    return;
}

void
vtsavecursatr(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    row = vt->state.row;
    m_word_t    col = vt->state.col;

    if (!narg) {
        vt->savecurs.row = row;
        vt->savecurs.col = col;
        /* TODO: attributes? */
    }

    return;
}

void
vtrestorcursatr(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t    row = vt->savecurs.row;
    m_word_t    col = vt->savecurs.col;

    if (!narg) {
        vt->state.row = row;
        vt->state.col = col;
        /* TODO: attributes */
    }

    return;
}

void
vtscroll(struct vt *vt, m_word_t nrow)
{
    /* TODO: scroll screen */
    ;
}

void
vtsetscroll(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    struct vtstate *state = &vt->state;

    if (!narg) {
        state->scrolltop = 0;
        state->scrollbottom = state->nrow - 1;
    } if (narg == 2) {
        state->scrolltop = argtab[0];
        state->scrollbottom = argtab[1];
    }

    return;
}

void
vtscrolldown(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    vtscroll(vt, -1);

    return;
}

void
vtscrollup(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    vtscroll(vt, 1);

    return;
}

void
vtsettab(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
//    m_word_t     row = vt->state.row;
    m_word_t        col = vt->state.col;
    uint32_t       *tabmap = vt->state.tabmap;

    setbit(tabmap, col);

    return;
}

void
vtclrtab(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    uint32_t       *tabmap = vt->state.tabmap;
    m_word_t        ncol;
    m_word_t        col;
    m_word_t        n;

    if (narg == 1 && argtab[0] == 3) {
        ncol = vt->state.ncol;
        n = roundup2(ncol, 32);
        n >>= 5;
        while (n--) {
            *tabmap++ = 0;
        }
    } else if (narg == 0) {
        col = vt->state.col;
        clrbit(tabmap, col);
    }

    return;
}

void
vtclralltabs(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    uint32_t       *tabmap = vt->state.tabmap;
    size_t          n = roundup2(vt->state.ncol, 32);

    n >>= 5;
    while (n--) {
        *tabmap++ = 0;
    }

    return;
}

void
vteraseline(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t        arg = 0;

    if (narg == 0) {
        /* erase to end of line */
    } else {
        arg = argtab[0];
        if (arg == 1) {
            /* erase to start of line */
        } else if (arg == 2) {
            /* erase entire line */
        }
    }

    return;
}

void
vterasedir(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t        arg;

    if (narg == 0) {
        /* erase down */
    } else {
        arg = argtab[0];
        if (arg == 1) {
            /* erase up */
        } else if (arg == 2) {
            /* erase screen and move cursor to home */
        }
    }

    return;
}

#if defined(VT_PRINT) && (VT_PRINT)
void
vtprint(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t        arg;

    if (!narg) {
        vtprintscr(vt);
    } else if (narg == 1) {
        arg = argtab[0];
        switch (arg) {
            case 1:
                vtprintln(vt);

                break;
            case 4:
                vtstoplog(vt);

                break;
            case 5:
                vtstartlog(vt);

                break;
            default:

                break;
        }
    }

    return;
}
#endif /* VT_PRINT */

void
vtresetatr(struct vt *vt)
{
    struct vtstate *state = &vt->state;

    state->fgcolor = VT_DEF_FG_COLOR;
    state->bgcolor = VT_DEF_BG_COLOR;
    state->textatr = VT_DEF_TEXT_ATR;

    return;
}

void
vtsetatr(struct vt *vt, m_word_t narg, m_word_t *argtab)
{
    m_word_t        n;
    m_word_t        cmd;

    for (n = 0 ; n < narg ; n++) {
        cmd = argtab[n];
        if (cmd >= 0 && cmd <= 8) {
            /* set attribute bits */
            if (!cmd) {
                vtresetatr(vt);
            } else {
                vt->state.textatr |= vtatrbittab[cmd];
            }
        } else if (cmd >= 30 && cmd <= 37) {
            /* set foreground color */
            vt->state.fgcolor = vtdefcolor(vtfgtodefcolor(cmd));
        } else if (cmd >= 40 && cmd <= 47) {
            /* set background color */
            vt->state.bgcolor = vtdefcolor(vtbgtodefcolor(cmd));
        }
    }

    return;
}

void
vtinitesc(struct vt *vt)
{
    vt->esctabs = &vtesctabs;
    vtsetcsifunc(vt, 'H', vtgoto);
    vtsetcsifunc(vt, 'A', vtgoup);
    vtsetcsifunc(vt, 'B', vtgodown);
    vtsetcsifunc(vt, 'C', vtgoforward);
    vtsetcsifunc(vt, 'D', vtgobackward);
    vtsetcsifunc(vt, 'f', vtgoto);
    vtsetcsifunc(vt, 's', vtsavecurs);
    vtsetcsifunc(vt, 'u', vtunsavecurs);
    vtsetescfunc(vt, '7', vtsavecursatr);
    vtsetescfunc(vt, '8', vtrestorcursatr);
    vtsetcsifunc(vt, 'r', vtsetscroll);
    vtsetescfunc(vt, 'D', vtscrolldown);
    vtsetcsifunc(vt, 'M', vtscrollup);
    vtsetescfunc(vt, 'H', vtsettab);
    vtsetcsifunc(vt, 'g', vtclrtab);
    vtsetcsifunc(vt, 'J', vterasedir);
    vtsetcsifunc(vt, 'K', vteraseline);
#if defined(VT_PRINT) && (VT_PRINT)
    vtsetcsifunc(vt, 'i'; vtprint);
#endif
    vtsetcsifunc(vt, 'm', vtsetatr);
}

m_word_t
vtgetescnum(char *str, char **retstr)
{
    m_word_t        num = 0;

    while (isdigit(*str)) {
        num *= 10;
        num += *str - '0';
        str++;
    }
    if (retstr) {
        *retstr = str;
    }

    return num;
}

char *
vtgetescstr(char *str, char **retstr)
{
    m_word_t        nb = 32;
    m_word_t        len = 0;
    char           *ptr = malloc(nb);
    char           *mptr;

    if (!ptr) {
        fprintf(stderr, "out of memory\n");

        exit(1);
    }
    while (*str != '"') {
        if (len == nb) {
            nb <<= 1;
            mptr = realloc(ptr, nb);
            if (!mptr) {
                fprintf(stderr, "out of memory\n");
                free(ptr);

                exit(1);
            }
            ptr = mptr;
        }
        *ptr++ = *str++;
        len++;
    }
    if (len == nb) {
        nb <<= 1;
        mptr = realloc(ptr, nb);
        if (!mptr) {
            free(ptr);
            fprintf(stderr, "out of memory\n");
            
            exit(1);
        }
        ptr = mptr;
    }
    ptr[len] = '\0';
    if ((len) && retstr) {
        *retstr = str;
    }

    return ptr;
}

#define VT_ESC_ARGS 32
void
vtparseesc(struct vt *vt, char *str, char **retstr)
{
    m_word_t        num = 0;
    m_word_t        cmd;
    vtescfunc_t    *func;
    m_word_t        narg = 1;
    m_word_t        argtab[VT_ESC_ARGS];
    char           *ptr;
    m_word_t        sign = 1;
#if (VT_DEBUG_ESC)
    m_word_t        ndx;
#endif

    while (*str == VT_ESC) {
        str++;
        if (*str == VT_CSI) {
            /* ESC[ command */
            str++;
            if (*str == '-') {
                sign = -1;
                str++;
            }
            while (isdigit(*str)) {
                /* read first numerical parameter */
                num = vtgetescnum(str, &str);
                if (num && narg < VT_ESC_ARGS) {
                    argtab[narg] = sign * num;
                    narg++;
                }
                if (*str == ';') {
                    str++;
                }
                if (*str == '"') {
                    /* associate string to key */
                    ptr = vtgetescstr(str, &str);
                    if (num < 0x80) {
                        vtkeystrtab[num] = ptr;
                    }
                }
                if (*str == '-') {
                    sign = -1;
                    str++;
                } else {
                    sign = 1;
                }
            }
            cmd = *str;
            if (vtiscsicmd(vt, cmd)) {
                str++;
                func = vt->esctabs->csifunctab[cmd];
                func(vt, narg, argtab);
#if (VT_DEBUG_ESC)
            } else {
                if (isprint(cmd)) {
                    fprintf(stderr, "ESC[: unknown sequence %c\n",
                            (char)cmd);
                } else {
                    fprintf(stderr, "ESC[: unknown sequence %lx\n", cmd);
                }
                fprintf(stderr, "ARGS:\t");
                for (ndx = 0 ; ndx < narg - 1 ; ndx++) {
                    fprintf(stderr, "%ld ", argtab[ndx]);
                }
                fprintf(stderr, "%ld\n", argtab[ndx]);
#endif
            }
        } else if (*str == VT_FONT_G0 || *str == VT_FONT_G1) {
            /* ESC( command */
            /* TODO: implement these? */
#if (VT_DEBUG_ESC)
                fprintf(stderr, "ESC%c: font commands not supported\n", *str);
#endif
        } else if (*str == VT_HASH) {
            /* ESC# command */
            if (str[1] == '8') {
                /* TODO: fill screen with E */
            } else {
                /* TODO: 3, 4, 5, 6 */
            }
        } else {
            cmd = *str;
            if (vtisesccmd(vt, cmd)) {
                func = vt->esctabs->escfunctab[cmd];
                func(vt, 0, NULL);
                str++;
#if (VT_DEBUG_ESC)
            } else {
                if (isprint(cmd)) {
                    fprintf(stderr, "ESC%c: unknown sequence\n",
                            (char)cmd);
                } else {
                    fprintf(stderr, "ESC: unknown sequence %lx\n", cmd);
                }
                fprintf(stderr, "ARGS:\t");
                for (ndx = 0 ; ndx < narg - 1 ; ndx++) {
                    fprintf(stderr, "%ld ", argtab[ndx]);
                }
                fprintf(stderr, "%ld\n", argtab[ndx]);
#endif
            }
        }
    }
    if (retstr) {
        *retstr = str;
    }

    return;
}

