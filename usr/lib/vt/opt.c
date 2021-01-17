#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vt/vt.h>

void
vtgetopt(struct vt *vt, int argc, char *argv[])
{
    int   ndx;
    char *cp;
    long  val;

    for (ndx = 1 ; ndx < argc ; ndx++) {
        cp = argv[ndx];
        if (!strncmp(cp, "--font", 6)) {
            ndx++;
            if (ndx < argc) {
                cp = argv[ndx];
                vt->font.name = cp;
            } else {
                fprintf(stderr, "VT: --font: no argument\n");

                exit(1);
            }
        } else if (!strncmp(cp, "--width", 7)) {
            ndx++;
            if (ndx < argc) {
                cp = argv[ndx];
                val = strtol(cp, NULL, 10);
                if (val > 0) {
                    vt->state.ncol = val;
                } else {
                    fprintf(stderr, "VT: --width: invalid argument %s\n", cp);

                    exit(1);
                }
            } else {
                fprintf(stderr, "VT: --width: no argument\n");

                exit(1);
            }
        } else if (!strncmp(cp, "--height", 8)) {
            ndx++;
            if (ndx < argc) {
                cp = argv[ndx];
                val = strtol(cp, NULL, 10);
                if (val > 0) {
                    vt->state.nrow = val;
                } else {
                    fprintf(stderr, "VT: --height: invalid argument %s\n", cp);

                    exit(1);
                }
            } else {
                fprintf(stderr, "VT: --height: no argument\n");

                exit(1);
            }
        } else if (!strncmp(cp, "--bufrows", 9)) {
            ndx++;
            if (ndx < argc) {
                cp = argv[ndx];
                val = strtol(cp, NULL, 10);
                if (val > 0) {
                    vt->textbuf.nrow = val;
                } else {
                    fprintf(stderr, "VT: --bufrows: invalid argument %s\n", cp);

                    exit(1);
                }
            } else {
                fprintf(stderr, "VT: --bufrows: no argument\n");

                exit(1);
            }
        } else if (!strncmp(cp, "--exec", 6)) {
            fprintf(stderr, "CMD:");
            while (ndx < argc - 1) {
                ndx++;
                fprintf(stderr, " %s", argv[ndx]);
            }
            fprintf(stderr, "\n");
        }
    }

    return;
}

