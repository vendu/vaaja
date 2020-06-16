#include <stdio.h>
#include <stdlib.h>
#include <malt/float.h>

void
fp_fprint(float f)
{
    fprintf(stderr, "%e\n", f);
    fprintf(stderr, "\texp: %d\tmant: %d\n", fp_fgetexp(f), fp_fgetmant(f));

    return;
}

void
fp_dprint(double d)
{
    fprintf(stderr, "%e\n", d);
    fprintf(stderr, "\texp: %lld\tmant: %lld\n", (long long)fp_dgetexp(d),
            (long long)fp_dgetmant(d));

    return;
}

