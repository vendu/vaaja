#include <stdio.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>
#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>
#include <mjolnir/scr.h>

extern long mjolhit(struct mjolchr *src, struct mjolchr *dest);
extern long mjoltrap(struct mjolobj *trap, struct mjolchr *dest);

extern mjolcmdfunc     *mjolcmdfunctab[1024][1024];
extern mjolcmdmovefunc *mjolcmdmovefunctab[1024];

struct mjolchr *mjolplayer;
struct mjolchr *mjolchaseq;

void
mjolinitchr(struct mjolchr *chr, long lvl)
{
    chr->hp = 16 + ((lvl > 0)
                    ? (mjolrand() % lvl)
                    : 0);
    chr->maxhp = chr->hp;
    chr->gold = mjolrand() & 0xff;
    chr->str = 4 + ((lvl > 0)
                    ? (mjolrand() % lvl)
                    : 0);
    chr->maxstr = chr->str;
    chr->arm = 0;
    chr->exp = 0;
    chr->speed = 1;
    chr->turn = 0;
    chr->nturn = 0;
/*  dex, lock, intl, def */

    return;
}

struct mjolchr *
mjolmkchr(long type)
{
    struct mjolchr *chr = calloc(1, sizeof(struct mjolchr));

    if (!chr) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    chr->data.type = type;

    return chr;
}

struct mjolchr *
mjolmkplayer(void)
{
    struct mjolchr *player = mjolmkchr(MJOLNIR_CHAR_PLAYER);

    mjolinitchr(player, 1);
    mjolplayer = player;

    return player;
}

/* determine how many continuous turns a character has */
long
mjolhasnturn(struct mjolchr *chr)
{
    long          retval = 0;
    long          speed = chr->speed;
    unsigned long turn;

    if (speed < 0) {
        turn = chr->turn;
        /* only move every abs(speed) turns */
        if (chr->nturn == turn) {
            /* allow movement */
            retval = 1;
            turn -= speed;
            chr->turn = turn;
        }
    } else {
        /* return speed */
        retval = speed;
    }

    return retval;
}

long
mjoldoturn(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj  *dest = NULL;
    long             lvl;
    long             retval = 0;
    long             n = mjolhasnturn(chr);
    long             x;
    long             y;
    int            (*printmsg)(const char *, ...) = game->scr->printmsg;
    int            (*getkbd)(void) = game->scr->getch;
    mjolcmdfunc     *func;
    mjolcmdmovefunc *movefunc;
    int              cmd;
    int              dir;
    int              item = 0;

    if (!n) {
        printmsg("You cannot move\n");
    }
    while (n) {
        if (n > 1) {
            printmsg("You have %ld turns\n", n);
        }
        do {
            cmd = getkbd();
        } while (cmd > 0xff);
//            clrmsg();
        if (cmd == MJOLNIR_CMD_BOSS) {

            exit(0);
        } else if (mjolismove(cmd)) {
            movefunc = mjolcmdmovefunctab[cmd];
            movefunc(game, chr);
        } else {
            if (mjolhasdir(cmd)) {
                lvl = game->lvl;
                x = chr->data.x;
                y = chr->data.y;
                printmsg("Which direction?");
                do {
                    dir = getkbd();
                } while (dir > 0xff);
                if (
#if (MJOLNIR_CURSES)
                    dir == KEY_UP ||
#endif
                    dir == MJOLNIR_CMD_MOVE_UP) {
                    printmsg("UP");
                    if (y) {
                        y--;
                    }
                } else if (
#if (MJOLNIR_CURSES)
                    dir == KEY_DOWN ||
#endif
                    dir == MJOLNIR_CMD_MOVE_DOWN) {
                    printmsg("DOWN");
                    if (y < game->height - 1) {
                        y++;
                    }
                } else if (
#if (MJOLNIR_CURSES)
                    dir == KEY_LEFT ||
#endif
                    dir == MJOLNIR_CMD_MOVE_LEFT) {
                    printmsg("LEFT");
                    if (x) {
                        x--;
                    }
                } else if (
#if (MJOLNIR_CURSES)
                    dir == KEY_RIGHT ||
#endif
                    dir == MJOLNIR_CMD_MOVE_RIGHT) {
                    printmsg("RIGHT");
                    if (x < game->width - 1) {
                        x++;
                    }
                } else {
                    printmsg("%lx\n", dir);
                }
                dest = game->objtab[lvl][x][y];
                func = mjolcmdfunctab[cmd][dir];
            }
            if (mjolhasarg(cmd)) {
                printmsg("Which item?");
                do {
                    item = getkbd();
                } while (item > 0xff);
                printmsg("%c\n", item);
                dest = game->inventory[item];
            }
            if (dest) {
                func = mjolcmdfunctab[cmd][item];
                if (func) {
                    retval += func(chr, dest);
                    n--;
                }
            }
        }
    }

    return retval;
}

void
mjoladditem(struct mjolchr *dest, struct mjolobj *item)
{
    ;
}

void
mjoldie(struct mjolchr *dest)
{
    ;
}

