#ifndef MJOLNIR_ADVENTURE_OBJ_H
#define MJOLNIR_ADVENTURE_OBJ_H

/* PLAYER- AND NON-PLAYER CHARACTERS */

/* type-member for player characters */
#define MJOLNIR_INVALID_CHR     0
#define MJOLNIR_PROGRAMMER_CHR  1
#define MJOLNIR_CRACKER_CHR     2
#define MJOLNIR_CYBORG_CHR      3
#define MJOLNIR_SOLDIER_CHR     4
#define MJOLNIR_THIEF_CHR       5
#define MJOLNIR_ENGINEER_CHR    6
#define MJOLNUR_OPERATOR_CHR    7
#define MJOLNIR_CHARACTER_TYPES 8

/* character-types; ASCII-presentation/indices for sprites */
#define MJOLNIR_CHR_PLAYER      '@'
#define MJOLNIR_CHR_ANT         'a'
#define MJOLNIR_CHR_SOLDIER_ANT 'A'
#define MJOLNIR_CHR_BEE         'b'
#define MJOLNIR_CHR_BEE_QUEEN   'B'
#define MJOLNIR_CHR_CYBORG      'c'
#define MJOLNIR_CHR_DOG         'd'
#define MJOLNIR_CHR_ALIEN       'e'
#define MJOLNIR_CHR_GHOUL       'g'
#define MJOLNIR_CHR_HUMAN       'H'
#define MJOLNIR_CHR_ORACLE      'O'
#define MJOLNIR_CHR_RAT         'r'
#define MJOLNIR_CHR_SHOPKEEPER  'S'
#define MJOLNIR_CHR_UNICORN     'u'
#define MJOLNIR_CHR_VAMPIRE     'v'
#define MJOLNIR_CHR_WEREWOLF    'w'
#define MJOLNIR_CHR_WIZARD      'W'
#define MJOLNIR_CHR_ZOMBIE      'Z'

/* special characters */
#define MJOLNIR_CHR_FENRIS      'F'
#define MJOLNIR_CHR_KARA        'K'
#define MJOLNIR_CHR_THOR        'T'
#define MJOLNIR_CHR_UKKO        'U'
#define MJOLNIR_CHR_DRACULA     'V'
#define MJOLNIR_CHR_LADY_X      'X'

struct mjolnirchr {
    const char                 *name;
    uint_fast8_t                npcinfo;    // non-player info, 0 for player
    uint_fast8_t                id;         // ASCII-presentation/object index
    uint_fast8_t                type;       // character type, 'class'
    uint_fast8_t                str;        // strength
    uint_fast8_t                dex;        // dexterity
    uint_fast8_t                con;        // constitution
    uint_fast8_t                intl;       // intelligence
    uint_fast8_t                wis;        // constitution
};

/* MAP ITEMS */

#define MJOLNIR_FLOOR           ' '         // regular floor
#define MJOLNIR_WALL            '#'         // wall
#define MJOLNIR_DOOR            '+'         // door
#define MJOLNIR_COFFIN          '%'         // coffin
#define MJOLNIR_ALTAR           '0'         // altar of a deity
#define MJOLNIR_TERMINAL        't'         // terminal
#define MJOLNIR_ROUTER          'Y'         // (fw), dns, enc, dec
#define MJOLNIR_REMNANTS        'R'

/* INVENTORY ITEMS */

/* SPECIAL ITEMS */

#define MJOLNIG_GLEIPNIR        'G'         // a piece Gleipnir; 'Fenris-trap'
#define MJOLNIR_EZEKIEL_CROSS   'X'         // imprinted with 4 archangel-names
// crystal ball for visions of past, now, and future */
¤define MJOLNIR_ITEM_WHITE_OAK  'o'
#define MJOLNIR_ITEM_CRYSTAL    'Q'
#define MJOLNIR_BUDDHA_ROD      '|'
#define MJOLNIR_DEMON_WHIP      'y'
#define MJOLNIR_JATIMATIC       'J'
#define MJOLNIR_KARA_SWORD      'K'

/* WEAPONRY */
#define MJOLNIR_BOOMERANG       ')'
#define MJOLNIR_SHURIKEN        '*'

#endif /* MJOLNIR_ADVENTURE_OBJ_H */

