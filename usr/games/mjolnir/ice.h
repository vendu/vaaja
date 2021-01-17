#ifndef __MJOLNIR_ICE_ICE_H__
#define __MJOLNIR_ICE_ICE_H__

void                            iceinitchr(struct objchr *chr,
                                           const char *name,
                                           long cat);

#define ICE_MAX_TURNS           128
#define ICE_MAX_LEVEL           32

struct ice {
    long                        xp;         // program experience
    long                        lvl;        // program level
    //    long                        basehp;     // program hitpoint increment
    long                        maxhp;      // program max # of hitpoints
    long                        nhp;        // program current # of hitpoints
    long                        atk;        // basic strength of attack
    long                        refprob;    // probability for reflex save
    struct d20dice              hitdice;    // hit die set
    struct d20dice              defdice;    // defense die set
#if 0
    long                        ndef;       // number of defdies for defense
    long                        defdie;     // die to roll for defense
    long                        nhit;       // number of hitdies for attack
    long                        hitdie;     // die to roll for attack
#endif
};

#endif /* __MJOLNIR_ICE_ICE_H__ */

