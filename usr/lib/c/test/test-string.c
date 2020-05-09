#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <c/strings.h>
#include <zero/trix.h>

#define TEST_TABSIZE 131072

static char *tab1;
static char *tab2;

void *
safe_memset(void *s, int val, size_t n)
{
    char       *cptr = s;
    char        byte = (char)val;

    while (n--) {
        *cptr++ = (char)byte;
    }

    return s;
}

void *
safe_memcpy(void *dest, void *src, size_t n)
{
    char       *cdest = dest;
    char       *csrc = src;

    while (n--) {
        *cdest++ = *csrc++;
    }

    return dest;
}

int
safe_memcmp(void *ptr1, void *ptr2, size_t n)
{
    unsigned char       *ucptr1 = ptr1;
    unsigned char       *ucptr2 = ptr2;
    int                  byte = 0;

    if (!n) {

        return 0;
    }
    do {
        byte = *ucptr1 - *ucptr2;
        ucptr1++;
        ucptr2++;
    } while (!byte && --n);

    return byte;
}

void
safe_bzero(void *s, size_t n)
{
    safe_memset(s, 0, n);

    return;
}

int
main(int argc, char *argv[])
{
    int         rnd;
    int         ofs;
    char       *cptr1;
    char       *cptr2;
    long        nloop;

    tab1 = calloc(TEST_TABSIZE, sizeof(char));
    tab2 = calloc(TEST_TABSIZE, sizeof(char));
    srand(0x666);
    nloop = 2048;
    fprintf(stderr, "testing bzero()/memset()... ");
    do {
        safe_memset(tab1, 0, TEST_TABSIZE);
        safe_memset(tab2, 0, TEST_TABSIZE);
        rnd = (rand() & (TEST_TABSIZE - 1)) + 1;
        rnd = max(rnd, sizeof(long));
        ofs = rand() & (sizeof(long) - 1);
        rnd -= ofs;
        cptr1 = tab1 + ofs;
        cptr2 = tab2 + ofs;
        safe_bzero(cptr1, rnd);
        bzero(cptr2, rnd);
        if (safe_memcmp(tab1, tab2, TEST_TABSIZE)) {

            exit(1);
        }
    } while (--nloop);
    fprintf(stderr, "PASSED\n");
    nloop = 2048;
    fprintf(stderr, "testing memcpy()... ");
    do {
        safe_memset(tab1, 0xff, TEST_TABSIZE);
        safe_memset(tab2, 0xff, TEST_TABSIZE);
        rnd = (rand() & (TEST_TABSIZE - 1)) + 1;
        rnd = max(rnd, sizeof(long));
        ofs = rand() & (sizeof(long) - 1);
        rnd -= ofs;
        cptr1 = tab1 + ofs;
        cptr2 = tab2 + ofs;
        bzero(cptr1, rnd);
        memcpy(cptr2, cptr1, rnd);
        if (safe_memcmp(tab1, tab2, TEST_TABSIZE)) {
            fprintf(stderr, "FAILED\n");

            exit(1);
        }
    } while (--nloop);
    fprintf(stderr, "PASSED\n");

    exit(0);
}

