#ifndef __MJOLNIR_MJOL_H__
#define __MJOLNIR_MJOL_H__

#if (TEST)
#include <stdio.h>
#endif
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <zero/trix.h>
#include <mjolnir/conf.h>
#if (MJOLNIR_USE_ZERO_RANDMT32)
#include <zero/randmt32.h>
#endif
#if (MJOLNIR_CURSES)
#include <curses.h>
#endif
#include <dungeon/dng.h>
#include <mjolnir/obj.h>
#include <mjolnir/chr.h>

extern struct mjolgame *mjolgame;
extern struct mjolchr  *chaseq;

#define mjolismove(ch)  bitset(mjolcmdismovemap, ch)
#define mjolsetmove(ch) setbit(mjolcmdismovemap, ch)
#define mjolhasdir(ch)  bitset(mjolcmdhasdirmap, ch)
#define mjolsetdir(ch)  setbit(mjolcmdhasdirmap, ch)
#define mjolhasarg(ch)  bitset(mjolcmdhasargmap, ch)
#define mjolsetarg(ch)  setbit(mjolcmdhasargmap, ch)
extern uint8_t          mjolcmdismovemap[256];
extern uint8_t          mjolcmdhasdirmap[256];
extern uint8_t          mjolcmdhasargmap[256];

#define mjolcanmoveto(type) bitset(mjolcanmovetomap, type)
#define mjolcanpickup(type) bitset(mjolcanpickupmap, type)
#define mjolcanwear(type)   bitset(mjolcanwearmap, type)
#define mjolcanwield(type)  bitset(mjolcanwieldmap, type)
extern uint8_t          mjolcanmovetomap[256 / CHAR_BIT];
extern uint8_t          mjolcanpickupmap[256 / CHAR_BIT];
extern uint8_t          mjolcanwearmap[256 / CHAR_BIT];
extern uint8_t          mjolcanwieldmap[256 / CHAR_BIT];

#define MJOLNIR_DEF_NICK   "johndoe"
#define MJOLNIR_LEN_NICK   16

#define MJOLNIR_DEF_NLVL   1
#define MJOLNIR_DEF_WIDTH  80
#define MJOLNIR_DEF_HEIGHT 24

/* commands */
#define ESC                       '\033'
#define CTRL(x)              l    ((x) & 0x80)
/* rogue commands */
#define MJOLNIR_CMD_BOSS             ESC
#define MJOLNIR_CMD_MENU             'm'
#define MJOLNIR_CMD_IDENTIFY         '/'
#define MJOLNIR_CMD_MOVE_LEFT        'h'
#define MJOLNIR_CMD_MOVE_DOWN        'j'
#define MJOLNIR_CMD_MOVE_UP          'k'
#define MJOLNIR_CMD_MOVE_RIGHT       'l'
#define MJOLNIR_CMD_MOVE_UPLEFT      'y'
#define MJOLNIR_CMD_MOVE_UPRIGHT     'u'
#define MJOLNIR_CMD_MOVE_DOWNLEFT    'b'
#define MJOLNIR_CMD_MOVE_DOWNRIGHT   'n'
#if (MJOLNIR_CURSES)
#define MJOLNIR_CMD_LEFT             (KEY_LEFT)
#define MJOLNIR_CMD_DOWN             (KEY_DOWN)
#define MJOLNIR_CMD_UP               (KEY_UP)
#define MJOLNIR_CMD_RIGHT            (KEY_RIGHT)
#endif
#define MJOLNIR_CMD_RUN_LEFT         'H'
#define MJOLNIR_CMD_RUN_DOWN         'J'
#define MJOLNIR_CMD_RUN_UP           'K'
#define MJOLNIR_CMD_RUN_RIGHT        'L'
#define MJOLNIR_CMD_RUN_UPLEFT       'Y'
#define MJOLNIR_CMD_RUN_UPRIGHT      'U'
#define MJOLNIR_CMD_RUN_DOWNLEFT     'B'
#define MJOLNIR_CMD_RUN_DOWNRIGHT    'N'
#define MJOLNIR_CMD_GO_LEFT          CTRL('h')
#define MJOLNIR_CMD_GO_DOWN          CTRL('j')
#define MJOLNIR_CMD_GO_UP            CTRL('k')
#define MJOLNIR_CMD_GO_RIGHT         CTRL('l')
#define MJOLNIR_CMD_GO_UPLEFT        CTRL('y')
#define MJOLNIR_CMD_GO_UPRIGHT       CTRL('u')
#define MJOLNIR_CMD_GO_DOWNLEFT      CTRL('b')
#define MJOLNIR_CMD_GO_DOWNRIGHT     CTRL('n')
#define MJOLNIR_CMD_FIGHT            'f'
#define MJOLNIR_CMD_THROW            't'
#define MJOLNIR_CMD_MOVE             'm'
#define MJOLNIR_CMD_ZAP              'z'
#define MJOLNIR_CMD_IDENTIFY_TRAP    '^'
#define MJOLNIR_CMD_SEARCH           's'
#define MJOLNIR_CMD_LEVEL_DOWN       '>'
#define MJOLNIR_CMD_LEVEL_UP         '<'
#define MJOLNIR_CMD_REST             '.'
#define MJOLNIR_CMD_PICK_UP          ','
#define MJOLNIR_CMD_INVENTORY        'i'
#define MJOLNIR_CMD_INVENTORY_SINGLE 'I'
#define MJOLNIR_CMD_QUAFF            'q'
#define MJOLNIR_CMD_READ             'r'
#define MJOLNIR_CMD_EAT              'e'
#define MJOLNIR_CMD_WIELD            'w'
#define MJOLNIR_CMD_WEAR             'W'
#define MJOLNIR_CMD_TAKE_OFF         'T'
#define MJOLNIR_CMD_PUT_RING_ON      'P'
#define MJOLNIR_CMD_REMOVE_RING      'R'
#define MJOLNIR_CMD_DROP             'd'
#define MJOLNIR_CMD_CALL             'c'
#define MJOLNIR_CMD_REPEAT           'a'
#define MJOLNIR_CMD_PRINT_WEAPON     ')'
#define MJOLNIR_CMD_PRINT_ARMOR      ']'
#define MJOLNIR_CMD_PRINT_RINGS      '='
#define MJOLNIR_CMD_PRINT_STATS      '@'
#define MJOLNIR_CMD_RECALL           'D'
#define MJOLNIR_CMD_OPTIONS          'o'
#define MJOLNIR_CMD_REDRAW           CTRL('R')
#define MJOLNIR_CMD_REPEAT_MESSAGE   CTRL('P')
#define MJOLNIR_CMD_CANCEL           CTRL('[')
#define MJOLNIR_CMD_SAVE             'S'
#define MJOLNIR_CMD_QUIT             'Q'
/* mjolnir extensions */
#define MJOLNIR_CMD_USE              'u'
#define MJOLNIR_CMD_TURN_ON          '0'
#define MJOLNIR_CMD_TURN_OFF         '1'

