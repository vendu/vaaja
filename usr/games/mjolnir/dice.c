/* Copyright (C) Tuomo Petteri Venäläinen 2011 */

/* compile with something like:
 * gcc -O -o dice dice.c
 * - vendu
 */

#include <dungeon/conf.h>
#if !defined(TEST_DICE)
#define TEST_DICE 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if (DNG_RANDMT32)
#include <prng/randmt32.h>
#endif
#include "dice.h"

void
diceparsecmd(struct diced20 *buf, int argc, char *argv[])
{
    long l;

    for (l = 1 ; l < argc ; l++) {
        if (!strcmp(argv[l], "-d4")) {
            buf->nd4 = atol(argv[++l]);
            fprintf(stderr, "%ldxd4\n", buf->nd4);
        } else if  (!strcmp(argv[l], "-d6")) {
            buf->nd6 = atol(argv[++l]);
            fprintf(stderr, "%ldxd6\n", buf->nd6);
        } else if  (!strcmp(argv[l], "-d8")) {
            buf->nd8 = atol(argv[++l]);
            fprintf(stderr, "%ldxd8\n", buf->nd8);
        } else if  (!strcmp(argv[l], "-d10")) {
            buf->nd10 = atol(argv[++l]);
            fprintf(stderr, "%ldxd10\n", buf->nd10);
        } else if  (!strcmp(argv[l], "-d12")) {
            buf->nd12 = atol(argv[++l]);
            fprintf(stderr, "%ldxd12\n", buf->nd12);
        } else if  (!strcmp(argv[l], "-d20")) {
            buf->nd20 = atol(argv[++l]);
            fprintf(stderr, "%ldxd20\n", buf->nd20);
        }
    }

    return;
}

/*
 * initialise random number generator with current time
 */
void
diceinit(unsigned long seed)
{
    if (!seed) {
        seed = 0xffffffff;
    }
#if (DNG_RANDMT32)
    srandmt32(seed);
#else
    srand(seed);
#endif

    return;
}

unsigned long
dicerolld20(struct diced20 *dicebuf)
{
    unsigned long retval = 0;
    unsigned long n;

    n = dicebuf->nd4;
    if (n) {
        retval += dicerolln(n, DICE_1D4);
    }
    n = dicebuf->nd6;
    if (n) {
        retval += dicerolln(n, DICE_1D6);
    }
    n = dicebuf->nd8;
    if (n) {
        retval += dicerolln(n, DICE_1D8);
    }
    n = dicebuf->nd10;
    if (n) {
        retval += dicerolln(n, DICE_1D10);
    }
    n = dicebuf->nd12;
    if (n) {
        retval += dicerolln(n, DICE_1D12);
    }
    n = dicebuf->nd20;
    if (n) {
        retval += dicerolln(n, DICE_1D20);
    }

    return retval;
}

#if (TEST)
int
main(int argc, char *argv[])
{
    struct diced20 dicebuf = { 1, 1, 1, 1, 1, 1 };
    int            retval = 0;

    diceinit(0);
    srandmt32(time(NULL));
    diceparsecmd(&dicebuf, argc, argv);
    retval = dicerolld20(&dicebuf);
    fprintf(stderr, "DICE: %lu\n", retval);

    fprintf(stderr, "%dd%d : %d\n",
            8, 6, dicerolln(8, 6));
    fprintf(stderr, "%dd%d : %d\n",
            4, 32, dicerolln(4, 32));
    fprintf(stderr, "%dd%d : %d\n",
            8, 64, dicerolln(8, 64));

    fprintf(stderr, "1d4: %d\n", diceroll(DICE_1D4));
    fprintf(stderr, "1d6: %d\n", diceroll(DICE_1D6));
    fprintf(stderr, "1d8: %d\n", diceroll(DICE_1D8));
    fprintf(stderr, "1d10: %d\n", diceroll(DICE_1D10));
    fprintf(stderr, "1d12: %d\n", diceroll(DICE_1D12));
    fprintf(stderr, "1d20: %d\n", diceroll(DICE_1D20));

    exit(retval);
}
#endif

