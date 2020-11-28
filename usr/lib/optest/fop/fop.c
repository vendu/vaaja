#include <math.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <unistd.h>

#include <zero/prof.h>

#define PROF_ITERATIONS 1024

void
profadd(void)
{
    long double ld1, ld2;
    double      d1, d2;
    float       f1, f2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    d2 = 0.0;
    profstartclk(clock);
    profstarttick(tick);
    for (d1 = 0.0 ; d1 < (double)PROF_ITERATIONS ; d1 += 1.0) {
        d2 += d1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "add\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    f2 = 0.0;
    profstartclk(clock);
    profstarttick(tick);
    for (f1 = 0.0 ; f1 < (float)PROF_ITERATIONS ; f1 += 1.0) {
        f2 += f1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "addf\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    ld2 = 0.0;
    profstartclk(clock);
    profstarttick(tick);
    for (ld1 = 0.0 ; ld1 < (long double)PROF_ITERATIONS ; ld1 += 1.0) {
        ld2 += ld1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "addl\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profmul(void)
{
    long double ld1, ld2;
    double      d1, d2;
    float       f1, f2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    d2 = 1.0;
    profstartclk(clock);
    profstarttick(tick);
    for (d1 = 0.0 ; d1 < (double)PROF_ITERATIONS ; d1 += 1.0) {
        d2 *= d1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "mul\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    f2 = 1.0;
    profstartclk(clock);
    profstarttick(tick);
    for (f1 = 0.0 ; f1 < (float)PROF_ITERATIONS ; f1 += 1.0) {
        f2 *= f1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "mulf\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    ld2 = 1.0;
    profstartclk(clock);
    profstarttick(tick);
    for (ld1 = 0.0 ; ld1 < (long double)PROF_ITERATIONS ; ld1 += 1.0) {
        ld2 *= ld1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "mull\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profsub(void)
{
    long double ld1, ld2;
    double      d1, d2;
    float       f1, f2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    d2 = DBL_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (d1 = 0.0 ; d1 < (double)PROF_ITERATIONS ; d1 += 1.0) {
        d2 -= d1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "sub\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    f2 = FLT_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (f1 = 0.0 ; f1 < (float)PROF_ITERATIONS ; f1 += 1.0) {
        f2 -= f1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "subf\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    ld2 = DBL_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (ld1 = 0.0 ; ld1 < (long double)PROF_ITERATIONS ; ld1 += 1.0) {
        ld2 -= ld1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "subl\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

void
profdiv(void)
{
    long double ld1, ld2;
    double      d1, d2;
    float       f1, f2;
    PROFDECLCLK(clock);
    PROFDECLTICK(tick);

    sleep(1);
    d2 = DBL_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (d1 = 0.0 ; d1 < (double)PROF_ITERATIONS ; d1 += 1.0) {
        d2 /= d1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "div\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    f2 = FLT_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (f1 = 0.0 ; f1 < (float)PROF_ITERATIONS ; f1 += 1.0) {
        f2 /= f1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "divf\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    sleep(1);
    ld2 = DBL_MAX;
    profstartclk(clock);
    profstarttick(tick);
    for (ld1 = 0.0 ; ld1 < (long double)PROF_ITERATIONS ; ld1 += 1.0) {
        ld2 /= ld1;
    }
    profstoptick(tick);
    profstopclk(clock);
    fprintf(stderr, "divl\t%Lf\t%Lf\n",
            (long double)proftickdiff(tick) / (long double)PROF_ITERATIONS,
            (long double)profclkdiff(clock) / (long double)PROF_ITERATIONS);

    return;
}

int
main(int argc, char *argv[])
{
    fprintf(stderr, "op\ttick/loosp\tusec/op\n");
    profadd();
    profsub();
    profmul();
    profdiv();

    return 0;
}

