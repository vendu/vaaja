#ifndef __MJOLNIR_KRAK_H__
#define __MJOLNIR_KRAK_H__

//#define KRAK_MAX_TURNS           128
#define KRAK_MAX_LEVEL           128

struct krak {
    long                        exp;    // KRAK experience points
    long                        level;  // KRAK level
    long                        maxhp;  // KRAK max # of hitpoints
    long                        nhp;    // KRAK current # of hitpoints
    long                        prob;   // hit/defense probability
    long                        atk;    // basic power of attack
    struct d20dice              dice;   // hit/defense die set
};

#endif /* __MJOLNIR_KRAK_H__ */

