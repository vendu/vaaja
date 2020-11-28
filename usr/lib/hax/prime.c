#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zero/trix.h>
#include <unistd.h>
#if defined(PRIMEPROF)
#include <zero/prof.h>
#endif

#define PRIMECHK 1
#define PRIMEN   (1U << 20)     // 1M

#if defined(PRIMECHK)
static unsigned long long  primelim;
static char               *primemap;
#endif

/*
 * Thanks to Warren Driscoll for spotting some redundant code and optimisations.
 */

/* Sieve of Erastosthenes with the optimisation to start from l^2 */
void *
sieve1(size_t lim)
{
    void *tab;
    long  l;
    long  m;
    long  n;

    n = roundup2(lim, 8) >> 3;
    tab = malloc(n * sizeof(char));
    if (tab) {
        memset(tab, 0xff, n * sizeof(char));
        l = 2;
        while (1) {
            for (m = l * l ; m < lim ; m += l) {
                clrbit(tab, m);
            }
            for ( ++l ; l < lim ; l++) {
                if (bitset(tab, l)) {

                    break;
                }
            }
            if (l == lim) {

                break;
            }
        }
    }

    return tab;
}

void *
sieve2(size_t lim)
{
    void *tab;
    long  l;
    long  m;
    long  n;

    n = roundup2(lim, 8) >> 3;
    tab = malloc(n * sizeof(char));
    if (tab) {
        memset(tab, 0xff, n * sizeof(char));
        l = 2;
        while (1) {
            for (m = l * l ; m < lim ; m += l) {
                clrbit(tab, m);
            }
            l++;
            if (!(l & 0x01)) {
                l++;
            }
            while (l < lim) {
                if (bitset(tab, l)) {

                    break;
                }
                l += 2;
            }
            if (l >= lim) {

                break;
            }
        }
    }

    return tab;
}

void *
sieve3(size_t lim)
{
    void *tab;
    long  l;
    long  m;
    long  n;

    n = roundup2(lim, 8) >> 3;
    tab = malloc(n * sizeof(char));
    if (tab) {
        memset(tab, 0xff, n * sizeof(char));
        l = 2;
        while (1) {
            for (m = l * l ; m < lim ; m += l) {
                clrbit(tab, m);
            }
            l++;
            l += !(l & 0x01);
            while (l < lim) {
                if (bitset(tab, l)) {

                    break;
                }
                l += 2;
            }
            if (l >= lim) {

                break;
            }
        }
    }

    return tab;
}

void *
sieve4(size_t lim)
{
    void *tab;
    long  l;
    long  m;
    long  n;

    n = roundup2(lim, 8) >> 3;
    tab = malloc(n * sizeof(char));
    if (tab) {
        memset(tab, 0xff, n * sizeof(char));
        l = 2;
        for (m = l * l ; m < lim ; m += l) {
            clrbit(tab, m);
        }
        l++;
        while (1) {
            for (m = l * l ; m < lim ; m += (l << 1)) {
                clrbit(tab, m);
            }
            l++;
            l += !(l & 0x01);
            while (l < lim) {
                if (bitset(tab, l)) {

                    break;
                }
                l += 2;
            }
            if (l >= lim) {

                break;
            }
        }
    }

    return tab;
}

#if defined(PRIMECHK)

static int
sieve(unsigned long long lim)
{
    void *tab = primemap;
    void *mptr;
    long  l;
    long  m;
    long  n;

    n = roundup2(lim, 8) >> 3;
    if (primelim <= lim) {
        if (tab) {
            mptr = realloc(tab, n * sizeof(char));
            if (!mptr) {
                free(tab);
                fprintf(stderr, "out of memory\n");

                exit(1);
            }
            tab = mptr;
        } else {
            tab = malloc(n * sizeof(char));
        }
        if (tab) {
            primemap = tab;
        }
    }
    if (tab) {
        memset(tab, 0xff, n * sizeof(char));
        l = 2;
        for (m = l * l ; m < lim ; m += l) {
            clrbit(tab, m);
        }
        l++;
        while (1) {
            for (m = l * l ; m < lim ; m += (l << 1)) {
                clrbit(tab, m);
            }
            l++;
            l += !(l & 0x01);
            while (l < lim) {
                if (bitset(tab, l)) {

                    break;
                }
                l += 2;
            }
            if (l >= lim) {

                break;
            }
            primemap = tab;
            primelim = lim;
        }
    }

    if (tab) {

        return 1;
    } else {

        return 0;
    }
}

