#ifndef __GAMES_DEICE_DEICE_H__
#define __GAMES_DEICE_DEICE_H__

#include <dice/d20.h>

#define DEICE_MAX_TURNS             128
#define DEICE_MAX_LEVEL             32

#define MJOLNIR_PROGRAMMER_CLASS    0
#define MJOLNIR_CRACKER_CLASS       1
#define MJOLNIR_CYBORG_CLASS        2
#define MJOLNIR_THIEF_CLASS         3
#define MJOLNIR_ENGINEER_CLASS      4
#define MJOLNIR_CHARACTER_CLASSES   5

struct deicestat {
    long                            chrclass;
    long                            xp;
    long                            lvl;
    long                            basehp;
    long                            maxhp;
    long                            nhp;
    long                            defdie;
    long                            hitdie;
    long                            ndefdice;    // dice set for defense
    long                            nhitdice;    // dice set for attack
};

/* global game structure */
struct deice {
    struct deicestat               *stats;
};

#endif /* __GAMES_DEICE_DEICE_H__ */

