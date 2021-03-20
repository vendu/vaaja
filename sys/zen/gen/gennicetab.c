#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ULE_NICE_MIN    (-20)
#define ULE_NICE_MAX    19
#define ULE_NICE_RANGE  40

#define SPT_NICE_MIN    (-20)
#define SPT_NICE_MAX    19
#define SPT_NICE_RANGE  40

#define UNI_NICE_MIN    (-20)
#define UNI_NICE_DEF    0
#define UNI_NICE_MAX    19
#define UNI_NICE_RANGE  40

#define ULE_PRIO_MAX    64
#define ULE_LOW_PRIO    (-(ULE_PRIO_MAX / 2))
#define ULE_PRIO_LIM    (ULE_PRIO_MAX / 2)
#define ULE_PRIO_RANGE  ULE_PRIO_MAX
#define ULE_NICE_STEP   (ULE_NICE_MAX >> 4)
#define ULE_SLICE_SHIFT 3

#define SPT_PRIO_MAX    64
#define SPT_LOW_PRIO    (-(SPT_PRIO_MAX / 2))
#define SPT_PRIO_LIM    (SPT_PRIO_MAX / 2)
#define SPT_PRIO_RANGE  SPT_PRIO_MAX
#define SPT_NICE_STEP   (SPT_NICE_MAX >> 1)
#define SPT_SLICE_SHIFT 2

#define UNI_PRIO_MAX    64
#define UNI_LOW_PRIO    (-(UNI_PRIO_MAX / 2))
#define UNI_PRIO_LIM    (UNI_PRIO_MAX / 2)
#define UNI_PRIO_RANGE  UNI_PRIO_MAX
#define UNI_NICE_STEP   1
#define UNI_SLICE_SHIFT 4

long ulenicetab[ULE_PRIO_RANGE];
long uleslicetab[ULE_PRIO_RANGE];
long sptnicetab[SPT_PRIO_RANGE];
long sptslicetab[SPT_PRIO_RANGE];
long uninicetab[UNI_PRIO_RANGE];
long unislicetab[UNI_PRIO_RANGE];

void
genniceule(void)
{
    long   *ptr = &ulenicetab[ULE_PRIO_LIM];
    long    lval = -20;
    long    ndx;

    for (ndx = ULE_LOW_PRIO ; ndx < ULE_NICE_MIN ; ndx++) {
        ptr[ndx] = ndx;
    }
    lval = ndx;
    for ( ; ndx <= ULE_NICE_MAX ; ndx++) {
        ptr[ndx] = lval;
        lval++;
    }
    for ( ; ndx < ULE_PRIO_LIM ; ndx++) {
        ptr[ndx] = ndx;
    }

    return;
}

void
gensliceule(void)
{
    long   *ptr = &uleslicetab[ULE_PRIO_LIM];
    long    val = 1 << ULE_SLICE_SHIFT;
    long    ndx;

    for (ndx = ULE_LOW_PRIO ; ndx < ULE_PRIO_LIM ; ndx++) {
        ptr[ndx] = val >> ULE_SLICE_SHIFT;
        val++;
    }

    return;
}

void
gennicespt(void)
{
    long   *ptr = &sptnicetab[SPT_PRIO_LIM];
    double  ofs = -SPT_PRIO_LIM;
    long    ndx;

    for (ndx = SPT_LOW_PRIO ; ndx < SPT_NICE_MIN ; ndx++) {
        ptr[ndx] = 0;
    }
    for ( ; ndx < SPT_NICE_MAX ; ndx++) {
        ptr[ndx] = (int8_t)ofs;
        ofs += SPT_NICE_STEP;
    }
    for ( ; ndx < SPT_PRIO_LIM ; ndx++) {
        ptr[ndx] = 0;
    }

    return;
}

void
genslicespt(void)
{
    long *ptr = sptslicetab;
    long  val = 0;
    long  ndx;

    for (ndx = 0 ; ndx < SPT_PRIO_LIM ; ndx++) {
        val = ndx >> 4;
        ptr[ndx] = val;
    }

    return;
}