#define MJOLNIR_SCR_NONE             0
#define MJOLNIR_SCR_VGA_TEXT         1
#define MJOLNIR_SCR_TTY              2
#define MJOLNIR_SCR_X11              3
struct mjolgame {
    struct dnggame      data;
    struct mjolchr     *player;
    char               *nick;           // names of players
    struct mjolobj    **inventory;      // inventory
    long                scrtype;        // type of screen to use
    struct mjolscr     *scr;            // screen interface
    struct mjolchr  ****chrtab;         // in-dungeon live characters
    long                nlvl;           // # of levels
    long                lvl;            // current level
    struct mjolobj  ****objtab;         // level data
    long               *nroomtab;       // per-level # of rooms
    struct mjolroom  ***lvltab;         // level data
    long                width;          // width of level in cells
    long                height;         // height of level
    long                nobj;           // # of objects
};

#define MJOLNIR_DIR_VERTICAL    0x00000001
#define MJOLNIR_ROOM_CONNECTED  0x00000002
struct mjolroom {
#if (MJOLNIR_ROOM_IDS)
    int              id;
    int              lvlid;
#endif
    long             lvl;
    long             x;
    long             y;
    long             width;
    long             height;
#if (MJOLHACKS)
    long             roomx;
    long             roomy;
    long             roomw;
    long             roomh;
#endif
    long             flg;
//    struct mjolroom *parent;
    struct mjolroom *part1;
    struct mjolroom *part2;
    struct mjolroom *next;
};

struct mjollvl {
    long              nroom;
    struct mjolroom **roomtab;
};

#if (TEST)
static __inline__ void
mjolprintroom(struct mjolroom *room)
{
    fprintf(stderr, "x = %ld, y = %ld, width = %ld, height = %ld\n",
            room->x, room->y, room->width, room->height);
}
#endif

static __inline__ void
mjolpushchase(struct mjolchr *data)
{
    struct mjolchr *next;

    data->data.prev = NULL;
    next = chaseq;
    if (chaseq) {
        chaseq->data.prev = data;
    }
    data->data.next = next;
    chaseq = data;

    return;
}

static __inline__ struct mjolchr *
mjolpopchase(void)
{
    struct mjolchr *next = chaseq->data.next;
    struct mjolchr *data = chaseq;

    if (data) {
        if (next) {
            next->data.prev = NULL;
        }
        chaseq = next;
    }

    return data;
}

static __inline__ void
mjolrmchase(struct mjolchr *data)
{
    struct mjolchr *prev = data->data.prev;
    struct mjolchr *next = data->data.next;

    if (next) {
        next->data.prev = prev;
    }
    if (prev) {
        prev->data.next = next;
    } else {
        if (next) {
            next->data.prev = NULL;
        }
        chaseq = next;
    }

    return;
}

typedef long             mjolcmdfunc(struct mjolchr *src,
                                     struct mjolobj *dest);
typedef struct mjolobj * mjolcmdmovefunc(struct mjolgame *game,
                                         struct mjolchr *chr);

#endif /* __MJOLNIR_MJOL_H__ */

