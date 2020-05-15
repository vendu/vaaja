#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>

void
mjolusage(void)
{
    printf("mjolnir <options>\n");
    printf("--help\t\tprint this help\n");
    printf("-n name\t\tset name of player\n");
    printf("-w num\t\tset level width\n");
    printf("-h num\t\tset level height\n");
    printf("-W type\t\tset screen type (tty)\n");

    return;
}

void
mjolgetopt(struct mjolgame *game, int argc, char *argv[])
{
    long   ndx;
    char  *str;
    char  *ptr;

    for (ndx = 1 ; ndx < argc ; ndx++) {
        str = argv[ndx];
        if (!strncmp(str, "--help", 6)) {
            mjolusage();

            exit(0);
        } else if (!strncmp(str, "-n", 2)) {
            /* set player nickname */
            ndx++;
            str = argv[ndx];
            game->nick = str;
        } else if (!strncmp(str, "-w", 2)) {
            /* set level width */
            ndx++;
            str = argv[ndx];
            if (!str) {
                fprintf(stderr, "level width not specified\n");

                exit(1);
            }
            game->width = strtol(str, &ptr, 10);
            if (ptr) {
                fprintf(stderr, "invalid level width: %s\n", str);
            }
        } else if (!strncmp(str, "-h", 2)) {
            /* set level height */
            ndx++;
            str = argv[ndx];
            if (!str) {
                fprintf(stderr, "level height not specified\n");

                exit(1);
            }
            game->height = strtol(str, &ptr, 10);
            if (ptr) {
                fprintf(stderr, "invalid level height: %s\n", str);
            }
        } else if (!strncmp(str, "-W", 2)) {
            /* set screen type */
            ndx++;
            str = argv[ndx];
            if (!str) {
                fprintf(stderr, "screen type not specified\n");

                exit(1);
            }
#if (MJOLNIR_VGA_TEXT)
            if (!strncmp(str, "vga", 3)) {
                game->scrtype = MJOLNIR_SCR_VGA_TEXT;
            }
#else
            if (!strncmp(str, "vga", 3)) {
                fprintf(stderr, "unsupported screen type vga\n");

                exit(1);
            }
#endif
            if (!game->scrtype) {
#if (MJOLNIR_TTY)
                if (!strncmp(str, "tty", 3)) {
                    game->scrtype = MJOLNIR_SCR_TTY;
                }
#else
                if (!strncmp(str, "tty", 3)) {
                    fprintf(stderr, "unsupported screen type tty\n");

                    exit(1);
                }
#endif
                if (!game->scrtype) {
#if (MJOLNIR_X11)
                    if (!strncmp(str, "x11", 3)) {
                        game->scrtype = MJOLNIR_SCR_X11;
                    }
#else
                    if (!strncmp(str, "x11", 3)) {
                        fprintf(stderr, "unsupported screen type x11\n");

                        exit(1);
                    }
#endif
                }
            }
        }
    }

    return;
}

