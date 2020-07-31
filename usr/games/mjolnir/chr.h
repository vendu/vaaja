#ifndef __MJOLNIR_CHR_H__
#define __MJOLNIR_CHR_H__

#include <dungeon/dng.h>
#include <mjolnir/obj.h>

/* characters */
#define MJOLNIR_CHAR_PLAYER         '@'
#define MJOLNIR_CHAR_ANT            'a'
#define MJOLNIR_CHAR_SOLDIER_ANT    'A'
#define MJOLNIR_CHAR_BEE            'b'
#define MJOLNIR_CHAR_BEE_QUEEN      'B'
#define MJOLNIR_CHAR_DOG            'd'
#define MJOLNIR_CHAR_ALIEN          'e'
#define MJOLNIR_CHAR_GHOUL          'g'
#define MJOLNIR_CHAR_HUMAN          'H'
#define MJOLNIR_CHAR_ORACLE         'O'
#define MJOLNIR_CHAR_RAT            'r'
#define MJOLNIR_CHAR_REMNANTS       'R'
#define MJOLNIR_CHAR_SHOPKEEPER     '£'
#define MJOLNIR_CHAR_UNICORN        'u'
#define MJOLNIR_CHAR_VAMPIRE        'v'
#define MJOLNIR_CHAR_WOLF           'w'
#define MJOLNIR_CHAR_WEREWOLF       'W'
#define MJOLNIR_CHAR_ZOMBIE         'Z'

#define MJOLNIR_CHAR_IKU_TURSO      'I'
#define MJOLNIR_CHAR_FENRIS         'F'
#define MJOLNIR_CHAR_KARA           'K'
#define MJOLNIR_CHAR_THOR           'T'
#define MJOLNIR_CHAR_DRACULA        'D'

/* character flags */
#define MJOLNIR_CHAR_NO_AUTOPICK    (1 << 0) // don't pick object up automatically
#define MJOLNIR_CHAR_BLIND          (1 << 1) // character is blind
#define MJOLNIR_CHAR_LEVITATES      (1 << 2) // character is levitating
#define MJOLNIR_CHAR_DRUNK          (1 << 3)
#define MJOLNIR_CHAR_STONED         (1 << 4)

/* speed values */
#define MJOLNIR_CHAR_FAST           2           // character is moving faster
#define MJOLNIR_CHAR_NORMAL         1           // normal speed
#define MJOLNIR_CHAR_FROZEN         0           // character can't move
#define MJOLNIR_CHAR_SLOW           (-1)        // slow speed

/*
 * - returns hit-point delta (negative)
 * - the argument is one of 8 directions
 */
typedef int   (*mjolmovefunc(int));
typedef long  (*hitfunc(struct mjolchr *, struct mjolchr *));

struct mjolchr {
    struct dngobj       data;           // common character data
    char               *name;
    struct mjolobjfunc  func;
    /* Rogue [visible] attributes */
    long                hp;             // current hitpoints
    long                maxhp;          // max hitpoints
    long                gold;           // current gold
    long                str;            // current strength
    long                maxstr;         // max strength
    long                arm;            // armor strength
    long                exp;            // experience
    long                lvl;            // level
    long                aln;            // alignment; CHAOTIC, NEUTRAL, LAWFUL
    /* mjolnir [hidden] attributes */
    long                pwr;
    long                dex;            // dexterity
    long                lock;           // lock-pick skill
    long                intl;           // intelligence
    long                def;            // defense
    long                speed;          // FAST, NORMAL, FROZEN, SLOW
    unsigned long       turn;           // next turn ID
    unsigned long       nturn;          // # of turns used
};

#endif /* __MJOLNIR_CHR_H__ */

