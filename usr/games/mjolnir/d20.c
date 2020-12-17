/* Copyright (C) Tuomo Petteri Venäläinen 2011 */

/* compile with something like:
 * gcc -O -o d20 d20.c
 * - vendu
 */

#include <mjolnir/mjolnir.h>

void
d20parsecmd(struct d20dice *buf, int argc, char *argv[])
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
d20init(void)
{
    seedrand(time(NULL));

    return;
}

void
d20add(struct d20dice *dicebuf, unsigned long die)
{
    switch (die) {
        default:
            fprintf(stderr, "invalid dice ID %lu\n", die);

            exit(1);
        case D20_DIE4:
            dicebuf->nd4++;

            break;
        case D20_DIE6:
            dicebuf->nd6++;

            break;
        case D20_DIE8:
            dicebuf->nd8++;

            break;
        case D20_DIE10:
            dicebuf->nd10++;

            break;
        case D20_DIE12:
            dicebuf->nd12++;

            break;
        case D20_DIE20:
            dicebuf->nd20++;

            break;
    }
}

unsigned long
d20rolldice(struct d20dice *dicebuf)
{
    unsigned long       retval = 0;
    unsigned long       n;

    n = dicebuf->nd4;
    if (n) {
        retval += d20rollndie(n, D20_DIE4);
    }
    n = dicebuf->nd6;
    if (n) {
        retval += d20rollndie(n, D20_DIE6);
    }
    n = dicebuf->nd8;
    if (n) {
        retval += d20rollndie(n, D20_DIE8);
    }
    n = dicebuf->nd10;
    if (n) {
        retval += d20rollndie(n, D20_DIE10);
    }
    n = dicebuf->nd12;
    if (n) {
        retval += d20rollndie(n, D20_DIE12);
    }
    n = dicebuf->nd20;
    if (n) {
        retval += d20rollndie(n, D20_DIE20);
    }

    return retval;
}

