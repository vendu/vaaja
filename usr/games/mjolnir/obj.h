#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#include <dungeon/conf.h>
#include <dungeon/dng.h>

/* objects */
#define MJOLNIR_OBJ_FLOOR            '.'
#define MJOLNIR_OBJ_CORRIDOR         '#'
#define MJOLNIR_OBJ_HORIZONTAL_WALL  '-'
#define MJOLNIR_OBJ_VERTICAL_WALL    '|'
#define MJOLNIR_OBJ_DOOR             '+'
#define MJOLNIR_OBJ_FOOD             '%'
#define MJOLNIR_OBJ_WATER            '~'
#define MJOLNIR_OBJ_FOUNTAIN         '{'
#define MJOLNIR_OBJ_GOLD             '$'
#define MJOLNIR_OBJ_BULLET           'o'
#define MJOLNIR_OBJ_POTION           '!'
#define MJOLNIR_OBJ_PLANT            '*'
#define MJOLNIR_OBJ_PUNCHCARD        '='
#define MJOLNIR_OBJ_TAPE             '8'
#define MJOLNIR_OBJ_STAIR_DOWN       '<'
#define MJOLNIR_OBJ_STAIR_UP         '>'
#define MJOLNIR_OBJ_STATUE           '&'
#define MJOLNIR_OBJ_TRAP             '^'
#define MJOLNIR_OBJ_WAND             '\\'
#define MJOLNIR_OBJ_SCROLL           '?'
#define MJOLNIR_OBJ_RING             '='
#define MJOLNIR_OBJ_WHIP             '/'
#define MJOLNIR_OBJ_ARMOR            ']'
#define MJOLNIR_OBJ_CHAIN            'c'
#define MJOLNIR_OBJ_CHEST            'C'
#define MJOLNIR_OBJ_SUBMACHINE_GUN   'g'
#define MJOLNIR_OBJ_HONEY            'h'
#define MJOLNIR_OBJ_JATIMATIC        'J'
#define MJOLNIR_OBJ_KNIFE            'k'
#define MJOLNIR_OBJ_LOCKPICK         'l'
#define MJOLNIR_OBJ_LASER            'L'
#define MJOLNIR_OBJ_MACE             'm'
#define MJOLNIR_OBJ_MAINFRAME        'M'
#define MJOLNIR_OBJ_PIPE             'p'
#define MJOLNIR_OBJ_PISTOL           'P'
#define MJOLNIR_OBJ_LONGBOW         ')'
#define MJOLNIR_OBJ_SWORD            's'
#define MJOLNIR_OBJ_TELEPORT         't'
#define MJOLNIR_OBJ_WELL             'w'
#define MJOLNIR_OBJ_CROSS            'x'
#define MJOLNIR_OBJ_ALTAR            'X'

/* special items */
#define MJOLNIR_ITEM_DEMON_WHIP      1
#define MJOLNIR_ITEM_JATIMATIC       2
#define MJOLNIR_ITEM_MJOLNIR         3
#define MJOLNIR_ITEM_PLEIGNIR        4
#define MJOLNIR_ITEM_STORMBRINGER    5
#define MJOLNIR_ITEM_EXCALIBUR       6
#define MJOLNIR_ITEM_HOLY_CROSS      7
#define MJOLNIR_ITEM_HOLY_WATER      8
#define MJOLNIR_ITEM_SILVER_BULLET   9

struct mjolobjfunc {
    char  *str;
    long (*hit)(void *, void *);
    long (*def)(void *, void *);
    long (*pick)(void *, void *);
};

/* data.flg values */
#define MJOLNIR_OBJ_HIDDEN  0x00000001
/* bless values */
#define MJOLNIR_OBJ_BLESSED 1
#define MJOLNIR_OBJ_NEUTRAL 0
#define MJOLNIR_OBJ_CURSED  (-1)
struct mjolobj {
#if (MJOLNIR_VT) || (MJOLNIR_CURSES)
//    int                 id;
    long                id;
    long                special;        // non-0 for special items
#endif
    long                flg;
    struct dngobj       data;           // common object data
    struct mjolobjfunc  func;
    long                weight;         // weight of object
    long                bless;          // BLESSED, NEUTRAL, CURSED
    long                parm;           // e.g. +1 or -1 for armor
    struct mjolobj     *prev;
    struct mjolobj     *next;
};

#if 0
/* event handler function prototype */
typedef void mjolfunc_t(struct dnggame *game,
                        struct dngobj *src, struct dngobj *dest);
#endif

typedef struct mjolobj * mjolmkfunc(void);

extern struct mjolobj * mjolmkfloor(void);
extern struct mjolobj * mjolmkcorridor(void);
extern struct mjolobj * mjolmkdoor(void);
extern struct mjolobj * mjolmkhorizwall(void);
extern struct mjolobj * mjolmkvertwall(void);
extern struct mjolobj * mjolmkstair(void);

#endif /* __MJOLNIR_OBJ_H__ */