typedef long hitfunc(struct mjolchr *, struct mjolchr *);
long
mjolfindmove(struct mjolchr *src, struct mjolchr *dest,
             hitfunc *func, long mindist)
{
    long              retval = 0;
    struct mjolchr ***chrtab = mjolgame->chrtab[mjolgame->lvl];
    struct mjolobj ***objtab = mjolgame->objtab[mjolgame->lvl];
    struct mjolchr  *chr;
    struct mjolobj   *obj;
    struct mjolobj   *item;
    long              destx = dest->data.x;
    long              desty = dest->data.y;
    long              srcx = src->data.x;
    long              srcy = src->data.y;
    long              dx = destx - srcx;
    long              dy = desty - srcy;
    long              type;
    long              val;

    if (srcx == destx && srcy == desty) {

        return retval;
    }
    if (((labs(dx) == 1 && labs(dy) <= 1)
         || (labs(dy) == 1 && labs(dx) <= 1))
        && (func)) {
        /* attack */
        retval = func(src, dest);
    } else {
        /* TODO: make collision-checking/path-finding actually work... =) */
        if (labs(dx) > mindist) {
            if  (dx < -mindist) {
                /* dest is to the left of src */
                dx = -1;
            } else {
                /* dest is to the right of src */
                dx = 1;
            }
            srcx += dx;
            if (dy < -mindist) {
                /* dest is above src */
                dy = -1;
            } else if (dy > mindist) {
                /* dest is below src */
                dy = 1;
            }
            chr = chrtab[srcy + dy][srcx];
            type = chr->data.type;
            if (mjolcanmoveto(type)) {
                /* src can moves horizontally and vertically */
                srcy += dy;
            } else {
                chr = chrtab[srcy][srcx];
                type = chr->data.type;
                if (mjolcanmoveto(type)) {
                    /* src moves horizontally but not vertically */
                    ;
                } else {
                    srcx -= dx;
                    chr = chrtab[srcy + dy][srcx];
                    type = chr->data.type;
                    if (mjolcanmoveto(type)) {
                        /* src moves vertically but not horizontally */
                        srcy += dy;
                    }
                }
            }
        } else if (labs(dy) > mindist) {
            /* vertical movement only */
            if (dy < -mindist) {
                dy = -1;
            } else {
                dy = 1;
            }
            srcy += dy;
            chr = chrtab[srcy][srcx];
            type = chr->data.type;
            if (mjolcanmoveto(type)) {
                /* src moves vertically */
            } else {
                srcy -= dy;
                val = mjolrand() & 0x01;
                if (val) {
                    /* try to move left */
                    dx = -1;
                } else {
                    /* try to move right */
                    dx = 1;
                }
                srcx += dx;
                chr = chrtab[srcy][srcx];
                type = chr->data.type;
                if (mjolcanmoveto(type)) {
                    /* move into chosen direction */
                    ;
                } else {
                    /* change horizontal direction */
                    dx = -dx;
                    srcx += dx << 1;
                    chr = chrtab[srcy][srcx];
                    type = chr->data.type;
                    if (mjolcanmoveto(type)) {
                        /* valid move */
                        ;
                    } else {
                        /* change vertical direction */
                        srcx -= dx;
                        dy = -dy;
                        srcy += dy << 1;
                        chr = chrtab[srcy][srcx];
                        type = chr->data.type;
                        if (mjolcanmoveto(type)) {
                            /* valid move */
                            ;
                        } else {
                            /* try vertical and horizontal */
                            srcx -= dx;
                            chr = chrtab[srcy][srcx];
                            type = chr->data.type;
                            if (mjolcanmoveto(type)) {
                                /* valid move */
                                ;
                            } else {
                                srcx += 2 * dx;
                                chr = chrtab[srcy][srcx];
                                type = chr->data.type;
                                if (mjolcanmoveto(type)) {
                                    /* valid move */
                                } else {
                                    /* no move found */

                                    return retval;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!mindist && srcx == destx && srcy == desty
            && !(src->data.flg & MJOLNIR_CHAR_NO_PICK)) {
            item = objtab[destx][desty];
            while (item) {
                type = item->data.type;
                if (mjolcanpickup(type)) {
                    obj = item->data.next;
                    if (obj) {
                        obj->data.prev = item->data.prev;
                    }
                    obj = item->data.prev;
                    if (obj) {
                        obj->data.next = item->data.next;
                    } else {
                        chrtab[destx][desty] = item->data.next;
                    }
                    mjoladditem(dest, item);
                }
                item = item->data.next;
            }
            item = objtab[destx][desty];
            while (item) {
                type = item->data.type;
                if (type == MJOLNIR_OBJ_TRAP) {
                    retval += mjoltrap(item, dest);
                }
                item = item->data.next;
            }
        }
        obj = src->data.next;
        if (obj) {
            obj->data.prev = src->data.prev;
        }
        obj = src->data.prev;
        if (obj) {
            obj->data.next = src->data.next;
        } else {
            objtab[src->data.y][src->data.x] = src->data.next;
        }
        src->data.prev = NULL;
        src->data.next = objtab[srcy][srcx];
        obj = src->data.next;
        if (obj) {
            obj->data.prev = src;
        }
        src->data.x = srcx;
        src->data.y = srcy;
        chrtab[srcy][srcx] = src;
    }

    return retval;
}

long
mjolchase(struct mjolchr *src, struct mjolchr *dest)
{
    long retval = mjolfindmove(src, dest, mjolhit, 1);

    return retval;
}

long
mjolchaseall(struct mjolgame *game)
{
    long            retval = 0;
    struct mjolchr *src = mjolchaseq;

    while (src) {
        retval += mjolchase(src, mjolplayer);
        src = src->data.next;
    }

    return retval;
}

