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
#define MJOLNIR_CHAR_SHOPKEEPER     '£'
#define MJOLNIR_CHAR_UNICORN        'u'
#define MJOLNIR_CHAR_VAMPIRE        'v'
#define MJOLNIR_CHAR_WEREWOLF       'w'
#define MJOLNIR_CHAR_WIZARD         'W'
#define MJOLNIR_CHAR_ZOMBIE         'Z'
/* special characters */
#define MJOLNIR_CHAR_FENRIS         'F'
#define MJOLNIR_CHAR_KARA           'K'
#define MJOLNIR_CHAR_THOR           'T'
#define MJOLNIR_CHAR_UKKO           'U'
#define MJOLNIR_CHAR_DRACULA        'D'

/* alignment values */
#define MJOLNIR_CHAR_CHAOTIC        (-1)
#define MJOLNIR_CHAR_NEUTRAL        0'
#define MJOLNIR_CHAR_LAWFUL         1'

/* character flags */
#define MJOLNIR_CHAR_BLIND          (1 << 0)    // character blind
#define MJOLNIR_CHAR_LEVITATES      (1 << 1)    // character levitating
#define MJOLNIR_CHAR_INVISIBLE      (1 << 2)    // character invisible
#define MJOLNIR_CHAR_STARVING       (1 << 3)    // character starving
#define MJOLNIR_CHAR_DROWNING       (1 << 4)    // character drowning
#define MJOLNIR_CHAR_SINKING        (1 << 5)    // character sinking
#define MJOLNIR_CHAR_POISONED       (1 << 5)    // ghoul-bite or other poison
#define MJOLNIR_CHAR_DRUNK          (1 << 6)    // character drunk
#define MJOLNIR_CHAR_HIGH           (1 << 7)    // character is high

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

#if 0
struct mjolchr {
    char               *name;           // character name string
    /* Rogue [visible] attributes */
    long                hp;             // current hitpoints
    long                maxhp;          // max hitpoints
    long                gold;           // current gold
    long                str;            // current strength
    long                maxstr;         // max strength
    long                armor;          // armor strength
    long                exp;            // experience
    long                lvl;            // level
    long                aln;            // alignment; CHAOTIC, NEUTRAL, LAWFUL
    long                flg;            // status bits
    struct mjolobjfunc  func;
    /* mjolnir [hidden] attributes */
    long                power;          // power
    long                luck;           // luck-factor
    long                dex;            // dexterity
    long                intl;           // intelligence
    long                def;            // defense
    long                lock;           // lock-pick skill
    long                spell;          // skill-level for casting spells
    long                throw;          // skill-level for throwing objects
    long                knife;          // skill-level for knives
    long                gun;            // skill-level for guns
    long                sword;          // skill-level for swords
    long                whip;           // skill-level for whips
    long                speed;          // FAST, NORMAL, FROZEN, SLOW
    unsigned long       turn;           // next turn ID
    unsigned long       nturn;          // # of turns used
};
#endif

#endif /* __MJOLNIR_CHR_H__ */

