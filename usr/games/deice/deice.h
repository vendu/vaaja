#ifndef __GAMES_DEICE_DEICE_H__
#define __GAMES_DEICE_DEICE_H__

#define DEICE_MAX_TURNS         128
#define DEICE_MAX_LEVEL         32

struct deicestat {
    long                        xp;
    long                        lvl;
    long                        maxhp;
    long                        nhp;
};

struct deiceprog {
    long                        ndmin;      // minimum number of dice to roll
    long                        ndmax;      // maximum number of dice to roll
};

#endif /* __GAMES_DEICE_DEICE_H__ */

