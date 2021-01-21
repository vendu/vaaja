#ifndef __MJOLNIR_ITEM_H__
#define __MJOLNIR_ITEM_H__

/* items */
#define MJOLNIR_ITEM_FLOOR              ' '
#define MJOLNIR_ITEM_WALL               '#'
#define MJOLNIR_ITEM_DOOR               '+'
#define MJOLNIR_ITEM_SAND               '.'
#define MJOLNIR_ITEM_STAIR_DOWN         '>'
#define MJOLNIR_ITEM_STAIR_UP           '<'
#define MJOLNIR_ITEM_COFFIN             '%'
#define MJOLNIR_ITEM_ATM_CARD           '-'
#define MJOLNIR_ITEM_CASH               'C'
#define MJOLNIR_ITEM_ATM_MACHINE        '|'
#define MJOLNIR_ITEM_BOOK               '9'
#define MJOLNIR_ITEM_FOOD               'f'
#define MJOLNIR_ITEM_WATER              '~'
#define MJOLNIR_ITEM_FOUNTAIN           '{'
#define MJOLNIR_ITEM_GOLD               '$'
#define MJOLNIR_ITEM_BULLET             '='
#define MJOLNIR_ITEM_POTION             '!'
#define MJOLNIR_ITEM_PLANT              '*'
#define MJOLNIR_ITEM_PUNCHCARD          ':'
#define MJOLNIR_ITEM_TAPE               '8'
#define MJOLNIR_ITEM_STATUE             '&'
#define MJOLNIR_ITEM_TRAP               '^'
#define MJOLNIR_ITEM_RING               'o'
#define MJOLNIR_ITEM_WHIP               '\\'
#define MJOLNIR_ITEM_ARMOR              ']'
#define MJOLNIR_ITEM_MIRROR             '['
#define MJOLNIR_ITEM_CHEST              'c'
#define MJOLNIR_ITEM_SUBMACHINE_GUN     'g'
#define MJOLNIR_ITEM_BEEHIVE            'h'
#define MJOLNIR_ITEM_HAMMER             't'
#define MJOLNIR_ITEM_IMPLANT            'i'
#define MJOLNIR_ITEM_KATANA             'k'
#define MJOLNIR_ITEM_CROWBAR            'j'
#define MJOLNIR_ITEM_LOCKPICK           'l'
#define MJOLNIR_ITEM_LASER              'L'
#define MJOLNIR_ITEM_MACE               'm'
#define MJOLNIR_ITEM_MAINFRAME          '0'
#define MJOLNIR_ITEM_PAPER              'p'
#define MJOLNIR_ITEM_PIPE               '?'
#define MJOLNIR_ITEM_PISTOL             '7'
#define MJOLNIR_ITEM_REMNANTS           'R'
#define MJOLNIR_ITEM_SOFTWARE           's'
#define MJOLNIR_ITEM_SWORD              '/'
#define MJOLNIR_ITEM_TERMINAL           'T'
#define MJOLNIR_ITEM_CROSS              'x'
#define MJOLNIR_ITEM_ALTAR              'X'
/* special items */
#define MJOLNIR_ITEM_CRYSTAL_BALL       'Q'
#define MJOLNIR_ITEM_JATIMATIC          'J'
#define MJOLNIR_ITEM_BOOMERANG          ')'
#define MJOLNIR_ITEM_MJOLNIR            'M'
#define MJOLNIR_ITEM_GLEIPNIR           'G'
#define MJOLNIR_ITEM_MAX_TYPES          128 // symbols in ASCII

/* subtypes */
#define MJOLNIR_ITEM_NORMAL_SAND        0
#define MJOLNIR_ITEM_QUICK_SAND         1
#define MJOLNIR_ITEM_LEATHER_WHIP       0
#define MJOLNIR_ITEM_DEMON_WHIP         1
#define MJOLNIR_ITEM_KATANA_SWORD       0
#define MJOLNIR_ITEM_GOLDEN_SWORD       1
#define MJOLNIR_ITEM_STORMBRINGER_SWORD 2
#define MJOLNIR_ITEM_EXCALIBUR_SWORD    3
#define MJOLNIR_ITEM_SILVER_CROSS       0
#define MJOLNIR_ITEM_EZEKIEL_CROSS      1
#define MJOLNIR_ITEM_IRON_HAMMER        0
#define MJOLNIR_ITEM_MJOLNIR_HAMMER     1
#define MJOLNIR_ITEM_MAX_SUBTYPES       4

/* flg values */
#define MJOLNIR_ITEM_HIDDEN             (1L << 0)

#endif /* __MJOLNIR_ITEM_H__ */

