#ifndef __MJOLNIR_D20_H__
#define __MJOLNIR_D20_H__

void                            d20init(void);

#define D20_RAND_SHIFT          16
#define D20_RAND_MAX            (0xffffffff >> D20_RAND_SHIFT)

/* standard D20 dice set */
#define D20_DIE4                4
#define D20_DIE6                6
#define D20_DIE8                8
#define D20_DIE10               10
#define D20_DIE12               12
#define D20_DIE20               20

#include <mjolnir/conf.h>
#include <time.h>
#include <zero/cdefs.h>
#include <prng/randmt32.h>
#include <mjolnir/util.h>

#define d20rolldie(die)         (1 + (getrand() % (die + 1)))

static C_INLINE long
d20rolln(long n, long die)
{
    long                        score = 0;

    while (n--) {
        score += d20rolldie(die);
    }

    return score;
}

/* roll buffer for d20 dice set */
struct d20dice {
    long                        nd4;    /* count of 4-side dice */
    long                        nd6;    /* count of 6-side dice */
    long                        nd8;    /* count of 8-side dice */
    long                        nd10;   /* count of 10-side dice */
    long                        nd12;   /* count of 12-side dice */
    long                        nd20;   /* count of 20-side dice */
};

#endif /* __MJOLNIR_D20_H__ */

