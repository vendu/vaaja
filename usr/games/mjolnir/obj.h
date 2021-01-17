#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#define MJOLNIR_OBJ_CHARACTER       1
#define MJOLNIR_OBJ_ITEM            2
#define MJOLNIR_OBJ_DECK            3
#define MJOLNIR_OBJ_PROG            4

/* character categories */
#define MJOLNIR_PROGRAMMER_CATEGORY 1
#define MJOLNIR_CRACKER_CATEGORY    2
#define MJOLNIR_CYBORG_CATEGORY     3
#define MJOLNIR_SOLDIER_CATEGORY    4
#define MJOLNIR_THIEF_CATEGORY      5
#define MJOLNIR_ENGINEER_CATEGORY   6
#define MJOLNIR_MAX_CATEGORY        6

struct objchr {
    char                           *name;       // character name
    long                            cat;        // category, 'class'
    long                            type;       // type/ASCII-presentation
    long                            flg;        // character status flags
    long                            xp;         // experience points
    long                            str;        // base strength for combat
    long                            ref;        // reflex value for saves
    long                            prob;       // hit success probability
    long                            rank;       // close combat rank
    struct d20dice                 *hitdice;    // dice-set for attack
    struct ice                      ice;        // ice subgame structure
};

#if 0
struct chrcatname {
    char                           *def;
    char                           *name;
};
#endif

struct objitem {
    char                           *name;       // object name for inventory etc.
    long                            type;       // type/ASCII-presentation
    long                            cat;        // category/subtype
    long                            flg;        // object status flags
    //    long                        hp;     // e.g. +1 or -1 for armor
    struct d20diceset               atkdice;
    struct d20diceset               defdice;
};

#endif /* __MJOLNIR_OBJ_H__ */

