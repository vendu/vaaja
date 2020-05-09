/*
 * 32-bit implementation of the Mersenne Twister MT19937 algorithm
 */

#if !defined(RANDMT32TEST)
#define RANDMT32TEST 0
#endif

/*
 * REFERENCE: https://en.wikipedia.org/wiki/Mersenne_twister
 */

/*
 * API
 * ---
 * void srandmt32(unsigned long seed);
 * - seed the pseudo-random number generator with the argument
 * unsigned long randmt32(void);
 * - get the next PRN in series
 *
 * void srandmt32_r(unsigned long seed);
 * unsigned long randmt32_r(void);
 * - re-entrant versions [for multithreaded processes]
 */

/* Thanks to Craig Robbins for pointing out errors and other help. :) */

/*
 * optimisations
 * -------------
 * - elimination of modulus calculations and in-loop branches by unrolling loops
 */

#include <stdint.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>
#if (RANDMT32TEST)
#include <stdio.h>
#if (RANDMT32PROF)
#include <zero/prof.h>
#endif
#endif

#define RANDMT32NSTATE      624                 // # of buffer values
#define RANDMT32MAGIC       397
/* magic numbers */
#define RANDMT32MATRIX      0x9908b0dfUL
#define RANDMT32HIMASK      0x80000000UL        // highest bit
#define RANDMT32LOMASK      0x7fffffffUL        // low 31 bits
#define RANDMT32DEFSEED     5489UL
#define RANDMT32TABSEED     19650218UL
#define RANDMT32MULTIPLIER1 1812433253UL
#define RANDMT32MULTIPLIER2 1664525UL
#define RANDMT32MULTIPLIER3 1566083941UL
/* shift counts */
#define RANDMT32SHIFT       30
#define RANDMT32SHIFT1      11
#define RANDMT32SHIFT2      7
#define RANDMT32SHIFT3      15
#define RANDMT32SHIFT4      18
/* bitmasks */
#define RANDMT32MASK1       0x9d2c5680UL
#define RANDMT32MASK2       0xefc60000UL

static unsigned long randmt32state[RANDMT32NSTATE] ALIGNED(PAGESIZE);
static unsigned long randmt32magic[2] = { 0UL, RANDMT32MATRIX };
static unsigned long randmt32curndx = RANDMT32NSTATE + 1;
#if (RANDMT32TEST)
static unsigned long randmt32key[4]
= {
    0x123UL,
    0x234UL,
    0x345UL,
    0x456UL
};
#endif

void
srandmt32(unsigned long seed)
{
    unsigned long val;
    unsigned long tmp;
    unsigned long ndx;

    tmp = seed & 0xffffffffUL;
    randmt32state[0] = tmp;
    for (ndx = 1 ; ndx < RANDMT32NSTATE ; ndx++) {
        val = RANDMT32MULTIPLIER1 * (tmp ^ (tmp >> RANDMT32SHIFT)) +  ndx;
        val &= 0xffffffffUL;
        randmt32state[ndx] = val;
        tmp = val;
    }
    randmt32curndx = ndx;

    return;
}

void
srandmt32tab(unsigned long *key, unsigned long keylen)
{
    unsigned long l = 1;
    unsigned long m = 0;
    unsigned long ndx;
    unsigned long tmp;
    unsigned long val;

    srandmt32(RANDMT32TABSEED);
    ndx = max(keylen, RANDMT32NSTATE);
    tmp = randmt32state[0];
    for ( ; (ndx) ; ndx--) {
        val = randmt32state[l];
        val ^= RANDMT32MULTIPLIER2 * (tmp ^ (tmp >> RANDMT32SHIFT));
        val += key[m] + m;
        val &= 0xffffffffUL;
        randmt32state[l] = val;
        m++;
        l++;
        if (m >= keylen) {
            m = 0;
        }
        if (l >= RANDMT32NSTATE) {
            val = randmt32state[RANDMT32NSTATE - 1];
            l = 1;
            randmt32state[0] = val;
        }
        tmp = val;
    }
    tmp = randmt32state[l - 1];
    for (ndx = RANDMT32NSTATE - 1 ; (ndx) ; ndx--) {
        val = randmt32state[l];
        val ^= RANDMT32MULTIPLIER3 * (tmp ^ (tmp >> RANDMT32SHIFT));
        val -= l;
        val &= 0xffffffffUL;
        randmt32state[l] = val;
        l++;
        if (l >= RANDMT32NSTATE) {
            val = randmt32state[RANDMT32NSTATE - 1];
            l = 1;
            randmt32state[0] = val;
        }
        tmp = val;
    }
    randmt32state[0] = RANDMT32HIMASK;

    return;
}

void
_randmt32buf(void)
{
    unsigned long val;
    unsigned long tmp1;
    unsigned long tmp2;
    unsigned long tmp3;
    long          l;

    for (l = 0 ; l < RANDMT32NSTATE - RANDMT32MAGIC ; l++) {
        tmp1 = randmt32state[l] & RANDMT32HIMASK;
        tmp2 = randmt32state[l + 1] & RANDMT32LOMASK;
        tmp3 = randmt32state[l + RANDMT32MAGIC];
        val = tmp1 | tmp2;
        tmp1 = randmt32magic[val & 0x01UL];
        randmt32state[l] = tmp3 ^ (val >> 1) ^ tmp1;
    }
    for ( ; l < RANDMT32NSTATE - 1 ; l++) {
        tmp1 = randmt32state[l] & RANDMT32HIMASK;
        tmp2 = randmt32state[l + 1] & RANDMT32LOMASK;
        tmp3 = randmt32state[l + RANDMT32MAGIC - RANDMT32NSTATE];
        val = tmp1 | tmp2;
        tmp1 = randmt32magic[val & 0x01];
        randmt32state[l] = tmp3 ^ (val >> 1) ^ tmp1;
    }
    tmp1 = randmt32state[RANDMT32NSTATE - 1] & RANDMT32HIMASK;
    tmp2 = randmt32state[0] & RANDMT32LOMASK;
    tmp3 = randmt32state[RANDMT32MAGIC - 1];
    val = tmp1 | tmp2;
    randmt32state[RANDMT32NSTATE - 1] = tmp3 ^ (val >> 1)
        ^ randmt32magic[val & 0x01];
    randmt32curndx = 0;

    return;
}

unsigned long
randmt32(void)
{
    unsigned long x;

    if (randmt32curndx >= RANDMT32NSTATE) {
        if (randmt32curndx == RANDMT32NSTATE + 1) {
            srandmt32(RANDMT32DEFSEED);
        }
        _randmt32buf();
    }
    x = randmt32state[randmt32curndx];
    x ^= x >> RANDMT32SHIFT1;
    x ^= (x << RANDMT32SHIFT2) & RANDMT32MASK1;
    x ^= (x << RANDMT32SHIFT3) & RANDMT32MASK2;
    x ^= x >> RANDMT32SHIFT4;
    randmt32curndx++;

    return x;
}

#if (RANDMT32TEST)
int
main(void)
{
    int i;
#if (RANDMT32PROF)
    PROFDECLCLK(clk);
    unsigned long *buf;
#endif

    srandmt32tab(randmt32key, sizeof(randmt32key) / sizeof(unsigned long));
    for (i = 0; i < 65536; i++) {
        printf("%lx\n", randmt32());
    }

    return 0;
}
#endif

