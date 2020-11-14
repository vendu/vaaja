#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/cdefs.h>
#if defined(D20_RANDMT32)
#include <prng/randmt32.h>
#endif
#include <dice/d20.h>
#include <deice/deice.h>

struct deice {
    struct deiceprog           *progs;
    struct deicestat           *stats;
    long                        *hptab;
};

static struct deiceprog         deiceprogs[2];
static struct deicestat         deicestats[2];
static long                     deicehptab[DEICE_MAX_LEVEL];

struct deice                    deice
= {
    deiceprogs,
    deicestats,
    deicehptab
};

static long
deicehit(struct deice *deice,
         struct deiceprog *prog,
         struct deicestat *stat,
         long hp)
{
    long                        nd = d20rolln(prog->ndmin, prog->ndmax);
    long                        lvl = stat->lvl;
    long                        xp = stat->xp;
    long                        nhp = stat->nhp;
    long                        def = d20rolln(nd, D20_D4);

    nhp += def;
    nhp -= hp;
    stat->nhp = nhp;
    xp++;
    if (nhp <= 0) {

        return INT32_MIN;
    }
    stat->xp = xp;
    lvl = __builtin_ctzll(xp) + 1;
    if (lvl > stat->lvl) {
        stat->lvl = lvl;
        stat->maxhp = deice->hptab[lvl];
    }

    return nhp;
}

static long
deicerolldice(struct deiceprog *prog)
{
    long                        nd = d20rolln(prog->ndmin, prog->ndmax);
    long                        nhp = d20rolln(nd, D20_D4);

    return nhp;
}

static void
deiceinithptab(long *tab)
{
    long                        lvl;

    lvl = 0;
    while (lvl < DEICE_MAX_LEVEL) {
        tab[lvl] = 512 * (4 + lvl);
        lvl++;
    }

    return;
}

static void
deiceinit(struct deice *deice, struct deiceprog *prog, struct deicestat *stat)
{
    long                        pid = d20rolldie(D20_D20);
    long                        nd = prog->ndmax;
    long                        nhp = d20rolln(nd, D20_D20);

    if (!deice->hptab[0]) {
        deiceinithptab(deice->hptab);
        d20initrand();
    }
    switch (pid) {
        default:
        case 0:
        case 1:
            prog->ndmin = 16;
            prog->ndmax = 32;

            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
        case 9:
            prog->ndmin = 32;
            prog->ndmax = 64;

            break;
        case 11:
        case 12:
            prog->ndmin = 64;
            prog->ndmax = 128;

            break;
    };
    stat->lvl = 1;
    stat->nhp = nhp;

    return;
}

static long
deiceloop(struct deice *deice)
{
    struct deiceprog           *prog1 = &deice->progs[0];
    struct deiceprog           *prog2 = &deice->progs[1];
    struct deicestat           *stat1 = &deice->stats[0];
    struct deicestat           *stat2 = &deice->stats[1];
    long                        nturn;
    long                        hit1;
    long                        hit2;

    deiceinit(deice, prog1, stat1);
    deiceinit(deice, prog2, stat2);
    for (nturn = 0 ; nturn < DEICE_MAX_TURNS ; nturn++) {
        hit1 = deicerolldice(prog1);
        hit2 = deicerolldice(prog2);
        if (deicehit(deice, prog2, stat2, hit1) == INT32_MIN) {

            return 1;
        }
        if (deicehit(deice, prog1, stat1, hit2) == INT32_MIN) {

            return 2;
        }
    }
    if (stat1->nhp >= stat2->nhp) {

        return 1;
    }

    return 2;
}

void
deiceprintstat(const char *msg, struct deiceprog *prog, struct deicestat *stat)
{
    printf("%s: lvl = %ld, xp = %ld, nhp = %ld (ndmin = %ld, ndmax = %ld)\n",
           msg,
           stat->lvl,
           stat->xp,
           stat->nhp,
           prog->ndmin,
           prog->ndmax);

    return;
}

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    long                        winner;

    winner = deiceloop(&deice);
    fprintf(stderr, "player %ld won\n", winner);
    deiceprintstat("player #1", &deice.progs[0], &deice.stats[0]);
    deiceprintstat("player #2", &deice.progs[1], &deice.stats[1]);

    exit(0);
}