static void
primeinit(unsigned long long num)
{
    if  (!sieve(num)) {
        fprintf(stderr, "cannot initialise prime bitmap\n");

        exit(1);
    }
}

int
isprime(unsigned long long num)
{
    int retval;

    if (!primelim || primelim <= num) {
        primeinit(num + 1);
    }
    retval = bitset(primemap, num);

    return retval;
}

static void
usage(void)
{
    fprintf(stderr, "USAGE\n");
    fprintf(stderr, "-----\n");
    fprintf(stderr, "prime <num>\ttest <num>\n");
    fprintf(stderr, "prime -l <lim>\tlist primes 0..<lim>\n");
    fprintf(stderr, "\n");

    return;
}

static void
primelst(unsigned long long lim)
{
    unsigned long long ull;

    sieve(lim);
    for (ull = 0 ; ull < lim ; ull++) {
        if (bitset(primemap, ull)) {
            printf("%llu\n", ull);
        }
    }

    return;
}

int
main(int argc, char *argv[])
{
    unsigned long long  val;
    char               *str;
    char               *ptr;
    char               *cp;
    int                 res;
#if defined(PRIMEPROF)
    PROFDECLCLK(clk);
#endif

#if defined(PRIMEPROF)
    profstartclk(clk);
#endif
    str = argv[1];
    ptr = str;
    if (argc == 3) {
        if (*str == '-' && toupper(str[1]) == 'L') {
            str = argv[2];
        } else {
            usage();

            exit(1);
        }
    } else if (argc != 2) {
        usage();

        exit(1);
    }
    if (*str == '0') {
        if (toupper(str[1]) == 'X') {
            ptr = str + 2;
            val = strtoull(ptr, &cp, 16);
        } else {
            ptr = str + 1;
            val = strtoull(ptr, &cp, 8);
        }
    } else {
        val = strtoull(str, &cp, 10);
    }
    if (*cp) {
        fprintf(stderr, "%s is not valid argument\n", str);

        exit(1);
    }
    if (argc == 2) {
        res = isprime(val);
        if (isatty(STDOUT_FILENO)) {
            if (res) {
                printf("%s is a prime\n", str);
            } else {
                printf("%s is NOT a prime\n", str);
            }
        }
        res = !res;
    } else {
        res = 0;
        primelst(val);
    }
#if defined(PRIMEPROF)
    profstopclk(clk);
    fprintf(stderr, "%s: %ld\n", argv[0], profclkdiff(clk));
#endif

    exit(res);
}

#else

int
main(int argc, char *argv[])
{
    uint8_t *tab1;
    uint8_t *tab2;
    uint8_t *tab3;
    uint8_t *tab4;
    int      i;
    PROFDECLCLK(clk);

    sleep(1);
    profstartclk(clk);
    tab1 = sieve1(PRIMEN);
    profstopclk(clk);
    printf("%ld\n", profclkdiff(clk));

    sleep(1);
    profstartclk(clk);
    tab2 = sieve2(PRIMEN);
    profstopclk(clk);
    printf("%ld\n", profclkdiff(clk));

    for (i = 2 ; i < PRIMEN >> 3 ; i++) {
        if (tab2[i] != tab1[i]) {
            fprintf(stderr, "ERROR sieve2(%d)\n", i);

            exit(1);
        }
    }

    sleep(1);
    profstartclk(clk);
    tab3 = sieve3(PRIMEN);
    profstopclk(clk);
    printf("%ld\n", profclkdiff(clk));

    for (i = 0 ; i < PRIMEN >> 3 ; i++) {
        if (tab3[i] != tab1[i]) {
            fprintf(stderr, "ERROR sieve2(%d)\n", i);

            exit(1);
        }
    }

    sleep(1);
    profstartclk(clk);
    tab4 = sieve4(PRIMEN);
    profstopclk(clk);
    printf("%ld\n", profclkdiff(clk));

    for (i = 0 ; i < PRIMEN >> 3 ; i++) {
        if (tab4[i] != tab1[i]) {
            fprintf(stderr, "ERROR sieve2(%d)\n", i);

            exit(1);
        }
    }

#if 0
    for (i = 2 ; i < PRIMEN ; i++) {
        if (bitset(tab2, i)) {
            printf("%d\n", i);
        }
//        printf("%d\t%lx\n", i, bitset(tab1, i));
    }
#endif

    exit(0);
}

#endif /* PRIMECHK */

