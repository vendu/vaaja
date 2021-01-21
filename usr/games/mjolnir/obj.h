#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#if 0
#define MJOLNIR_OBJ_CHARACTER       1
#define MJOLNIR_OBJ_ITEM            2
#define MJOLNIR_OBJ_DECK            3
#define MJOLNIR_OBJ_PROG            4
#endif

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
    long                            aln;        // alighnment
    long                            type;       // type/ASCII-presentation
    long                            flg;        // character status flags
    long                            exp;        // experience points
    long                            str;        // base strength for combat
    long                            con;        // conecntration (for spells)
    long                            ref;        // reflex value for saves
    long                            rank;       // close combat rank ('belt')
    long                            hitprob;    // hit success probability
    struct d20dice                 *hitdice;    // rank-based dice-set for atk
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

