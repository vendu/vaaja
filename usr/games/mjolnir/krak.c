#include <mjolnir/mjolnir.h>

extern struct chrcatname        chrcatnames[CHR_MAX_CATEGORY + 1];

#if defined(TEST_KRAK)
static struct objchr            krakchrtab[2];
static struct krak              *kraktab[2]
= {
    &krakchrtab[0].krak,
    &krakchrtab[1].krak
};
#endif

void
chrini

/* per-category krak skill initialization */
void
chrinitkrak(struct objchr *chr)
{
    long                        cat = chr->cat;
    struct krak                 *krak = &chr->krak;

    switch (cat) {
        case CHR_PROGRAMMER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 128;
            krak->nhp = 128;

            break;
        case CHR_CRACKER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 64;
            krak->nhp = 64;

            break;
        case CHR_SOLDIER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 32;
            krak->nhp = 32;

            break;
        case CHR_CYBORG_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 64;
            krak->nhp = 64;

            break;
        case CHR_THIEF_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->basehp = 16;
            krak->maxhp = 16;

            break;
        case CHR_ENGINEER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 96;
            krak->nhp = 96;

            break;
        default:
            fprintf(stderr, "invalid character category %ld\n", chr->cat);

            break;
    }
}

#if 0
static long
krakrollkarma(struct objchr *chr)
{
    long                        n = chr->nkarma;
    long                        die = chr->karmadie;
    long                        nlp = d20rollndie(n, die);

    return nlp;
}
#endif

static long
krakrolldef(struct krak *krak)
{
    long                        def = d20rollset(&krak->defdkrak);

    return def;
}

static long
krakrollhit(struct krak *krak)
{
    long                        hit = d20rollset(&krak->hitdkrak);

    return hit;
}

static long
krakhit(struct objchr *atk,
       struct objchr *def)
{
    long                        lvl = atk->lvl;
    long                        xp = atk->exp;
    long                        save = krakrolldie(D20_D10);
    long                        nhp;
    long                        maxhp;

    if (save > def->refprob) {
        /* reflex save failed */
        xp++;
        atk->krak.xp = xp;
        lvl = xp >> 2;
        if (lvl > atk->krak.lvl) {
            nhp = atk->krak.nhp;
            maxhp = atk->krak.maxhp;
            atk->krak.lvl = lvl;
            nhp += lvl;
            maxhp += lvl;
            atk->krak.nhp = nhp;
            atk->krak.maxhp = maxhp;
        }
    } else {
        hit = 0;
    }

    return hit;
}

static void
krakinit(void)
{
    d20init();

    return;
}

static long
krakrollhit(struct objchr *atk,
           struct objchr *def)
{
    long                        atkhit;
    long                        hit;
    long                        nhp= def->nhp;

    atkhit = krakrollhit(atk, def);
    hit = krakhit(&def->krak, atkhit);
    if (hit) {
        printf("%s hits with %ld damage\n", atk->name, hit);
        nhp -= hit;
        if (nhp <= 0) {
            printf("%s wins combat\n");
        }
        def->nhp = nhp;
    } else {
        printf("%s misses\n", atk->name);
    }

    return nhp;
}

#if defined(TEST_KRAK)

/*
 * run close combat
 * - returns 1 if chr1 wins, 2 if chr2
 */
static long
krakruncombat(struct objchr *chr1, struct objchr *chr2)
{
    struct krak                 *krak1 = &chr1->krak;
    struct krak                 *krak2 = &chr2->krak;
    long                        ret = 0;
    long                        nhp;

    while (chr1->nhp > 0 && chr2->nhp > 0) {
        nhp = krakrollhit(chr1, chr2);
        if (nhp <= 0) {
            ret = 1;
        }
        nhp = krakrollhit(&chr2, chr1);
        if (nhp <= 0) {
            ret = 2;
        }
    }

    return ret;
}

void
krakprintstats(const char *msg, struct krak *krak)
{
    printf("%s: lvl = %ld, xp = %ld, basehp = %ld, maxhp = %ld, nhp = %ld\n",
           msg,
           krak->lvl,
           krak->exp,
           krak->basehp,
           krak->maxhp,
           krak->nhp);

    return;
}

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    long                        winner;

    krakinit();
    chrinitkrak(&krakchrtab[0], NULL, CHR_PROGRAMMER_CATEGORY);
    chrinitkrak(&krakchrtab[1], NULL, CHR_CRACKER_CATEGORY);
    winner = krakrun(&krakchrtab[0], &krakchrtab[1]);
    if (winner == 1) {
        fprintf(stderr, "player %ld (%s/%s) won player 2 (%s/%s)\n",
                winner,
                krakchrtab[0].name,
                chrcatnames[krakchrtab[0].cat].def,
                krakchrtab[1].name,
                chrcatnames[krakchrtab[1].cat].def);
    } else {
        fprintf(stderr, "player %ld (%s/%s) won player 1 (%s/%s)\n",
                winner,
                krakchrtab[1].name,
                chrcatnames[krakchrtab[1].cat].def,
                krakchrtab[0].name,
                chrcatnames[krakchrtab[0].cat].def);
    }
    krakprintstats("player #1", kraktab[0]);
    krakprintstats("player #2", kraktab[1]);

    exit(0);
}

#endif /* TEST_KRAK */

