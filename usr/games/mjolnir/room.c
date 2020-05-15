#include <stdio.h>
#include <stdlib.h>
#include <mjolnir/conf.h>
#if (MJOLNIR_HASH)
#include <zero/hash.h>
#include <zero/trix.h>
#endif
#if (MJOLNIR_USE_ZERO_RANDMT32)
#include <zero/randmt32.h>
#endif
#include <zero/trix.h>
#include <mjolnir/mjol.h>

#define MJOLTEST                0
#define MJOLNIR_ROOM_MIN_WIDTH  8
#define MJOLNIR_ROOM_MIN_HEIGHT 6
#define MJOLNIR_ROOM_MAX_WIDTH  16
#define MJOLNIR_ROOM_MAX_HEIGHT 12
#define MJOLNIR_MIN_ROOMS       2
#define MJOLNIR_MAX_ROOMS       6

extern struct mjolobj * mjolmkcorridor(void);
extern struct mjolobj * mjolmkdoor(void);
extern struct mjolobj * mjolmkhorizwall(void);
extern struct mjolobj * mjolmkvertwall(void);

#if (MJOLNIR_ROOM_IDS)
const char roomidtab[16] =
{
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f'
};

struct mjolobj *
mjolmkid(struct mjolroom *room)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() & 0x0f;

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
#if 0
    obj->data.type = roomidtab[room->treelvl];
#endif
    obj->data.type = room->id;

    return obj;
}

#endif

void
mjolmkroom(struct mjolroom *room)
{
    struct mjolobj ***objtab = mjolgame->objtab[mjolgame->lvl];
    long              x;
    long              y;
    long              w;
    long              h;
    long              lim1;
    long              lim2;

    x = mjolrand() % max(room->width >> 2, 2);
    x = min(x, room->width - MJOLNIR_ROOM_MIN_WIDTH - 2);
    x = max(x, 1);
    y = mjolrand() % max(room->height >> 2, 2);
    y = min(y, room->height - MJOLNIR_ROOM_MIN_HEIGHT - 2);
    y = max(y, 1);
#if 0
    x = mjolrand() % max(room->width - MJOLNIR_ROOM_MIN_WIDTH - 1, 1);
    x = max(x, 1);
    y = mjolrand() % max(room->height - MJOLNIR_ROOM_MIN_HEIGHT - 1, 1);
    y = max(y, 1);
#endif
    w = MJOLNIR_ROOM_MIN_WIDTH + (mjolrand() % max(room->width - 2, 1));
    w = max(w, MJOLNIR_ROOM_MIN_WIDTH);
    w = min(w, room->width - x - 2);
    w = min(w, MJOLNIR_ROOM_MAX_WIDTH);
    h = MJOLNIR_ROOM_MIN_HEIGHT + (mjolrand() % max(room->height - 2, 1));
    h = max(h, MJOLNIR_ROOM_MIN_HEIGHT);
    h = min(h, room->height - y - 2);
    h = min(h, MJOLNIR_ROOM_MAX_HEIGHT);
    room->roomx += room->x + x;
    room->roomy += room->y + y;
    room->roomw = w;
    room->roomh = h;
    /* draw top wall */
    y = room->roomy;
    lim1 = room->roomx + w;
#if (MJOLNIR_ROOM_IDS)
    for (x = room->roomx ; x < lim1 ; x++) {
        objtab[y][x] = mjolmkid(room);
    }
    /* draw bottom wall */
    y = room->roomy + h - 1;
    lim1 = room->roomx + w;
    for (x = room->roomx ; x < lim1 ; x++) {
        objtab[y][x] = mjolmkid(room);
    }
    /* draw left wall */
    x = room->roomx;
    lim1 = room->roomy + h - 1;
    for (y = room->roomy + 1 ; y < lim1 ; y++) {
        objtab[y][x] = mjolmkid(room);
    }
    /* draw right wall */
    x = room->roomx + w - 1;
    lim1 = room->roomy + h - 1;
    for (y = room->roomy + 1 ; y < lim1 ; y++) {
        objtab[y][x] = mjolmkid(room);
    }
#else
    for (x = room->roomx ; x < lim1 ; x++) {
        objtab[y][x] = mjolmkhorizwall();
    }
    /* draw bottom wall */
    y = room->roomy + h - 1;
    lim1 = room->roomx + w;
    for (x = room->roomx ; x < lim1 ; x++) {
        objtab[y][x] = mjolmkhorizwall();
    }
    /* draw left wall */
    x = room->roomx;
    lim1 = room->roomy + h - 1;
    for (y = room->roomy + 1 ; y < lim1 ; y++) {
        objtab[y][x] = mjolmkvertwall();
    }
    /* draw right wall */
    x = room->roomx + w - 1;
    lim1 = room->roomy + h - 1;
    for (y = room->roomy + 1 ; y < lim1 ; y++) {
        objtab[y][x] = mjolmkvertwall();
    }
#endif
#if 0
    lim1 = room->roomx + w - 1;
    lim2 = room->roomy + h - 1;
    for (x = room->roomx + 1 ; x < lim1 ; lim1++) {
        for (y = room->roomy + 1 ; y < lim2 ; y++) {
            objtab[y][x] = mjolmkfloor();
        }
    }
#endif

    return;
}

