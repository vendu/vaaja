#ifndef __MJOLNIR_CHR_H__
#define __MJOLNIR_CHR_H__

#include <dungeon/dng.h>
#include <mjolnir/obj.h>

/* characters */
#define MJOLNIR_CHAR_PLAYER          '@'
#define MJOLNIR_CHAR_ANT             'a'
#define MJOLNIR_CHAR_ALIEN           'A'
#define MJOLNIR_CHAR_BEE             'b'
#define MJOLNIR_CHAR_BEE_QUEEN       'B'
#define MJOLNIR_CHAR_DOG             'd'
#define MJOLNIR_CHAR_DEITY           'D'
#define MJOLNIR_CHAR_GHOUL           'G'
#define MJOLNIR_CHAR_HUMAN           'H'
#define MJOLNIR_CHAR_ORACLE          'O'
#define MJOLNIR_CHAR_REMNANTS        'R'
#define MJOLNIR_CHAR_UNICORN         'u'
#define MJOLNIR_CHAR_THOR            'T'
#define MJOLNIR_CHAR_VAMPIRE         'v'
#define MJOLNIR_CHAR_DRACULA         'V'
#define MJOLNIR_CHAR_WOLF            'w'
#define MJOLNIR_CHAR_ZOMBIE          'Z'

/* character flags */
#define MJOLNIR_CHAR_NO_PICK   0x00000001U // don't pick object up automatically
#define MJOLNIR_CHAR_BLIND     0x00000020U // character is blind
#define MJOLNIR_CHAR_LEVITATES 0x00000040U // character is levitating
/* speed values */

#define MJOLNIR_CHAR_FAST      2           // character is moving faster
#define MJOLNIR_CHAR_NORMAL    1           // normal speed
#define MJOLNIR_CHAR_FROZEN    0           // character can't move
#define MJOLNIR_CHAR_SLOW      (-1)        // slow speed
struct mjolchr {
    struct dngobj      data;            // common character data
    struct mjolobjfunc func;
    /* Rogue [visible] attributes */
    long               hp;              // current hitpoints
    long               maxhp;           // max hitpoints
    long               gold;            // current gold
    long               str;             // current strength
    long               maxstr;          // max strength
    long               arm;             // armor strength
    long               exp;             // experience
    long               lvl;             // level
    long               aln;             // alignment; CHAOTIC, NEUTRAL, LAWFUL
    /* mjolnir [hidden] attributes */
    long               pwr;
    long               dex;             // dexterity
    long               lock;            // lock-pick skill
    long               intl;            // intelligence
    long               def;             // defense
    long               speed;           // FAST, NORMAL, FROZEN, SLOW
    unsigned long      turn;            // next turn ID
    unsigned long      nturn;           // # of turns used
};

#endif /* __MJOLNIR_CHR_H__ */

