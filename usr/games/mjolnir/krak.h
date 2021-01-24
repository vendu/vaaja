#ifndef __MJOLNIR_KRAK_H__
#define __MJOLNIR_KRAK_H__

void                            krakinitchr(struct objchr *chr,
                                           const char *name,
                                           long cat);

#define KRAK_MAX_TURNS           128
#define KRAK_MAX_LEVEL           32

<<<<<<< HEAD:usr/games/mjolnir/krak.h
<<<<<<< HEAD:usr/games/mjolnir/krak.h
struct krak {
    long                        exp;        // program experience points
    long                        lvl;        // program level
    //    long                        basehp;     // program hitpoint increment
    long                        maxhp;      // program max # of hitpoints
    long                        nhp;        // program current # of hitpoints
=======
=======
>>>>>>> 115ea79 (...):usr/games/mjolnir/ice.h
struct ice {
    long                        exp;        // ICE experience points
    long                        lvl;        // ICE level
    long                        maxhp;      // ICE max # of hitpoints
    long                        nhp;        // ICE current # of hitpoints
<<<<<<< HEAD:usr/games/mjolnir/krak.h
>>>>>>> 115ea79 (...):usr/games/mjolnir/ice.h
=======
>>>>>>> 115ea79 (...):usr/games/mjolnir/ice.h
    long                        atk;        // basic strength of attack
    long                        refprob;    // probability for reflex save
    struct d20dice              hitdice;    // hit die set
    struct d20dice              defdice;    // defense die set
};

#endif /* __MJOLNIR_KRAK_H__ */

