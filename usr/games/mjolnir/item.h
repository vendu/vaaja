#ifndef __MJOLNIR_ITEM_H__
#define __MJOLNIR_ITEM_H__

#include <dungeon/conf.h>
#include <dungeon/dng.h>

/* items */
#define MJOLNIR_ITEM_FLOOR                  ' '
#define MJOLNIR_ITEM_WALL                   '#'
#define MJOLNIR_ITEM_DOOR                   '+'
#define MJOLNIR_ITEM_SAND                   '.'
#define MJOLNIR_ITEM_STAIR_DOWN             '>'
#define MJOLNIR_ITEM_STAIR_UP               '<'
#define MJOLNIR_ITEM_COFFIN                 '%'
#define MJOLNIR_ITEM_ATM_CARD               '-'
#define MJOLNIR_ITEM_ATM_MACHINE            'C'
#define MJOLNIR_ITEM_FOOD                   'f'
#define MJOLNIR_ITEM_WATER                  '~'
#define MJOLNIR_ITEM_FOUNTAIN               '{'
#define MJOLNIR_ITEM_GOLD                   '$'
#define MJOLNIR_ITEM_MONEY                  '¤'
#define MJOLNIR_ITEM_BULLET                 '='
#define MJOLNIR_ITEM_POTION                 '!'
#define MJOLNIR_ITEM_PLANT                  '*'
#define MJOLNIR_ITEM_PUNCHCARD              '0'
#define MJOLNIR_ITEM_TAPE                   't'
#define MJOLNIR_ITEM_STATUE                 '&'
#define MJOLNIR_ITEM_TRAP                   '^'
#define MJOLNIR_ITEM_WAND                   '\\'
#define MJOLNIR_ITEM_SCROLL                 '9'
#define MJOLNIR_ITEM_RING                   'o'
#define MJOLNIR_ITEM_WHIP                   '/'
#define MJOLNIR_ITEM_ARMOR                  ']'
#define MJOLNIR_ITEM_MIRROR                 '['
#define MJOLNIR_ITEM_CHEST                  'c'
#define MJOLNIR_ITEM_SUBMACHINE_GUN         'g'
#define MJOLNIR_ITEM_HONEY                  'h'
#define MJOLNIR_ITEM_KNIFE                  'k'
#define MJOLNIR_ITEM_CROWBAR                '¿'
#define MJOLNIR_ITEM_LOCKPICK               'l'
#define MJOLNIR_ITEM_LASER                  'L'
#define MJOLNIR_ITEM_MACE                   'm'
#define MJOLNIR_ITEM_MAINFRAME              '8'
#define MJOLNIR_ITEM_FORTUNE_COOKIE         '?'
#define MJOLNIR_ITEM_PIPE                   'P'
#define MJOLNIR_ITEM_PISTOL                 '7'
#define MJOLNIR_ITEM_REMNANTS               'R'
#define MJOLNIR_ITEM_SWORD                  's'
#define MJOLNIR_ITEM_TERMINAL               'T'
#define MJOLNIR_ITEM_WELL                   'w'
#define MJOLNIR_ITEM_CROSS                  'x'
#define MJOLNIR_ITEM_ALTAR                  'X'
/* special items */
#define MJOLNIR_ITEM_CRYSTAL_BALL           'Q'
#define MJOLNIR_ITEM_DEMON_WHIP             '|'
#define MJOLNIR_ITEM_JATIMATIC              'J'
#define MJOLNIR_ITEM_GOLDEN_SWORD           '('
#define MJOLNIR_ITEM_BOOMERANG              ')'
#define MJOLNIR_ITEM_MJOLNIR                'M'
#define MJOLNIR_ITEM_GLEIPNIR               'G'
#define MJOLNIR_ITEM_STORMBRINGER           'S'
#define MJOLNIR_ITEM_EXCALIBUR              'E'
#define MJOLNIR_ITEM_CROSS_OF_LIGHT         'x'

/* flg values */
#define MJOLNIR_ITEM_HIDDEN                 (1L << 0)
#define MJOLNIR_ITEM_HIDDEN_TRAP            (1L << 1)
#define MJOLNIR_ITEM_HIDDEN_QUICK_SAND      (1L << 2)
#define MJOLNIR_ITEM_HIDDEN_CARVING         (1L << 3)
#define MJOLNIR_ITEM_HIDDEN_DOOR            (1L << 4)
/* bless values */
#define MJOLNIR_ITEM_BLESSED                1
#define MJOLNIR_ITEM_NEUTRAL                0
#define MJOLNIR_ITEM_CURSED                 (-1)

struct mjolitem {
    char               *name;           // object name string for inventory etc.
    long                sym;            // ASCII-presentation
    long                id;             // unique object ID
    long                special;        // non-0 for special items
    long                flg;
    struct dngobj       data;           // common object data
    struct mjolobjfunc  func;
    long                weight;         // weight of object
    long                bless;          // BLESSED, NEUTRAL, CURSED
    long                parm;           // e.g. +1 or -1 for armor
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

#endif /* __MJOLNIR_ITEM_H__ */

