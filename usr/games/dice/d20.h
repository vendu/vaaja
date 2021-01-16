#ifndef __GAMES_DICE_D20_H__
#define __GAMES_DICE_D20_H__

#define D20_RANDMT32
#define D20_RAND_SHIFT          16
#define D20_RAND_MAX            (0xffffffff >> D20_RAND_SHIFT)

/* standard D20 dice set */
#define D20_D4                  4
#define D20_D6                  6
#define D20_D8                  8
#define D20_D10                 10
#define D20_D12                 12
#define D20_D20                 20

#if defined(D20_RANDMT32)
#include <prng/randmt32.h>
#endif
#define d20srand(x)             srandmt32(x)
#define d20rand()               (randmt32() >> D20_RAND_SHIFT)
#else
#define d20srand(x)             srand(x)
#define d20rand()               (rand() >> D20_RAND_SHIFT)
#endif
#include <time.h>
#define d20initrand(max)        d20srand(time(NULL))
#define d20rolldie(max)         (1 + (d20rand() % (max)))
#include <zero/cdefs.h>

static C_INLINE long
d20rolln(long n, long die)
{
    long                        score = 0;

    while (n--) {
        score += d20rolldie(die);
    }
    fprintf(stderr, "RET: %ld\n", score);

    return score;
}

static C_INLINE long
d20rollset(struct d20dice *dice)
{
    long                        score = dice->str;
    long                        prob;
    long                        n;

    prob = d20rolldie(D20_D10);
    if (prob <= dice->prob) {
        n = dice->nd4;
        if (n) {
            score += d20rolln(n, D20_D4);
        }
        n = dice->nd6;
        if (n) {
            score += d20rolln(n, D20_D6);
        }
        n = dice->nd8;
        if (n) {
            score += d20rolln(n, D20_D8);
        }
        n = dice->nd10;
        if (n) {
            score += d20rolln(n, D20_D10);
        }
        n = dice->nd12;
        if (n) {
            score += d20rolln(n, D20_D12);
        }
        n = dice->nd20;
        if (n) {
            score += d20rolln(n, D20_D20);
        }
    }

    return score;
}

/* roll buffer for d20 dice set */
struct d20diceset {
    long                        nd4;    /* count of 4-side dice */
    long                        nd6;    /* count of 6-side dice */
    long                        nd8;    /* count of 8-side dice */
    long                        nd10;   /* count of 10-side dice */
    long                        nd12;   /* count of 12-side dice */
    long                        nd20;   /* count of 20-side dice */
};

#endif /* __GAMES_DICE_D20_H__ */

