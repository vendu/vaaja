#ifndef __MJOLNIR_ITEM_H__
#define __MJOLNIR_ITEM_H__

/* items */
#define ITEM_FLOOR              ' '
#define ITEM_WALL               '#'
#define ITEM_DOOR               '+'
#define ITEM_SAND               '.'
#define ITEM_STAIR_DOWN         '>'
#define ITEM_STAIR_UP           '<'
#define ITEM_COFFIN             '%'
#define ITEM_ATM_CARD           '-'
#define ITEM_CASH               '¤'
#define ITEM_ATM_MACHINE        'C'
#define ITEM_BOOK               '9'
#define ITEM_FOOD               'f'
#define ITEM_WATER              '~'
#define ITEM_FOUNTAIN           '{'
#define ITEM_GOLD               '$'
#define ITEM_BULLET             '='
#define ITEM_POTION             '!'
#define ITEM_PLANT              '*'
#define ITEM_PUNCHCARD          ':'
#define ITEM_TAPE               '8'
#define ITEM_STATUE             '&'
#define ITEM_TRAP               '^'
#define ITEM_WAND               '\\'
#define ITEM_RING               'o'
#define ITEM_WHIP               '/'
#define ITEM_ARMOR              ']'
#define ITEM_MIRROR             '['
#define ITEM_CHEST              'c'
#define ITEM_SUBMACHINE_GUN     'g'
#define ITEM_BEEHIVE            'h'
#define ITEM_HAMMER             't'
#define ITEM_IMPLANT            'i'
#define ITEM_KATANA             'k'
#define ITEM_CROWBAR            '¿'
#define ITEM_LOCKPICK           'l'
#define ITEM_LASER              'L'
#define ITEM_MACE               'm'
#define ITEM_MAINFRAME          '0'
#define ITEM_PAPER              'p'
#define ITEM_PIPE               '?'
#define ITEM_PISTOL             '7'
#define ITEM_REMNANTS           'R'
#define ITEM_SOFTWARE           's'
#define ITEM_SWORD              'S'
#define ITEM_TERMINAL           'T'
#define ITEM_CROSS              'x'
#define ITEM_ALTAR              'X'
/* special items */
#define ITEM_CRYSTAL_BALL       'Q'
#define ITEM_JATIMATIC          'J'
#define ITEM_BOOMERANG          ')'
#define ITEM_MJOLNIR            'M'
#define ITEM_GLEIPNIR           'G'
#define ITEM_MAX_TYPES          128 // symbols in ASCII

/* subtypes */
#define ITEM_NORMAL_SAND        0
#define ITEM_QUICK_SAND         1
#define ITEM_LEATHER_WHIP       0
#define ITEM_DEMON_WHIP         1
#define ITEM_KATANA_SWORD       0
#define ITEM_GOLDEN_SWORD       1
#define ITEM_STORMBRINGER_SWORD 2
#define ITEM_EXCALIBUR_SWORD    3
#define ITEM_SILVER_CROSS       0
#define ITEM_EZEKIEL_CROSS      1
#define ITEM_IRON_HAMMER        0
#define ITEM_MJOLNIR_HAMMER     1
#define ITEM_MAX_SUBTYPES       4

/* flg values */
#define ITEM_HIDDEN             (1L << 0)

#endif /* __MJOLNIR_ITEM_H__ */

