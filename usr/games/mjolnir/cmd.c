#include <zero/cdefs.h>
#include <asm/param.h>
#include <mjolnir/mjol.h>

extern void mjolbeep(void);

mjolcmdfunc     *mjolcmdfunctab[1024][1024] ALIGNED(CLSIZE);
mjolcmdmovefunc *mjolcmdmovefunctab[1024] ALIGNED(PAGESIZE);
uint8_t          mjolcmdismovemap[256];
uint8_t          mjolcmdhasdirmap[256];
uint8_t          mjolcmdhasargmap[256];

#if 0
struct mjolobj *
mjolidentify(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest;

    char  *str = dest->str;
    int  (*printmsg)(const char *, ...) = game->scr->printmsg;

    if (str) {
        printmsg("%s\n", str);
    }

    return NULL;
}
#endif

struct mjolobj *
mjolmoveleft(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmovedown(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmoveup(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmoveright(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmoveupleft(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmoveupright(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmovedownleft(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

struct mjolobj *
mjolmovedownright(struct mjolgame *game, struct mjolchr *chr)
{
    struct mjolobj *dest = NULL;

    return dest;
}

void
mjolinitcmd(void)
{
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_LEFT] = mjolmoveleft;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_DOWN] = mjolmovedown;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_UP] = mjolmoveup;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_RIGHT] = mjolmoveright;
#if (MJOLNIR_CURSES)
    mjolcmdmovefunctab[MJOLNIR_CMD_LEFT] = mjolmoveleft;
    mjolcmdmovefunctab[MJOLNIR_CMD_DOWN] = mjolmovedown;
    mjolcmdmovefunctab[MJOLNIR_CMD_UP] = mjolmoveup;
    mjolcmdmovefunctab[MJOLNIR_CMD_RIGHT] = mjolmoveright;
#endif
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_UPLEFT] = mjolmoveupleft;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_UPRIGHT] = mjolmoveupright;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_DOWNLEFT] = mjolmovedownleft;
    mjolcmdmovefunctab[MJOLNIR_CMD_MOVE_DOWNRIGHT] = mjolmovedownright;
    mjolsetmove(MJOLNIR_CMD_MOVE_LEFT);
    mjolsetmove(MJOLNIR_CMD_MOVE_DOWN);
    mjolsetmove(MJOLNIR_CMD_MOVE_UP);
    mjolsetmove(MJOLNIR_CMD_MOVE_RIGHT);
#if (MJOLNIR_CURSES)
    mjolsetmove(MJOLNIR_CMD_LEFT);
    mjolsetmove(MJOLNIR_CMD_DOWN);
    mjolsetmove(MJOLNIR_CMD_UP);
    mjolsetmove(MJOLNIR_CMD_RIGHT);
#endif
    mjolsetmove(MJOLNIR_CMD_MOVE_UPLEFT);
    mjolsetmove(MJOLNIR_CMD_MOVE_UPRIGHT);
    mjolsetmove(MJOLNIR_CMD_MOVE_DOWNLEFT);
    mjolsetmove(MJOLNIR_CMD_MOVE_DOWNRIGHT);

    mjolsetdir(MJOLNIR_CMD_FIGHT);
    mjolsetdir(MJOLNIR_CMD_THROW);
    mjolsetdir(MJOLNIR_CMD_MOVE);
    mjolsetdir(MJOLNIR_CMD_ZAP);
    mjolsetdir(MJOLNIR_CMD_IDENTIFY_TRAP);

    mjolsetarg(MJOLNIR_CMD_IDENTIFY);
    mjolsetarg(MJOLNIR_CMD_THROW);
    mjolsetarg(MJOLNIR_CMD_ZAP);
    mjolsetarg(MJOLNIR_CMD_QUAFF);
    mjolsetarg(MJOLNIR_CMD_READ);
    mjolsetarg(MJOLNIR_CMD_EAT);
    mjolsetarg(MJOLNIR_CMD_WIELD);
    mjolsetarg(MJOLNIR_CMD_TAKE_OFF);
    mjolsetarg(MJOLNIR_CMD_PUT_RING_ON);
    mjolsetarg(MJOLNIR_CMD_REMOVE_RING);
    mjolsetarg(MJOLNIR_CMD_DROP);
    mjolsetarg(MJOLNIR_CMD_CALL);

    return;
}

