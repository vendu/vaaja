#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#define MJOLNIR_OBJ_CHARACTER       1
#define MJOLNIR_OBJ_ITEM            2

/* character categories */
#define MJOLNIR_PROGRAMMER_CATEGORY 1
#define MJOLNIR_CRACKER_CATEGORY    2
#define MJOLNIR_CYBORG_CATEGORY     3
#define MJOLNIR_SOLDIER_CATEGORY    4
#define MJOLNIR_THIEF_CATEGORY      5
#define MJOLNIR_ENGINEER_CATEGORY   6
#define MJOLNIR_MAX_CATEGORY        6

/* negative value of [-16, -1] for evil alignment */
#define MJOLNIR_NEUTRAL_ALIGNMENT   0
/* positive value of [1, 16] for evil alignment */

struct objchr {
    char                           *name;       // character name
    long                            cat;        // category, 'class'
    long                            type;       // type/ASCII-presentation
    long                            flg;        // character status flags
    long                            aln;        // alighnment
    long                            str;        // strength
    long                            con;        // conecntration (for spells)
    long                            exp;        // experience points
    long                            lvl;        // experience level
    struct budo                     budo;       // budo close-combat attributes
    struct ice                      ice;        // ice subgame structure
};

#if 0
struct chrcatname {
    char                           *def;
    char                           *name;
};
#endif

struct objitem {
    char                           *name;       // object name (inventory etc.)
    long                            type;       // type/ASCII-presentation
    long                            cat;        // category/subtype
    long                            flg;        // object status flags
    long                            str;        // strength for attack (weapon)
    long                            def;        // non-zero e.g. for armors
    struct d20diceset               atkdice;
    struct d20diceset               defdice;
};

#endif /* __MJOLNIR_OBJ_H__ */

