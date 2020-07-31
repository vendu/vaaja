#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#include <dungeon/conf.h>
#include <dungeon/dng.h>

/* objects */
#define MJOLNIR_OBJ_FLOOR           ' '
#define MJOLNIR_OBJ_SAND            '.'
#define MJOLNIR_OBJ_WALL            '#'
#define MJOLNIR_OBJ_DOOR            '+'
#define MJOLNIR_OBJ_STAIR_DOWN      '>'
#define MJOLNIR_OBJ_STAIR_UP        '<'
#define MJOLNIR_ATM_CARD            '-'
#define MJOLNIR_ATM_MACHINE         'C'
#define MJOLNIR_OBJ_FOOD            'f'
#define MJOLNIR_OBJ_WATER           '~'
#define MJOLNIR_OBJ_FOUNTAIN        '{'
#define MJOLNIR_OBJ_GOLD            '$'
#define MJOLNIR_OBJ_BULLET          '='
#define MJOLNIR_OBJ_POTION          '!'
#define MJOLNIR_OBJ_PLANT           '*'
#define MJOLNIR_OBJ_PUNCHCARD       '0'
#define MJOLNIR_OBJ_TAPE            't'
#define MJOLNIR_OBJ_STATUE          '&'
#define MJOLNIR_OBJ_TRAP            '^'
#define MJOLNIR_OBJ_WAND            '\\'
#define MJOLNIR_OBJ_SCROLL          '9'
#define MJOLNIR_OBJ_RING            'o'
#define MJOLNIR_OBJ_WHIP            '/'
#define MJOLNIR_OBJ_ARMOR           ']'
#define MJOLNIR_OBJ_MIRROR          '['
#define MJOLNIR_OBJ_CHEST           'c'
#define MJOLNIR_OBJ_SUBMACHINE_GUN  'g'
#define MJOLNIR_OBJ_HONEY           'h'
#define MJOLNIR_OBJ_KNIFE           'k'
#define MJOLNIR_OBJ_KALEIDOSCOPE    'K'
#define MJOLNIR_OBJ_LOCKPICK        'l'
#define MJOLNIR_OBJ_LASER           'L'
#define MJOLNIR_OBJ_MACE            'm'
#define MJOLNIR_OBJ_MAINFRAME       '8'
#define MJOLNIR_OBJ_PIPE            '?'
#define MJOLNIR_OBJ_PISTOL          'P'
#define MJOLNIR_OBJ_SWORD           's'
#define MJOLNIR_OBJ_TERMINAL        'T'
#define MJOLNIR_OBJ_WELL            'w'
#define MJOLNIR_OBJ_CROSS           'x'
#define MJOLNIR_OBJ_ALTAR           'X'

/* special items */
#define MJOLNIR_OBJ_CRYSTAL_BALL    'Q'
#define MJOLNIR_OBJ_DEMON_WHIP      '|'
#define MJOLNIR_OBJ_JATIMATIC       'J'
#define MJOLNIR_OBJ_GOLDEN_SWORD    '('
#define MJOLNIR_OBJ_MJOLNIR         'M'
#define MJOLNIR_OBJ_GLEIPNIR        'G'
#define MJOLNIR_OBJ_STORMBRINGER    'S'
#define MJOLNIR_OBJ_EXCALIBUR       'E'
#define MJOLNIR_OBJ_CROSS_OF_LIGHT  'x'
#define MJOLNIR_OBJ_HOLY_WATER      '~'
#define MJOLNIR_OBJ_SILVER_BULLET   '='

struct mjolobjfunc {
    char   *str;
    long  (*hit)(void *, void *);
    long  (*def)(void *, void *);
    long  (*pick)(void *, void *);
};

/* data.flg values */
#define MJOLNIR_OBJ_HIDDEN              (1L << 0)
#define MJOLNIR_OBJ_HIDDEN_TRAP         (1L << 1)
#define MJOLNIR_OBJ_HIDDEN_QUICK_SAND   (1L << 2)
#define MJOLNIR_OBJ_HIDDEN_CARVING      (1L << 3)
#define MJOLNIR_OBJ_HIDDEN_DOOR         (1L << 4)
/* bless values */
#define MJOLNIR_OBJ_BLESSED             1
#define MJOLNIR_OBJ_NEUTRAL             0
#define MJOLNIR_OBJ_CURSED              (-1)

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
extern struct mjolobj * mjolmksand(void);
extern struct mjolobj * mjolmkwall(void);
extern struct mjolobj * mjolmkdoor(void);
extern struct mjolobj * mjolmkstairup(void);
extern struct mjolobj * mjolmkstairdown(void);

#endif /* __MJOLNIR_OBJ_H__ */

