#include <mjolnir/conf.h>

#if defined(MJOLNIR_TTY)

#include <stdlib.h>
#include <mjolnir/mjol.h>
#include <mjolnir/scr.h>
#include <mjolnir/tty.h>

int
mjolgetchtty(void)
{
    int retval = EOF;

#if defined(MJOLNIR_VT)
    printf("READ\n");
    retval = getchar();
    if (retval == EOF) {
        fprintf(stderr, "EOF read\n");

        exit(0);
    }
#elif defined(MJOLNIR_CURSES)
    retval = getch();
#endif

    return retval;
}

void
mjolmovetotty(int x, int y)
{
#if defined(MJOLNIR_VT)
    char esc[] = "\033[";
#endif

#if defined(MJOLNIR_VT)
    printf("%s", esc);
    printf("%d:%dH", y, x);
#elif defined(MJOLNIR_CURSES)
    /* TODO */
#endif
}

void
mjoldrawchrtty(struct game *game, struct chr *chr)
{
    struct scr     *scr = game->scr;
    long            lvl = game->lvl;
    int             x = chr->data.x;
    int             y = chr->data.y;

    if (x != scr->x || y != scr->y) {
        scr->moveto(x, y);
    }
#if defined(MJOLNIR_VT)
    printf("%c", game->objtab[lvl][x][y]->id);
#elif defined(MJOLNIR_CURSES)
    printw("%c", game->objtab[lvl][x][y]->id);
#endif
}

int
mjolrefreshtty(void)
{
    return 1;
}

void
mjolclosetty(void)
{
#if defined(MJOLNIR_CURSES)
    endwin();
#endif

    return;
}

void
mjolmkscrtty(struct game *game)
{
#if defined(MJOLNIR_CURSES)
    struct ttywins     *wins = calloc(1, sizeof(struct ttywins));
    WINDOW             *win;
#endif

#if defined(MJOLNIR_CURSES)
    /* create main window */
    win = newwin(game->height, game->width, 0, 0);
    if (!win) {
        mjolclosetty();
        fprintf(stderr, "window creation failure\n");

        exit(1);
    }
    wins->mainwin = win;
    touchwin(win);
    /* create game window */
    win = derwin(wins->mainwin, game->height - 2, game->width, 1, 0);
    if (!win) {
        mjolclosetty();
        fprintf(stderr, "window creation failure\n");

        exit(1);
    }
    wins->gamewin = win;
    /* create message window */
    win = derwin(wins->mainwin, 1, game->width, 0, 0);
    if (!win) {
        mjolclosetty();
        fprintf(stderr, "window creation failure\n");

        exit(1);
    }
    wins->msgwin = win;
    /* create status window */
    win = derwin(wins->mainwin, 1, game->width, game->height - 1, 0);
    if (!win) {
        mjolclosetty();
        fprintf(stderr, "window creation failure\n");

        exit(1);
    }
    wins->statwin = win;
#endif /* defined(MJOLNIR_CURSES) */
    game->scr = calloc(1, sizeof(struct scr));
    if (game->scr) {
        game->scr->wins = wins;
#if defined(MJOLNIR_VT)
        game->scr->getch = mjolgetchtty;
#elif defined(MJOLNIR_CURSES)
        game->scr->getch = getch;
#endif
        game->scr->moveto = mjolmovetotty;
        game->scr->drawchr = mjoldrawchrtty;
#if defined(MJOLNIR_VT)
        game->scr->printmsg = printf;
        game->scr->refresh = mjolrefreshtty;
#elif defined(MJOLNIR_CURSES)
        game->scr->printmsg = printw;
        game->scr->refresh = refresh;
#endif
        game->scr->close = mjolclosetty;
    } else {
        mjolclosetty();
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }

    return;
}

void
mjolopentty(struct game *game)
{
#if defined(MJOLNIR_VT)
    setvbuf(stdin, NULL, _IONBF, BUFSIZ);
#elif defined(MJOLNIR_CURSES)
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
#endif
    mjolmkscrtty(game);
#if 0
    game->scr->getch = getchtty;
    game->scr->moveto = movetotty;
    game->scr->drawchr = drawchrtty;
    game->scr->printmsg = printf;
#endif

    return;
}

#endif /* MJOLNIR_TTY */

