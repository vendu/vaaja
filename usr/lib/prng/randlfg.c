#include <stdlib.h>
#include <stdint.h>
#include <zero/randlfg.h>
//#include <zero/randmt32.h>

/* REFERENCE: http://arxiv.org/abs/cond-mat/9710104 */

#define RAND_A 471
#define RAND_B 1586
#define RAND_C 6988
#define RAND_D 9689
#define RAND_N 16384    // power of two
#define RAND_M (RAND_N - 1)

static volatile int  randinit;
static          int *randbuf;
static          long randndx;

int
randlfg(void)
{
    int retval;
    int val1;
    int val2;
    int val3;
    int val4;
    int i;

    if (!randinit) {
        randbuf = malloc(RAND_N * sizeof(int));
        if (randbuf) {
            srand(1);
            for (i = 0 ; i < RAND_N ; i++) {
                randbuf[i] = rand();
            }
            randinit = 1;
        }
    }
    randndx++;
    val1 = randbuf[(randndx - RAND_A) & RAND_M];
    val2 = randbuf[(randndx - RAND_B) & RAND_M];
    val3 = randbuf[(randndx - RAND_C) & RAND_M];
    val4 = randbuf[(randndx - RAND_D) & RAND_M];
    retval = val1 ^ val2 ^ val3 ^ val4;
    randbuf[randndx & RAND_M] = retval;

    return retval;
}

