#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CTYPE_ALNUM_BIT         (1 << 0)
#define CTYPE_ALPHA_BIT         (1 << 1)
#define CTYPE_CNTRL_BIT         (1 << 2)
#define CTYPE_DIGIT_BIT         (1 << 3)
#define CTYPE_GRAPH_BIT         (1 << 4)
#define CTYPE_LOWER_BIT         (1 << 5)
#define CTYPE_PRINT_BIT         (1 << 6)
#define CTYPE_PUNCT_BIT         (1 << 7)
#define CTYPE_SPACE_BIT         (1 << 8)
#define CTYPE_UPPER_BIT         (1 << 9)
#define CTYPE_XDIGIT_BIT        (1 << 10)
#define CTYPE_ASCII_BIT         (1 << 11)
#define CTYPE_BLANK_BIT         (1 << 12)

typedef uint16_t                ctypeflag_t;

ctypeflag_t                     ctypeflagtab[256];

int
main(int argc, char *argv[])
{
    int                         ch;
    int                         ofs;
    ctypeflag_t                 val;

    for (ch = 0 ; ch < 256 ; ch++) {
        val = 0;
        if (isalnum(ch)) {
            val |= CTYPE_ALNUM_BIT;
        }
        if (isalpha(ch)) {
            val |= CTYPE_ALPHA_BIT;
        }
        if (iscntrl(ch)) {
            val |= CTYPE_CNTRL_BIT;
        }
        if (isdigit(ch)) {
            val |= CTYPE_DIGIT_BIT;
        }
        if (isgraph(ch)) {
            val |= CTYPE_GRAPH_BIT;
        }
        if (islower(ch)) {
            val |= CTYPE_LOWER_BIT;
        }
        if (isprint(ch)) {
            val |= CTYPE_PRINT_BIT;
        }
        if (ispunct(ch)) {
            val |= CTYPE_PUNCT_BIT;
        }
        /* space, upper, xdigit, ascii, blank */
        if (isspace(ch)) {
            val |= CTYPE_SPACE_BIT;
        }
        if (isupper(ch)) {
            val |= CTYPE_UPPER_BIT;
        }
        if (isxdigit(ch)) {
            val |= CTYPE_XDIGIT_BIT;
        }
        if (isascii(ch)) {
            val |= CTYPE_ASCII_BIT;
        }
        if (isblank(ch)) {
            val |= CTYPE_BLANK_BIT;
        }
    }

    ofs = 0;
    printf("k_ctypeflagtab[256] = {\n    ");
    for (ch = 0 ; ch < 256 ; ch++) {
        printf("0x%04hx", ch);
        if (ch != 255) {
            if (ofs == 7) {
                printf(",\n    ");
                ofs = 0;
            } else {
                printf(", ");
                ofs++;
            }
        } else {
            printf("\n};");
        }
    }
    printf("\n\n");

    exit(0);
}

