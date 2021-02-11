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
            krak->prob = 20;
            
            break;
        case CHR_CRACKER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 64;
            krak->nhp = 64;
            krak->prob = 20;

            break;
        case CHR_SOLDIER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 32;
            krak->nhp = 32;
            krak->prob = 20;

            break;
        case CHR_CYBORG_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 64;
            krak->nhp = 64;
            krak->prob = 20;

            break;
        case CHR_THIEF_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->basehp = 16;
            krak->maxhp = 16;
            krak->prob = 20;

            break;
        case CHR_ENGINEER_CATEGORY:
            krak->exp = 0;
            krak->lvl = 1;
            krak->maxhp = 96;
            krak->nhp = 96;
            krak->prob = 20;

            break;
        default:
            fprintf(stderr, "invalid character category %ld\n", chr->cat);

            break;
    }
}

/*
 * HIT = 2 * level + 1d20
 * LEVEL = EXP / 32
 * PROB = 20 + LEVEL / 5
 */
static long
krakhit(struct objchr *src,
        struct objchr *dest)
{
    struct krak                *krak = &src->krak;
    long                        hit = 0;
    long                        exp = src->exp;
    long                        lvl = src->level;
    long                        prob = d20rolldie(D20_D20);
    long                        nhp = dest->nhp;
    long                        maxhp;

    if (prob > dest->krak.prob) {
        /* protection failed */
        if (ice <= krak->prob) {
            /* hit succeeds */
            hit = 2 * lvl + d20rolldie(D20_D20);
            if (lvl < KRAK_MAX_LEVEL - 1) {
                exp++;
                krak->exp = exp;
                if (!(exp & 0x1f)) {
                    lvl = exp >> 5;
                    nhp = krak->nhp;
                    maxhp = krak->maxhp;
                    krak->level = lvl;
                    nhp += lvl;
                    maxhp += lvl;
                    krak->prob = 20 + level / 5;
                    krak->nhp = nhp;
                    krak->maxhp = maxhp;
                }
            }
            printf("%s hits with %ld damage\n", src->name, hit);
            nhp = dest->nhp;
            nhp -= hit;
            dest->nhp = nhp;
        } else {
            printf("%s misses\n", src->name);
        }
    }

    return nhp;
}

static long
krakatk(struct objchr *src,
        struct objchr *dest)
{
    long                        nhp;

    nhp = krakhit(src, dest);
    if (nhp <= 0) {
        printf("%s wins combat\n", src->name);
    }

    return nhp;
}

/*
 * run KRAK-combat turn
 * - returns 1 if atk wins, 2 if def wins, 0 otherwise
 */
static long
krakrunturn(struct objchr *atk, struct objchr *def)
{
    long                        ret = 0;
    long                        nhp;

    nhp = krakhit(atk, def);
    if (nhp <= 0) {
        ret = 1;
    }
    nhp = krakhit(def, atk);
    if (nhp <= 0) {
        ret = 2;
    }

    return ret;
}

#if defined(TEST_KRAK)

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

    combatinit(void);
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

