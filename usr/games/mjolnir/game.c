#include <stdio.h>
#include <stdlib.h>
#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>
#include <mjolnir/scr.h>

extern long               mjolgetopt(struct mjolgame *game,
                                     int argc, char *argv[]);
extern struct mjolchr   * mjolmkplayer(struct mjolgame *game);
extern struct mjolrect ** mjolinitrooms(struct mjolgame *game, long *nroom);
extern void               mjolopenscr(struct mjolgame *game);
extern void               mjolinitcmd(void);
extern void               mjolinitobj(void);
void                      mjolgendng(struct mjolgame *game);
extern void               mjoldocmd(struct mjolgame *game, int ch);
extern long               mjoldoturn(struct mjolgame *game,
                                     struct mjolchr *data);
extern long               mjolchaseall(struct mjolgame *game);

extern struct mjolchr *mjolplayer;
extern struct mjolchr *mjolchaseq;

static char            mjolgamename[] = "mjolnir";
static volatile long   mjolquitgame;
struct mjolgame       *mjolgame;

void
mjolquit(long val)
{
    void (*func)(void) = mjolgame->scr->close;

    if (func) {
        func();
    }

    exit(val);
}

void
mjolquitsig(int sig)
{
    mjolquit(sig);
}

void
mjolintro(void)
{
    printf("\n");
    printf("Legend has it that many moons ago, Fenris, the eldest child of Loki and\n");
    printf("Angrboda, broke free from his chain, Pleignir, and stole Mjolnir from Thor.\n");
    printf("Fenris is believed to have hidden Mjolnir, Thor's hammer, into a dungeon.\n");
    printf("\n");
    printf("It is said that Thor shall ascend the one to return Mjolnir to a deity for\n");
    printf("saving the world from the forces of evil creating chaos with lightnings and\n");
    printf("thunder. Clairvoyants say evil forces are already on the hunt for Mjolnir.\n");
    printf("\n");
    printf("Armed with nothing more than a few food rations, your pet dog, a +1 blessed\n");
    printf("armor, a +3 neutral sword, and a piece of Pleignir, you enter the Dungeon.\n");
    printf("\n");
    printf("Good luck, adventurer, and beware of Fenris!\n");
    printf("\n");
    printf("Press a key to continue...\n");
#if (MJOLNIR_VT) || (MJOLNIR_CURSES)
    getchar();
#endif

    return;
}

void
mjolinit(struct mjolgame *game, int argc, char *argv[])
{
    struct mjolgame    *data = calloc(1, sizeof(struct mjolgame *));
    struct mjolobj  ****objtab;
    struct mjolchr  ****chrtab;
    long                y;
    long                w;
    long                h;
    long                lvl;
    long                lim;

    mjolgame = game;
    if (!data) {
        fprintf(stderr, "failed to allocate game data\n");

        exit(1);
    }
    game->data.name = mjolgamename;
    game->nick = calloc(1, sizeof(struct dngchr *));
    mjolgetopt(game, argc, argv);
    if (!game->player) {
        game->player = mjolmkplayer(game);
    }
    if (!game->nick) {
        game->nick = MJOLNIR_DEF_NICK;
    }
    if (!game->scrtype) {
#if (MJOLNIR_TTY)
        game->scrtype = MJOLNIR_SCR_TTY;
#elif (MJOLNIR_VGA_TEXT)
        game->scrtype = MJOLNIR_SCR_VGA_TEXT;
#endif
    }
    if (!game->scrtype) {
        fprintf(stderr, "no supported screen type found\n");

        exit(1);
    }
    mjolintro();
    if (!game->nlvl) {
        game->nlvl = MJOLNIR_DEF_NLVL;
    }
    if (!game->width) {
        game->width = MJOLNIR_DEF_WIDTH;
    }
    if (!game->height) {
        game->height = MJOLNIR_DEF_HEIGHT;
    }
    objtab = calloc(game->nlvl, sizeof(struct mjolobj ***));
    chrtab = calloc(game->nlvl, sizeof(struct mjolchr ***));
#if 0
    for (y = 0 ; y < game->height ; y++) {
        objtab[y] = calloc(game->width, sizeof(struct mjolobj *));
        if (!objtab[y]) {
            fprintf(stderr, "memory allocation failure\n");

            exit(1);
        }
    }
#endif
    lim = game->nlvl;
    w = game->width;
    h = game->height;
    for (lvl = 0 ; lvl < lim ; lvl++) {
        objtab[lvl] = calloc(h, sizeof(struct mjolobj ***));
        chrtab[lvl] = calloc(h, sizeof(struct mjolchr ***));
        for (y = 0 ; y < h ; y++) {
            objtab[lvl][y] = calloc(w, sizeof(struct mjolobj **));
            chrtab[lvl][y] = calloc(w, sizeof(struct mjolchr **));
        }
    }
    game->objtab = objtab;
    game->chrtab = chrtab;
    mjolopenscr(game);
    mjolinitobj();
    mjolinitcmd();
    mjolgendng(game);
#if (MJOLDEBUG) && 0
    mjolclosetty();
#if 0
    for (lvl = 0 ; lvl < game->nlvl ; lvl++) {
        mjolprintlvl(game, lvl);
    }
#endif
//    mjolprintlvl(game, 0);
//    sleep(5);
#endif

    return;
}

void
mjolgameloop(struct mjolgame *game)
{
#if (MJOLDEBUG)
//    mjolprintlvl(game, 0);

    exit(1);
#endif
    do {
        mjolplayer->hp -= mjoldoturn(game, mjolplayer);
        mjolplayer->hp -= mjolchaseall(game);
    } while (!mjolquitgame);
    mjolquit(0);

    /* NOTREACHED */
    exit(0);
}

