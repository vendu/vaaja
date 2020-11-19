#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mjolnir/util.h>
#include <mjolnir/obj.h>
#include <mjolnir/d20.h>
#include <mjolnir/ice.h>

extern const char              *chrcatnames[CHR_MAX_CATEGORY + 1];

#if defined(TEST_ICE)
static struct objchr            icechrs[2];
static struct ice              *ices[2]
= {
    &icechrs[0].ice,
    &icechrs[1].ice
};
#endif

static long
icerolldef(struct ice *ice)
{
    long                        nd = ice->ndefdice;
    long                        die = ice->defdie;
    long                        nhp = d20rolln(nd, die);

    return nhp;
}

static long
icerollhit(struct ice *ice)
{
    long                        nd = ice->nhitdice;
    long                        die = ice->hitdie;
    long                        nhp = d20rolln(nd, die);

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
    lvl = xp >> 5;
    if (lvl > ice->lvl) {
        maxhp = ice->maxhp;
        hp = ice->basehp;
        nd = lvl;
        ice->lvl = lvl;
        maxhp += hp;
        nhp += hp;
        ice->ndefdice += nd;
        ice->nhitdice += nd;
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
icerun(struct ice *ice1, struct ice *ice2)
{
    long                        nturn;
    long                        hit1;
    long                        hit2;

    for (nturn = 0 ; nturn < ICE_MAX_TURNS ; nturn++) {
        hit1 = icerollhit(ice1);
        hit2 = icerollhit(ice2);
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
    chrinit(&icechrs[0], "john", CHR_PLAYER, CHR_PROGRAMMER_CATEGORY);
    chrinit(&icechrs[1], "doe", CHR_PLAYER, CHR_CRACKER_CATEGORY);
    winner = icerun(ices[0], ices[1]);
    fprintf(stderr, "player %ld won\n", winner);
    iceprintstats("player #1", ices[0]);
    iceprintstats("player #2", ices[1]);

    exit(0);
}
#endif /* TEST_ICE */