void
genniceuni(void)
{
    long   *ptr = &uninicetab[UNI_PRIO_LIM];
    double  ofs = -UNI_PRIO_LIM;
    long    ndx;

    for (ndx = UNI_LOW_PRIO ; ndx < UNI_NICE_MIN ; ndx++) {
        ptr[ndx] = 0;
    }
    for ( ; ndx < UNI_NICE_MAX ; ndx++) {
        ptr[ndx] = (int8_t)ofs;
        ofs += UNI_NICE_STEP;
    }
    for ( ; ndx < UNI_PRIO_LIM ; ndx++) {
        ptr[ndx] = 0;
    }

    return;
}

void
gensliceuni(void)
{
    long *ptr = unislicetab;
    long  val = 0;
    long  ndx;

    for (ndx = 0 ; ndx < UNI_PRIO_LIM ; ndx++) {
        val = ndx >> 4;
        ptr[ndx] = val;
    }

    return;
}

/* map nice values [-20, 19] to priorities [lo, hi] */
void
printniceule(void)
{
    long ndx;
    long n = 0;

    printf("/*\n");
    printf(" * lookup table to convert nice values to priority offsets\n");
    printf(" * as well as slices in ticks\n");
    printf(" *\n");
    printf(" * - nice is between -20 and 19 inclusive\n");
    printf(" * - taskniceptr = &tasknicetab[SCHED_ULE_NICE_HALF]\n");
    printf(" * - prio += taskniceptr[nice];\n");
    printf(" * - slice = tasksliceptr[nice];\n");
    printf(" */\n");
    printf("struct zennice k_schednicetab[SCHED_ULE_NICE_RANGE]\n= {\n    ");
    for (ndx = 0 ; ndx < ULE_PRIO_MAX ; ndx++) {
        printf("{ %3ld, %3ld }", ulenicetab[ndx], uleslicetab[ndx]);
        if (ndx != 63) {
            if (n == 3) {
                printf("\n    ");
                n = 0;
            } else {
                printf(", ");
                n++;
            }
        } else {
            printf("\n};");
        }
    }
    printf("\n\n");

    return;
}

void
printnicespt(void)
{
    long ndx;
    long n = 0;

    printf("long k_schedslicetab[SPT_PRIO_RANGE] = {\n    ");
    for (ndx = 0 ; ndx < SPT_PRIO_RANGE ; ndx++) {
        printf("{ %3ld, %3ld }", sptnicetab[ndx], sptslicetab[ndx]);
        if (ndx != 63) {
            if (n == 3) {
                printf(",\n    ");
                n = 0;
            } else {
                printf(", ");
                n++;
            }
        } else {
            printf("\n};");
        }
    }
    printf("\n\n");
}

void
printniceuni(void)
{
    long ndx;
    long n = 0;

    printf("long k_schedslicetab[UNI_PRIO_RANGE] = {\n    ");
    for (ndx = 0 ; ndx < UNI_PRIO_RANGE ; ndx++) {
        printf("{ %3ld, %3ld }", uninicetab[ndx], unislicetab[ndx]);
        if (ndx != 63) {
            if (n == 3) {
                printf(",\n    ");
                n = 0;
            } else {
                printf(", ");
                n++;
            }
        } else {
            printf("\n};");
        }
    }
    printf("\n\n");
}

void
printhelp(const char *msg)
{
    fprintf(stderr, "usage:\t%s [\"ule\" OR \"spt\" OR \"uni\"]", msg);

    exit(1);
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        printhelp(argv[0]);
    }
    if (!strcmp(argv[1], "ule")) {
        genniceule();
        gensliceule();
        printniceule();
    } else if (!strcmp(argv[1], "spt")) {
        gennicespt();
        genslicespt();
        printnicespt();
    } else if (!strcmp(argv[1], "uni")) {
        genniceuni();
        gensliceuni();
        printniceuni();
    } else {
        printhelp(argv[0]);
    }

    exit(0);
}

