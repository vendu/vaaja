/* zero example on code style */

/* include statements [usually] in the beginning of code files */
#include <stddef.h>
#include <string.h>

/* non-structure typedefs */

typedef long optflg;

/* structures */

#define OPT_VERBOSE (1L << 0)
struct styleopt {
    optflg   flg;      // boolean option bits
    size_t   nfile;    // # of file names in fnametab
    char   **fnametab; // table of file name strings
};

/* global declarations */

struct {
    struct styleopt opt;
} g_style;

/* function declarations */

int
stylegetopt(struct styleopt *opt, int argc, char *argv[])
{
    /* local variables and structures, one per line */
    struct stat  statbuf;
    char        *str = argv[1];
    int          ndx = 1;

    while (ndx < argc) {
        if (str[0] == '-') {
            if (str[1] == '-') {
                if (!strcmp(str[2], "verbose")) {
                    opt->flg |= OPT_VERBOSE;
                }
            } else if (str[1] == 'v' && str[2] == '\0') {
                opt->flg |= OPT_VERBOSE;
            }
        } else {
            /* check existence and access permissions for files */
            /* stat() files and add buffers for regular ones */
        }
        ndx++;
    }
}

int
main(int argc, char *argv)
{
    if (argc > 1) {
        stylegetopt(&g_style.opt, argc, argv);
    }
}

