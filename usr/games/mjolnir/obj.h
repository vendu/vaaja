#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#define MJOLNIR_OBJ_CHARACTER   1
#define MJOLNIR_OBJ_ITEM        2
#define MJOLNIR_OBJ_DECK        3
#define MJOLNIR_OBJ_PROG        4

/* character categories */
#define CHR_PROGRAMMER_CATEGORY 1
#define CHR_CRACKER_CATEGORY    2
#define CHR_CYBORG_CATEGORY     3
#define CHR_SOLDIER_CATEGORY    4
#define CHR_THIEF_CATEGORY      5
#define CHR_ENGINEER_CATEGORY   6
#define CHR_MAX_CATEGORY        6

struct objchr {
    char                       *name;   // character name
    long                        cat;    // category, 'class'
    long                        type;   // type/ASCII-presentation
    long                        flg;    // character status flags
    long                        xp;
    long                        str;
    long                        prob;
    struct d20dice             *hitdice;
    struct ice                  ice;
};

struct chrcatname {
    char                       *def;
    char                       *name;
};

extern struct chrcatname        chrcatnames[CHR_MAX_CATEGORY + 1];

struct objitem {
    char                       *name;   // object name for inventory etc.
    long                        type;   // type/ASCII-presentation
    long                        cat;    // category/subtype
    long                        flg;    // object status flags
    //    long                        hp;     // e.g. +1 or -1 for armor
    struct d20dice              defdice;
};

#endif /* __MJOLNIR_OBJ_H__ */

