#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/cdefs.h>
#if defined(D20_RANDMT32)
#include <prng/randmt32.h>
#endif
#include <dice/d20.h>
#include <deice/deice.h>

static const char              *chrclassnames[MJOLNIR_CHARACTER_CLASSES]
= {
    "programmer",
    "cracker",
    "cyborg",
    "thief",
    "engineer"
};
static struct deicestat         deicestats[2];

struct deice                    deice
= {
    deicestats
};

static void
deiceinitclass(struct deicestat *stat)
{
    long                        chrclass = stat->chrclass;

    switch(chrclass) {
        case MJOLNIR_PROGRAMMER_CLASS:
            stat->basehp = 32;
            stat->maxhp = 32;
            stat->nhp = 32;
            stat->defdie = 20;
            stat->hitdie = 4;

            break;
        case MJOLNIR_CRACKER_CLASS:
            stat->basehp = 32;
            stat->maxhp = 32;
            stat->nhp = 32;
            stat->defdie = 12;
            stat->hitdie = 8;

            break;
        case MJOLNIR_CYBORG_CLASS:
            stat->basehp = 32;
            stat->maxhp = 32;
            stat->nhp = 32;
            stat->defdie = 32;
            stat->hitdie = 12;

            break;
        case MJOLNIR_THIEF_CLASS:
            stat->basehp = 32;
            stat->maxhp = 32;
            stat->nhp = 32;
            stat->defdie = 6;
            stat->hitdie = 6;

            break;
        case MJOLNIR_ENGINEER_CLASS:
            stat->basehp = 32;
            stat->maxhp = 32;
            stat->nhp = 32;
            stat->defdie = 10;
            stat->hitdie = 4;

            break;
        default:
            fprintf(stderr, "invalid character class %ld\n", chrclass);

            break;
    }
}

static long
deicerolldef(struct deicestat *stat)
{
    long                        nd = stat->ndefdice;
    long                        dval = stat->defdie;
    long                        nhp = d20rolln(nd, dval);

    return nhp;
}

static long
deicerollhit(struct deicestat *stat)
{
    long                        nd = stat->nhitdice;
    long                        dval = stat->hitdie;
    long                        nhp = d20rolln(nd, dval);

    return nhp;
}

static long
deicehit(struct deicestat *stat,
         long hit)
{
    long                        lvl = stat->lvl;
    long                        xp = stat->xp;
    long                        nhp = stat->nhp;
    long                        def = deicerolldef(stat);
    long                        maxhp;
    long                        hp;
    long                        nd;

    if (hit > def) {
        nhp += def;
        nhp -= hit;
        stat->nhp = nhp;
    }
    xp++;
    if (nhp <= 0) {

        return INT32_MIN;
    }
    stat->xp = xp;
    lvl = __builtin_ctzl(xp) + 1;
    if (lvl > stat->lvl) {
        maxhp = stat->maxhp;
        hp = stat->basehp;
        nd = lvl;
        stat->lvl = lvl;
        maxhp += hp;
        nhp += hp;
        stat->ndefdice += nd;
        stat->nhitdice += nd;
        stat->basehp = hp;
        stat->maxhp = maxhp;
    }

    return nhp;
}

static void
deiceinitstat(struct deicestat *stat)
{
    stat->chrclass = d20rolln(1, MJOLNIR_CHARACTER_CLASSES);
    if (!stat->xp) {
        d20initrand();
        deiceinitclass(stat);
        stat->xp = 0;
        stat->lvl = 1;
    }

    return;
}

static long
deiceloop(struct deice *deice)
{
    struct deicestat           *stat1 = &deice->stats[0];
    struct deicestat           *stat2 = &deice->stats[1];
    long                        nturn;
    long                        hit1;
    long                        hit2;

    deiceinitstat(stat1);
    deiceinitstat(stat2);
    for (nturn = 0 ; nturn < DEICE_MAX_TURNS ; nturn++) {
        hit1 = deicerollhit(stat1);
        hit2 = deicerollhit(stat2);
        if (deicehit(stat2, hit1) == INT32_MIN) {

            return 1;
        }
        if (deicehit(stat1, hit2) == INT32_MIN) {

            return 2;
        }
    }
    if (stat1->nhp >= stat2->nhp) {

        return 1;
    }

    return 2;
}

void
deiceprintstat(const char *msg, struct deicestat *stat)
{
    printf("%s: class = %s, lvl = %ld, xp = %ld, basehp = %ld, maxhp = %ld, nhp = %ld\n",
           msg,
           chrclassnames[stat->chrclass],
           stat->lvl,
           stat->xp,
           stat->basehp,
           stat->maxhp,
           stat->nhp);

    return;
}

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    long                        winner;

    winner = deiceloop(&deice);
    fprintf(stderr, "player %ld won\n", winner);
    deiceprintstat("player #1", &deice.stats[0]);
    deiceprintstat("player #2", &deice.stats[1]);

    exit(0);
}

