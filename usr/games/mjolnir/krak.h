#ifndef __MJOLNIR_KRAK_H__
#define __MJOLNIR_KRAK_H__

void                            krakinitchr(struct objchr *chr,
                                           const char *name,
                                           long cat);

#define KRAK_MAX_TURNS           128
#define KRAK_MAX_LEVEL           32

struct krak {
    long                        exp;        // program experience points
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

#endif /* __MJOLNIR_KRAK_H__ */

