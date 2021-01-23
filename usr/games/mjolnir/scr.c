#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <mjolnir/scr.h>

#if defined(MJOLNIR_VGA_TEXT) && 0
extern void mjolopenvga(struct game *game);
#define MJOLNIR_VGA_TEXT_OPEN mjolopenvga
#else
#define MJOLNIR_VGA_TEXT_OPEN NULL
#endif
#if defined(MJOLNIR_TTY)
extern void gameopentty(struct game *game);
#define MJOLNIR_TTY_OPEN      gameopentty
#else
#define MJOLNIR_TTY_OPEN      NULL
#endif
#if defined(MJOLNIR_X11)
extern void gameopenx11(struct game *game);
#define MJOLNIR_X11_OPEN      gameopenx11
#else
#define MJOLNIR_X11_OPEN      NULL
#endif

typedef void gameopenscrfunc(struct game *);
static gameopenscrfunc *gameopenscrfuncs[4] = {
    NULL,
    MJOLNIR_VGA_TEXT_OPEN,
    MJOLNIR_TTY_OPEN,
    MJOLNIR_X11_OPEN
};

void
gameopenscr(struct game *game)
{
    gameopenscrfunc *func = gameopenscrfuncs[game->scrtype];

    if (func) {
        func(game);
    } else {
        fprintf(stderr, "no initialiser for screen type\n");

        exit(1);
    }

    return;
}

