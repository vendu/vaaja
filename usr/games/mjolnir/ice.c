#include <mjolnir/mjolnir.h>

extern struct chrcatname        chrcatnames[CHR_MAX_CATEGORY + 1];

#if defined(TEST_ICE)
static struct objchr            icechrtab[2];
static struct ice              *icetab[2]
= {
    &icechrtab[0].ice,
    &icechrtab[1].ice
};
#endif

void
chrini

/* per-category ice skill initialization */
void
chrinitice(struct objchr *chr)
{
    long                        cat = chr->cat;
    struct ice                 *ice = &chr->ice;

    switch (cat) {
        case CHR_PROGRAMMER_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->maxhp = 128;
            ice->nhp = 128;

            break;
        case CHR_CRACKER_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->maxhp = 64;
            ice->nhp = 64;

            break;
        case CHR_SOLDIER_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->maxhp = 32;
            ice->nhp = 32;

            break;
        case CHR_CYBORG_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->maxhp = 64;
            ice->nhp = 64;

            break;
        case CHR_THIEF_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->basehp = 16;
            ice->maxhp = 16;

            break;
        case CHR_ENGINEER_CATEGORY:
            ice->exp = 0;
            ice->lvl = 1;
            ice->maxhp = 96;
            ice->nhp = 96;

            break;
        default:
            fprintf(stderr, "invalid character category %ld\n", chr->cat);

            break;
    }
}

#if 0
static long
icerollkarma(struct objchr *chr)
{
    long                        n = chr->nkarma;
    long                        die = chr->karmadie;
    long                        nlp = d20rollndie(n, die);

    return nlp;
}
#endif

static long
icerolldef(struct ice *ice)
{
    long                        def = d20rollset(&ice->defdice);

    return def;
}

static long
icerollhit(struct ice *ice)
{
    long                        hit = d20rollset(&ice->hitdice);

    return hit;
}

static long
icehit(struct objchr *atk,
       struct objchr *def)
{
    long                        lvl = atk->lvl;
    long                        xp = atk->exp;
    long                        save = icerolldie(D20_D10);
    long                        nhp;
    long                        maxhp;

    if (save > def->refprob) {
        /* reflex save failed */
        xp++;
        atk->ice.xp = xp;
        lvl = xp >> 2;
        if (lvl > atk->ice.lvl) {
            nhp = atk->ice.nhp;
            maxhp = atk->ice.maxhp;
            atk->ice.lvl = lvl;
            nhp += lvl;
            maxhp += lvl;
            atk->ice.nhp = nhp;
            atk->ice.maxhp = maxhp;
        }
    } else {
        hit = 0;
    }

    return hit;
}

static void
iceinit(void)
{
    d20init();

    return;
}

static long
icerollhit(struct objchr *atk,
           struct objchr *def)
{
    long                        atkhit;
    long                        hit;
    long                        nhp= def->nhp;

    atkhit = icerollhit(atk, def);
    hit = icehit(&def->ice, atkhit);
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

#if defined(TEST_ICE)

/*
 * run close combat
 * - returns 1 if chr1 wins, 2 if chr2
 */
static long
iceruncombat(struct objchr *chr1, struct objchr *chr2)
{
    struct ice                 *ice1 = &chr1->ice;
    struct ice                 *ice2 = &chr2->ice;
    long                        ret = 0;
    long                        nhp;

    while (chr1->nhp > 0 && chr2->nhp > 0) {
        nhp = icerollhit(chr1, chr2);
        if (nhp <= 0) {
            ret = 1;
        }
        nhp = icerollhit(&chr2, chr1);
        if (nhp <= 0) {
            ret = 2;
        }
    }

    return ret;
}

void
iceprintstats(const char *msg, struct ice *ice)
{
    printf("%s: lvl = %ld, xp = %ld, basehp = %ld, maxhp = %ld, nhp = %ld\n",
           msg,
           ice->lvl,
           ice->exp,
           ice->basehp,
           ice->maxhp,
           ice->nhp);

    return;
}

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    long                        winner;

    iceinit();
    chrinitice(&icechrtab[0], NULL, CHR_PROGRAMMER_CATEGORY);
    chrinitice(&icechrtab[1], NULL, CHR_CRACKER_CATEGORY);
    winner = icerun(&icechrtab[0], &icechrtab[1]);
    if (winner == 1) {
        fprintf(stderr, "player %ld (%s/%s) won player 2 (%s/%s)\n",
                winner,
                icechrtab[0].name,
                chrcatnames[icechrtab[0].cat].def,
                icechrtab[1].name,
                chrcatnames[icechrtab[1].cat].def);
    } else {
        fprintf(stderr, "player %ld (%s/%s) won player 1 (%s/%s)\n",
                winner,
                icechrtab[1].name,
                chrcatnames[icechrtab[1].cat].def,
                icechrtab[0].name,
                chrcatnames[icechrtab[0].cat].def);
    }
    iceprintstats("player #1", icetab[0]);
    iceprintstats("player #2", icetab[1]);

    exit(0);
}

#endif /* TEST_ICE */

