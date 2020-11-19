#ifndef __MJOLNIR_ICE_ICE_H__
#define __MJOLNIR_ICE_ICE_H__

#include <mjolnir/d20.h>

#define ICE_MAX_TURNS           128
#define ICE_MAX_LEVEL           32

struct ice {
    long                        xp;         // program experience
    long                        lvl;        // program level
    long                        basehp;     // program hitpoint increment
    long                        maxhp;      // program max # of hitpoints
    long                        nhp;        // program current # of hitpoints
    long                        defdie;     // die to roll for defense
    long                        hitdie;     // die to roll for attack
    long                        ndefdice;   // # of dice for defense
    long                        nhitdice;   // # of dice for attack
};

#endif /* __MJOLNIR_ICE_ICE_H__ */

