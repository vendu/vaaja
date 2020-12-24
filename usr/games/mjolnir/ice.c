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
iceinitchr(struct objchr *chr)
{
    long                        cat = chr->cat;
    struct ice                 *ice = &chr->ice;

    switch (cat) {
        case CHR_PROGRAMMER_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 128;
            ice->maxhp = 128;
            ice->nhp = 128;
            ice->ndef = 1;
            ice->defdie = 12;
            ice->nhit = 1;
            ice->hitdie = 12;

            break;
        case CHR_CRACKER_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 64;
            ice->maxhp = 64;
            ice->nhp = 64;
            ice->ndef = 1;
            ice->defdie = 10;
            ice->nhit = 1;
            ice->hitdie = 10;

            break;
        case CHR_SOLDIER_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 32;
            ice->maxhp = 32;
            ice->nhp = 32;
            ice->ndef = 1;
            ice->defdie = 6;
            ice->nhit = 1;
            ice->hitdie = 10;

            break;
        case CHR_CYBORG_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 64;
            ice->maxhp = 64;
            ice->nhp = 64;
            ice->ndef = 1;
            ice->defdie = 20;
            ice->nhit = 1;
            ice->hitdie = 12;

            break;
        case CHR_THIEF_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 16;
            ice->maxhp = 16;
            ice->nhp = 16;
            ice->ndef = 1;
            ice->defdie = 6;
            ice->nhit = 1;
            ice->hitdie = 4;

            break;
        case CHR_ENGINEER_CATEGORY:
            chr->nkarma = 1;
            chr->karmadie = 6;
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 96;
            ice->maxhp = 96;
            ice->nhp = 96;
            ice->ndef = 1;
            ice->defdie = 10;
            ice->ndef = 1;
            ice->hitdie = 12;

            break;
        default:
            fprintf(stderr, "invalid character category %ld\n", chr->cat);

            break;
    }
}

static long
icerollkarma(struct objchr *chr)
{
    long                        n = chr->nkarma;
    long                        die = chr->karmadie;
    long                        nlp = d20rollndie(n, die);

    return nlp;
}

static long
icerolldef(struct ice *ice)
{
    long                        n = ice->ndef;
    long                        die = ice->defdie;
    long                        nhp = d20rollndie(n, die);

    return nhp;
}

static long
icerollhit(struct ice *ice)
{
    long                        n = ice->nhit;
    long                        die = ice->hitdie;
    long                        nhp = d20rollndie(n, die);

    return nhp;
}

static long
icehit(struct ice *ice,
       long hit)
{
    long                        lvl = ice->lvl;
    long                        xp = ice->xp;
    long                        nhp = ice->nhp;
    long                        def = icerolldef(ice);
    long                        maxhp;
    long                        hp;
    long                        nd;

    if (hit > def) {
        nhp += def;
        nhp -= hit;
        ice->nhp = nhp;
    }
    xp++;
    if (nhp <= 0) {

        return INT32_MIN;
    }
    ice->xp = xp;
    lvl = xp >> 4;
    if (lvl > ice->lvl) {
        maxhp = ice->maxhp;
        hp = ice->basehp;
        nd = lvl;
        ice->lvl = lvl;
        maxhp += hp;
        nhp += hp;
        ice->ndef += nd;
        ice->nhit += nd;
        ice->basehp = hp;
        ice->maxhp = maxhp;
    }

    return nhp;
}

static void
iceinit(void)
{
    d20init();

    return;
}

static long
icerun(struct objchr *chr1, struct objchr *chr2)
{
    long                        nturn;
    long                        hit1;
    long                        hit2;
    struct ice                 *ice1 = &chr1->ice;
    struct ice                 *ice2 = &chr2->ice;

    for (nturn = 0 ; nturn < ICE_MAX_TURNS ; nturn++) {
        hit1 = icerollhit(ice1);
        hit1 -= min2(icerollkarma(chr2), hit1 >> 2);
        hit2 = icerollhit(ice2);
        hit2 -= min2(icerollkarma(chr1), hit2 >> 2);
        if (icehit(ice2, hit1) == INT32_MIN) {

            return 1;
        }
        if (icehit(ice1, hit2) == INT32_MIN) {

            return 2;
        }
    }
    if (ice1->nhp >= ice2->nhp) {

        return 1;
    }

    return 2;
}

void
iceprintstats(const char *msg, struct ice *ice)
{
    printf("%s: lvl = %ld, xp = %ld, basehp = %ld, maxhp = %ld, nhp = %ld\n",
           msg,
           ice->lvl,
           ice->xp,
           ice->basehp,
           ice->maxhp,
           ice->nhp);

    return;
}

#if defined(TEST_ICE)
int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    long                        winner;

    iceinit();
    iceinitchr(&icechrtab[0], "john", CHR_PROGRAMMER_CATEGORY);
    iceinitchr(&icechrtab[1], "doe", CHR_CRACKER_CATEGORY);
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

