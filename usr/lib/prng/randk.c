#include <stdlib.h>
#include <zero/trix.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/randk.h>

struct _randkglobal _randkglobal ALIGNED(CLSIZE);
long                _randktbl[RANDKNTAB];

/*
 * randk() is based on Donald E Knuth's algorithm as represented in the book
 * Numerical Recipes in C. randk() is a bit over 2 times as fast as rand() on
 * my system.
 */
long
randk(void)
{
    long *tblptr;
    long  mj;
    long  mk;
    int   inext;
    int   inextp;

    if (!_randkglobal.initialized) {
        srandk(0);
    }
    tblptr = _randktbl;
    inext = _randkglobal.inext;
    inextp = _randkglobal.inextp;
    if (++inext == RANDKNTAB) {
        inext = 1;
    }
    if (++inextp == RANDKNTAB) {
        inextp = 1;
    }
    mj = tblptr[inext];
    mk = tblptr[inextp];
    mj -= mk;
    if (mj < RANDKMZ) {
        mj += RANDKMBIG;
    }
    tblptr[inext] = mj;
    _randkglobal.inext = inext;
    _randkglobal.inextp = inextp;

    return mj;
}

void
srandk(long seed)
{
    long *tblptr;
    long mj;
    long mk;
    long mt;
    long seedval;
    int  i;
    int  ii;
    int  j;
    int  k;

    tblptr = _randktbl;
    seedval = seed;
    if (!seedval) {
        seedval = 1;
    }
    mj = labs(RANDKMSEED - seedval);
    mj %= RANDKMBIG;
    tblptr[RANDKNTAB - 1] = mj;
    mk = 1;
    j = 21;
    for (i = 1 ; i < RANDKNTAB - 1 ; i++) {
        ii = j % RANDKNTAB - 1;
        tblptr[ii] = mk;
        mt = mk;
        mk = mj - mk;
        if (mk < RANDKMZ) {
            mk += RANDKMBIG;
        }
        mj = mt;
        j += 21;
    }
    for (k = 1 ; k < 5 ; k++) {
        mt = 32;
        for (i = 1 ; i < 56 ; i++) {
            mj = tblptr[i];
            mk = tblptr[mt % RANDKNTAB - 1];
            mj -= mk;
            if (mj < RANDKMZ) {
                mj += RANDKMBIG;
            }
            tblptr[i] = mj;
            mt++;
        }
    }
    _randkglobal.inext = 0;
    _randkglobal.inextp = 31;
    _randkglobal.initialized = 1;

    return;
}

#if 0
#if (TEST)
#include <stdio.h>
#include <stdlib.h>
#include <zero/prof.h>

int
main(int argc, char *argv[])
{
    int i;
    int val;
    PROFCLK(clk);

    sleep(5);

    srand(time(NULL));
    profstartclk(clk);
    for (i = 0 ; i < 1048576 ; i++) {
        val = rand();
    }
    profstopclk(clk);
    fprintf(stderr, "GNU: %lu\n", profclkdiff(clk));

    srandk(time(NULL));
    profstartclk(clk);
    for (i = 0 ; i < 1048576 ; i++) {
        val = randk();
    }
    profstopclk(clk);
    fprintf(stderr, "ZERO: %lu\n", profclkdiff(clk));

    exit(0);
}
#endif
#endif /* 0 */

