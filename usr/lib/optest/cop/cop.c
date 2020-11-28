#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/*
 * NOTES: if anyone can find a good way to deduce loop overhead please let me
 * know.
 */

int64_t       tickoverhead;
unsigned long clkoverhead;

#include <zero/prof.h>

#define PROF_ITERATIONS 524288

void
profinc(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2++;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "++\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profdec(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = PROF_ITERATIONS;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2--;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "--\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profadd(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2 += i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "+\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profsub(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2 -= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "-\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profmul(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 1;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2 *= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "*\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profdiv(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 /= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "/\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profmod(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 %= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "%%\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profand(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 &= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "&\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profor(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 |= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "|\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profxor(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 ^= i1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "^\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profnot(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 1 ; i1 < PROF_ITERATIONS + 1 ; i1++) {
        i2 = ~i1;
    }
    i1 = i2;    // to make the compiler shut up :)
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "~\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profshl(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2 <<= 1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "<<\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profshr(void)
{
    int i1, i2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    i2 = 0xffffffff;
    profstartclk(clock);
    profstarttick(tick);
    for (i1 = 0 ; i1 < PROF_ITERATIONS ; i1++) {
        i2 >>= 1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, ">>\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

int
main(int argc, char *argv[])
{
    int i = 0;
    PROFDECLCLK(clk);
    PROFDECLTICK(tick);

    profstarttick(tick);
    profstartclk(clk);
    for (i = 0 ; i < PROF_ITERATIONS ; i++) ;
    profstoptick(tick);
    profstopclk(clk);

    fprintf(stderr, "op\ttick/loop\tusec/loop\n");
    fprintf(stderr, "loop\t%LF\t%LF\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clk) / (long double)PROF_ITERATIONS);
    sleep(1);
    /* arithmetic operations. */
    profinc();
    sleep(1);
    profdec();
    sleep(1);
    profadd();
    sleep(1);
    profsub();
    sleep(1);
    profmul();
    sleep(1);
    profdiv();
    sleep(1);
    profmod();
    sleep(1);
    /* logical bitwise operations. */
    profand();
    sleep(1);
    profor();
    sleep(1);
    profxor();
    sleep(1);
    profnot();
    sleep(1);
    profshl();
    sleep(1);
    profshr();

    return 0;
}