void
mjolconnrooms(struct mjolgame *game,
              struct mjolroom *src, struct mjolroom *dest)
{
    struct mjolobj ***objtab = game->objtab[game->lvl];
    long              x;
    long              y;
    long              lim;
    long              delta;
    long              val;

    if (src->roomy <= dest->roomy) {
        /* top of src is above dest */
        if (src->roomx <= dest->roomx) {
            /* left of src is to the left of dest */
            if (dest->roomx <= src->roomx + src->roomw - 1) {
                /* src has horizontal items adjacent with dest */
                delta = src->roomx + src->roomw - dest->roomx - 1;
#if (MJOLTEST)
                delta = min(delta, dest->roomw - 1);
#endif
                lim = dest->roomy;
                val = mjolrand() % delta;
#if 0
                x = src->roomx + max(val, 1);
#endif
//                x = dest->roomx + max(val, 1);
//                x = src->roomx + src->width - max(val, 2);
//              x = max(x, dest->roomx + 1);
                y = src->roomy + src->roomh - 1;
                /* draw vertical line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++y < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
            } else {
                /* src is completely to the left of dest */
                if (dest->roomy <= src->roomy + src->roomh - 1) {
                    /* src has vertical items adjacent with dest */
                    delta = src->roomy + src->roomh - dest->roomy - 1;
#if (MJOLTEST)
                    delta = min(delta, dest->roomh - 1);
#endif
                    lim = dest->roomx;
                    val = mjolrand() % delta;
                    x = src->roomx + src->roomw - 1;
                    y = dest->roomy + max(val, 1);
//                    y = max(y, src->roomy + 1);
                    /* draw horizontal line */
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();
                    while (++x < lim) {
                        if (!objtab[y][x]) {
                            objtab[y][x] = mjolmkcorridor();
                        } else if (objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_VERTICAL_WALL
                                   || objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                            free(objtab[y][x]);
                            objtab[y][x] = mjolmkdoor();

                            return;
                        }
                    }
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();
                } else {
                    /* src is completely above and to the left of dest */
                    val = mjolrand() % (dest->roomh - 1);
                    lim = dest->roomy + max(val, 1);
//                    lim = dest->y + dest->height - 1;
                    val = mjolrand() % (src->roomw - 1);
                    x = src->roomx + max(val, 1);
                    y = src->roomy + src->roomh - 1;
                    /* draw vertical line */
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();
                    while (++y < lim) {
                        if (!objtab[y][x]) {
                            objtab[y][x] = mjolmkcorridor();
                        } else if (objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_VERTICAL_WALL
                                   || objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                            free(objtab[y][x]);
                            objtab[y][x] = mjolmkdoor();

                            return;
                        }
                    }
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    }
                    /* draw horizontal line */
                    lim = dest->roomx;
                    while (++x < lim) {
                        if (!objtab[y][x]) {
                            objtab[y][x] = mjolmkcorridor();
                        } else if (objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_VERTICAL_WALL
                                   || objtab[y][x]->data.type
                                   == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                            free(objtab[y][x]);
                            objtab[y][x] = mjolmkdoor();

                            return;
                        }
                    }
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();
                }
            }
        } else if (src->roomx <= dest->roomx + dest->roomw - 1) {
            /* src has horizontal items adjacent with dest */
            delta = dest->roomx + dest->roomw - src->roomx - 1;
#if (MJOLTEST)
            delta = min(delta, src->roomw - 1);
#endif
            lim = dest->roomy;
            val = mjolrand() % delta;
            x = src->roomx + max(val, 1);
//            x = max(x, dest->roomx + 1);
            y = src->roomy + src->roomh - 1;
            /* draw vertical line */
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
            while (++y < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
        } else {
            /* src is completely to the right of dest */
            if (dest->roomy <= src->roomy + src->roomh - 1) {
                /* src has vertical items adjacent with dest */
                delta = src->roomy + src->roomh - dest->roomy - 1;
#if (MJOLTEST)
                delta = min(delta, dest->roomh - 1);
#endif
                lim = src->roomx;
                val = mjolrand() % delta;
                x = dest->roomx + dest->roomw - 1;
                y = dest->roomy + max(val, 1);
//                y = max(y, src->roomy + 1);
                /* draw horizontal line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++x < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
            } else {
                /* src is completely above and to the right of dest */
                val = mjolrand() % (dest->roomh - 1);
                lim = dest->roomy + max(val, 1);
//                lim = dest->y + dest->height - 1;
                x = src->roomx + max(val, 1);
                y = src->roomy + src->roomh - 1;
                /* draw vertical line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++y < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                lim = x;
                x = dest->roomx + dest->roomw - 1;
                /* draw horizontal line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++x < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                free(objtab[y][x]);
                objtab[y][x] = mjolmkcorridor();
            }
        }
    } else if (src->roomy <= dest->roomy + dest->roomh - 1) {
        /* top of src is below top of dest */
        /* src has vertical items adjacent with dest */
        if (src->roomx <= dest->roomx) {
            /* src is to the left of dest */
            delta = min(dest->roomy + dest->roomh - src->roomy - 1, src->roomh - 1);
#if (MJOLTEST)
            delta = min(delta, src->roomh - 1);
#endif
            lim = dest->roomx;
            x = src->roomx + src->roomw - 1;
            val = mjolrand() % delta;
            y = src->roomy + max(val, 1);
//            y = max(y, dest->roomy + 1);
            /* draw horizontal line */
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
            while (++x < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
        } else {
            /* src is to the right of dest */
            delta = min(dest->roomy + dest->roomh - src->roomy, src->roomh - 1);
            lim = src->roomx;
            val = mjolrand() % delta;
            x = dest->roomx + dest->roomw - 1;
//            y = src->roomy + max(val, 1);
            y = src->roomy + max(val, 1);
            /* draw horizontal line */
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
            while (++x < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
        }
    } else {
        /* src is completely below dest */
        if (src->roomx <= dest->roomx) {
            /* left of src is to the left of dest */
            if (dest->roomx <= src->roomx + src->roomw - 1) {
                /* src has horizontal items adjacent with dest */
                delta = src->roomx + src->roomw - dest->roomx - 1;
#if (MJOLTEST)
                delta = min(delta, dest->roomw - 1);
#endif
                lim = dest->roomy + dest->roomh - 1;
                val = mjolrand() % delta;
//                lim = src->roomy + max(val, 1);
                val = mjolrand() % delta;
                x = dest->roomx + max(val, 1);
//                x = max(x, src->roomx + 1);
                y = dest->roomy + dest->roomh - 1;
                /* draw vertical line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++y < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
            } else {
                /* src is completely to the left of dest */
                delta = src->roomh - 1;
                val = mjolrand() % delta;
                lim = dest->roomx + max(mjolrand() % (dest->roomw - 1), 1);
//                lim = dest->x + dest->width - 1;
                x = src->roomx + src->roomw - 1;
                y = src->roomy + max(val, 1);
                /* draw horizontal line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++x < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                }
                lim = y;
                y = dest->roomy + dest->roomh - 1;
                /* draw vertical line */
                free(objtab[y][x]);
                objtab[y][x] = mjolmkdoor();
                while (++y < lim) {
                    if (!objtab[y][x]) {
                        objtab[y][x] = mjolmkcorridor();
                    } else if (objtab[y][x]->data.type
                               == MJOLNIR_OBJ_VERTICAL_WALL
                               || objtab[y][x]->data.type
                               == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                        free(objtab[y][x]);
                        objtab[y][x] = mjolmkdoor();

                        return;
                    }
                }
            }
        } else if (src->roomx <= dest->roomx + dest->roomw - 1) {
            /* src has horizontal items adjacent with dest */
            delta = dest->roomx + dest->roomw - src->roomx - 1;
#if (MJOLTEST)
            delta = min(delta, src->roomw - 1);
#endif
            lim = src->roomy;
            val = mjolrand() % delta;
            x = src->roomx + max(val, 1);
//            x = max(x, dest->roomx + 1);
            y = dest->roomy + dest->roomh - 1;
            /* draw vertical line */
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
            while (++y < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
        } else {
            /* src is completely to the right of dest */
            val = mjolrand() % (src->roomh - 1);
            lim = src->roomy + max(val, 1);
//            lim = src->y + src->width - 1;
            val = mjolrand() % (dest->roomw - 1);
            x = dest->roomx + max(val, 1);
            y = dest->roomy + dest->roomh - 1;
            /* draw vertical line */
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
            while (++y < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkcorridor();
            lim = src->roomx;
            /* draw horizontal line */
            while (++x < lim) {
                if (!objtab[y][x]) {
                    objtab[y][x] = mjolmkcorridor();
                } else if (objtab[y][x]->data.type
                           == MJOLNIR_OBJ_VERTICAL_WALL
                           || objtab[y][x]->data.type
                           == MJOLNIR_OBJ_HORIZONTAL_WALL) {
                    free(objtab[y][x]);
                    objtab[y][x] = mjolmkdoor();

                    return;
                }
            }
            free(objtab[y][x]);
            objtab[y][x] = mjolmkdoor();
        }
    }
    src->flg |= MJOLNIR_ROOM_CONNECTED;
    dest->flg |= MJOLNIR_ROOM_CONNECTED;

    return;
}

void
mjolinitroom(struct mjolgame *game, struct mjolroom *room)
{
    struct mjolobj ***objtab = game->objtab[game->lvl];
    long x;
    long y;
    long lim;

    /* top wall */
    y = room->roomy;
    x = room->roomx;
    lim = x + room->roomw;
    while (x < lim) {
        if (!objtab[y][x]) {
            objtab[y][x] = mjolmkhorizwall();
        }
        x++;
    }
    /* bottom wall */
    y = room->roomy + room->roomh;
    x = room->roomx;
    lim = x + room->roomw;
    while (x < lim) {
        if (!objtab[y][x]) {
            objtab[y][x] = mjolmkhorizwall();
        }
        x++;
    }
    /* left wall */
    y = room->roomy;
    x = room->roomx;
    lim = y + room->roomh;
    while (++y < lim) {
        if (!objtab[y][x]) {
            objtab[y][x] = mjolmkvertwall();
        }
    }
    /* right wall */
    y = room->roomy;
    x = room->roomx + room->roomw;
    lim = y + room->roomh;
    while (++y < lim) {
        if (!objtab[y][x]) {
            objtab[y][x] = mjolmkvertwall();
        }
    }

    return;
}

void
mjolsplitroom(struct mjolroom *room)
{
    long pos;

    if (!room) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    if (room->flg & MJOLNIR_DIR_VERTICAL) {
      //        pos = MJOLNIR_ROOM_MIN_WIDTH + (mjolrand() % max(room->width, MJOLNIR_ROOM_MIN_WIDTH));
      //        pos = min(pos, room->width >> 1);
#if 0
        pos = mjolrand() % room->width;
        pos = max(pos, room->width - MJOLNIR_ROOM_MIN_WIDTH - 2);
#endif
        pos = room->width >> 1;
//        pos = room->width >> 1;
//        pos = max(pos, MJOLNIR_ROOM_MIN_WIDTH);
        room->part1 = calloc(1, sizeof(struct mjolroom));
        room->part2 = calloc(1, sizeof(struct mjolroom));
        if (!room->part1 || !room->part2) {
            fprintf(stderr, "memory allocation failure\n");

            exit(1);
        }
        room->part1->x = room->x;
        room->part1->y = room->y;
        room->part1->width = pos;
        room->part1->height = room->height;
        room->part2->x = room->x + pos;
        room->part2->y = room->y;
        room->part2->width = room->width - pos;
        room->part2->height = room->height;
    } else {
#if 0
        pos = mjolrand() % room->height;
        pos = max(pos, room->height - MJOLNIR_ROOM_MIN_HEIGHT - 2);
#endif
        pos = room->height >> 1;
//        pos = max(pos, MJOLNIR_ROOM_MIN_HEIGHT);
        room->part1 = calloc(1, sizeof(struct mjolroom));
        room->part2 = calloc(1, sizeof(struct mjolroom));
        if (!room->part1 || !room->part2) {
            fprintf(stderr, "memory allocation failure\n");

            exit(1);
        }
        room->part1->x = room->x;
        room->part1->y = room->y;
        room->part1->width = room->width;
        room->part1->height = pos;
        room->part1->flg |= MJOLNIR_DIR_VERTICAL;
        room->part2->x = room->x;
        room->part2->y = room->y + pos;
        room->part2->width = room->width;
        room->part2->height = room->height - pos;
        room->part2->flg |= MJOLNIR_DIR_VERTICAL;
    }

    return;
}

struct mjolroom **
mjolinitrooms(struct mjolgame *game, long *nret)
{
//    struct mjolroom  *tree = calloc(1, sizeof(struct mjolroom));
//    struct mjolroom  *tab[MJOLNIR_MAX_ROOMS << 1];
    struct mjolroom  *room1 = calloc(1, sizeof(struct mjolroom));
    struct mjolroom  *room2;
    struct mjolroom  *room3;
//    long              nlvl = 1 + (mjolrand() % 0x02);
    long              nlvl = 3;
    struct mjolroom **roomstk = calloc(nlvl, sizeof(struct mjolroom *));
    struct mjolroom **ret = calloc(1 << nlvl, sizeof(struct mjolroom *));
    long              num;
//    long              lim = n + (n & 0x01);
    long              lim;
    long              ndx;
    long              cnt;
    long              lvl;
    long              val;
    struct mjolroom **tab = calloc(1 << nlvl, sizeof(struct mjolroom *));
    long              ndx1;
    long              ndx2;

    if (!tab || !room1) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    fprintf(stderr, "generating %ld rooms\n", (1 << nlvl));
    /* split the dungeon */
    room1->x = 0;
    room1->y = 0;
    room1->width = game->width;
    room1->height = game->height;
    room1->flg |= mjolrand() & 0x01;
#if (MJOLNIR_ROOM_IDS)
    room1->id = roomidtab[0];
#endif
    tab[0] = room1;
    ndx = 0;
    ndx1 = 1;
    ndx2 = 2;
    num = 0;
    lim = (1 << nlvl) - 1;
    while (num < lim) {
        fprintf(stderr, "%ld, %ld, %ld\n", ndx, ndx1, ndx2);
        room1 = tab[ndx];
        mjolsplitroom(room1);
        room2 = room1->part1;
        room3 = room1->part2;
        tab[ndx1] = room2;
        tab[ndx2] = room3;
#if (MJOLNIR_ROOM_IDS)
        room1->part1->id = roomidtab[ndx1];
        room1->part2->id = roomidtab[ndx2];
#endif
        num++;
        ndx++;
        ndx1 += 2;
        ndx2 += 2;
    }
#if 0
    ndx1 -= 2;
    ndx2 -= 2;
    if (!ndx) {
        lim = 1;
    } else {
        lim = ndx - 1;
    }
    num = 0;
    while (ndx1 >= lim) {
        room1 = tab[ndx2];
        room2 = tab[ndx1];
        ret[num] = room1;
        ret[num + 1] = room2;
        mjolmkroom(room1);
        mjolmkroom(room2);
        mjolconnrooms(game, room1, room2);
        ndx2 -= 2;
        ndx1 -= 2;
        num += 2;
    }
#endif

    if (!ndx) {
        lim = 1;
    } else {
        lim = ndx - 1;
    }
    ndx = ndx2 - 2;
    num = 0;
    room2 = NULL;
    while (ndx > lim) {
        room1 = tab[ndx];
        ret[num] = room1;
        mjolmkroom(room1);
#if 0
        if (room2) {
            mjolconnrooms(game, room1, room2);
        }
#endif
        ndx--;
        num++;
        room2 = room1;
    }

#if 0
    lim = ndx2 - 2;
    num = 0;
    room2 = NULL;
    while (ndx <= lim) {
        room1 = tab[ndx];
        ret[num] = room1;
        mjolmkroom(room1);
        if (room2) {
            mjolconnrooms(game, room1, room2);
        }
        ndx++;
        num++;
        room2 = room1;
    }
#endif

#if 0
    lim = ndx2 - 2;
    num = 0;
    room2 = NULL;
    while (ndx <= lim) {
        room1 = tab[ndx];
        ret[num] = room1;
        mjolmkroom(room1);
#if 0
        if (room2) {
            mjolconnrooms(game, room1, room2);
        }
#endif
        ndx++;
        num++;
        room2 = room1;
    }
#endif
    lim = num;
    num = 0;
    room2 = NULL;
    while (num < lim) {
        room1 = ret[num];
        if (room2) {
            fprintf(stderr, "CONNECTING %ld and %ld\n", num - 1, num);
            mjolconnrooms(game, room1, room2);
        }
        num++;
        room2 = room1;
    }

    *nret = (nlvl << 1);
    mjolprintlvl(game, game->lvl);

    return ret;
}

void
mjolgendng(struct mjolgame *game)
{
    long               nlvl = game->nlvl;
    long              *nroomtab = calloc(nlvl, sizeof(long));
    struct mjolroom ***lvltab = calloc(nlvl, sizeof(struct mjolroom ***));
    struct mjolroom  **roomtab;
    long               nroom;
    long               lvl;

    if (!nroomtab) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    mjolsrand(time(NULL));
//    mjolsrand(~0L);
    for (lvl = 0 ; lvl < nlvl ; lvl++) {
        game->lvl = lvl;
        roomtab = mjolinitrooms(game, &nroom);
        if (!roomtab) {
            fprintf(stderr, "memory allocation failure\n");

            exit(1);
        }
        nroomtab[lvl] = nroom;
        lvltab[lvl] = roomtab;
    }
    game->lvl = 0;
    game->nroomtab = nroomtab;
    game->lvltab = lvltab;

    return;
}

