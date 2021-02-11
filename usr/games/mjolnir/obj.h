#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#define MJOLNIR_OBJ_CHARACTER       1
#define MJOLNIR_OBJ_ITEM            2

/* character categories */
#define MJOLNIR_INVALID_CHR         0
#define MJOLNIR_PROGRAMMER_CHR      1
#define MJOLNIR_CRACKER_CHR         2
#define MJOLNIR_CYBORG_CHR          3
#define MJOLNIR_SOLDIER_CHR         4
#define MJOLNIR_THIEF_CHR           5
#define MJOLNIR_ENGINEER_CHR        6
#define MJOLNIR_CHR_CATEGORIES      6

/* negative value of [-16, -1] for evil alignment */
#define MJOLNIR_NEUTRAL_ALIGNMENT   0
/* positive value of [1, 16] for evil alignment */

#define MJOLNIR_HEAL_PARALYZED      5           // paralyzed/elecrocuted, frozen
#define MJOLNIR_HEAL_INFECTED       4           // ghoul-, zombie-, rat-bites...
#define MJOLNIR_HEAL_NORMAL         3           // normal condition
#define MJOLNIR_HEAL_FAST           2           // natural salves
#define MJOLNIR_HEAL_EXTRA_FAST     1           // fast-to-heal undead
#define MJOLNIR_HEAL_SUPER_FAST     0           // fastest-to-heal undead
#define MJOLINR_CHR_CHAOTIC_EVIL    (-2)        // 'entropy and destruction'
#define MJOLNIR_CHR_EVIL            (-1)        // 'tyranny and hatred'
#define MJOLNIR_CHR_UNALIGNED       0           // 'no alignment'
#define MJOLNIR_LAWFUL_GOOD         1           // 'civilization and order
#define MJOLNIR_CHR_GOOD            2           // 'freedom and kindness'
struct objchr {
    const char                     *name;       // character name
    const long                      cat;        // category, 'class' ID
    const long                      type;       // type/ASCII-presentation
    long                            flg;        // character status flags
    long                            aln;        // alighnment
    /* mental skills */
    long                            psi;        // psionic skills e.g. telepathy
    long                            intel;      // intelligence; magic success
    long                            magic;      // general magic
    long                            spell;      // spell-casting modifier
    /* physical abilities */
    long                            heal;       // heal rate [1/32,1]; 1 >> heal
    long                            str;        // strength
    long                            throw;      // throwing; grenade, cross etc.
    /* softwre skills */
    struct krak                     krak;       // krak subgame structure
    /* combat skills */
    struct budo                     budo;       // budo close-combat attributes
    /* weapon skills */
    long                            whip;       // whip skill
    long                            knife;      // knife skill
    long                            sword;      // sword skill
    long                            gun;        // gun skill
    long                            bomb;       // explosive skill
    /* miscellaneous modifiers */
    long                            lock;       // lock-picking
    long                            karma;
    long                            cha;        // charisma; communication skill
    long                            con;        // frauds and other hoaxes
};

#if 0
struct chrcatname {
    char                           *def;
    char                           *name;
};
#endif

struct objitem {
    char                           *name;       // object name (inventory etc.)
    long                            type;       // type/ASCII-presentation
    long                            cat;        // category/subtype
    long                            flg;        // object status flags
    long                            str;        // strength for attack (weapon)
    long                            def;        // non-zero e.g. for armors
    struct d20diceset               atkdice;
    struct d20diceset               defdice;
};

struct objdeck {
    long                            netplugs;   // number of connections
    long                            netspeed;   // 512000, 1000000, or 4000000
    long                           *bram;       // TLB, NVRAM, CACHE, MTR
    long                            bramsize;
    int8_t                         *flash;      // operating system and programs
    long                            flashsize;
    int8_t                         *ram;        // physical memory; code + data
    long                            ramsize;    // max 64 megs (16x4M), 32-bit
};

#endif /* __MJOLNIR_OBJ_H__ */

