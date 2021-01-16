#if (TEST_DIV)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <malt/div.h>
#include <malt/irp.h>

int32_t
div32(int32_t num, int32_t den)
{
    double  dval = den;
    double  guess = irpguess0(dval);

    if (!den) {
        raise(SIGFPE);
    }
    if (den > num) {

        return 0;
    } else if (den == num) {

        return 1;
    } else if (!num) {

        return 0;
    }
    guess = nrpiter2(guess, dval);
    guess = nrpiter2(guess, dval);
    guess = nrpiter2(guess, dval);
    guess = nrpiter2(guess, dval);
    num *= guess;

    return num;
}

/* FIXME: divirp32() doesn't work yet */
int
main(int argc, char *argv[])
{
    //    int32_t res = divirp32(55, 27);
    int32_t res;
    int     i;
    int     j;

    for (i = 0 ; i <= 0xffff ; i++) {
        fprintf(stderr, "I: %x\n", i);
        for (j = 1 ; j <= 0xffff ; j++) {
            res = div32(i, j);
            if (res != i / j) {
                fprintf(stderr, "%d/%d != %d", i, j, res);
                abort();
            }
        }
    }

    exit(0);
}

#endif /* TEST_DIV */

