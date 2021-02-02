#ifndef __MJOLNIR_KRAK_H__
#define __MJOLNIR_KRAK_H__

void                            krakinitchr(struct objchr *chr,
                                           const char *name,
                                           long cat);

#define KRAK_MAX_TURNS           128
#define KRAK_MAX_LEVEL           32

struct krak {
    long                        exp;        // KRAK experience points
    long                        lvl;        // KRAK level
    long                        maxhp;      // KRAK max # of hitpoints
    long                        nhp;        // KRAK current # of hitpoints
    long                        atk;        // basic power of attack
    struct d20dice              hitdice;    // hit die set
    struct d20dice              defdice;    // defense die set
};

#endif /* __MJOLNIR_KRAK_H__ */

