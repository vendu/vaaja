#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mjolnir/conf.h>
#if (MJOLNIR_USE_ZERO_RANDMT32)
#include <zero/randmt32.h>
#endif
#include <zero/cdefs.h>
#include <asm/param.h>
#include <zero/trix.h>
#include <mjolnir/mjol.h>

struct mjolobjcap {
    uint8_t canmove[256 / CHAR_BIT];
    uint8_t canhit[256 / CHAR_BIT];
    uint8_t canpick[256 / CHAR_BIT];
    uint8_t canwear[256 / CHAR_BIT];
    uint8_t canwield[256 / CHAR_BIT];
};

struct mjolobjparm  mjolobjparms[256];
struct mjolobjcap   mjolobjcap C_ALIGNED(MACH_PAGE_SIZE);

void
mjolinitobjparm(sym, ntotal, nmax, minlvl, maxlvl, problim, probsft)
{
    mjolobjparms[sym].sym = sym;
    mjolobjparms[sym].ntotal = ntotal;
    mjolobjparms[sym].nmax = nmax;
    mjolobjparms[sym].minlvl = minlvl;
    mjolobjparms[sym].maxlvl = maxlvl;
    mjolobjparms[sym].problim = problim;
    mjolobjparms[sym].probsft = probsft;

    return;
}

/* initialise bitmap for objects you can move on top of */
void
mjolinitcanmove(void)
{
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_FLOOR);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_SAND);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_DOOR);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_FOOD);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_WATER);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_FOUNTAIN);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_GOLD);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_BULLET);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_POTION);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_PLANT);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_PUNCHCARD);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_TAPE);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_STAIR_DOWN);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_STAIR_UP);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_STATUE);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_TRAP);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_WAND);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_SCROLL);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_RING);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_WHIP);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_ARMOR);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_CHAIN);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_CHEST);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_HONEY);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_KNIFE);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_LOCKPICK);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_LASER);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_MACE);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_MAINFRAME);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_PIPE);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_PISTOL);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_SWORD);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_WELL);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_CROSS);
    setbit(mjolobjcap.canmove, MJOLNIR_OBJ_ALTAR);

    return;
}

void
mjolinitcanhit(void)
{
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_SOLDIER_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_BEE);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_BEE_QUEEN);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_DOG);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ALIEN);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_GHOUL);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_HUMAN);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ORACLE);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_RAT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_SHOPKEEPER);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_UNICORN);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_VAMPIRE);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_WOLF);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_WEREWOLF);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ZOMBIE);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
    setbit(mjolobjcap.canhit, MJOLNIR_CHAR_ANT);
}

/* initialise bitmap for objects you can pick up */
void
mjolinitcanpick(void)
{
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_FOOD);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_WATER);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_GOLD);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_BULLET);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_POTION);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_PLANT);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_PUNCHCARD);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_TAPE);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_STATUE);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_WAND);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_SCROLL);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_RING);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_ARMOR);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_CHAIN);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_CHEST);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_HONEY);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_KNIFE);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_LOCKPICK);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_LASER);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_MACE);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_MAINFRAME);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_PIPE);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_PISTOL);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_SWORD);
    setbit(mjolobjcap.canpick, MJOLNIR_OBJ_CROSS);

    return;
}

/* initialise bitmap for objects you can wear */
void
mjolinitcanwear(void)
{
    setbit(mjolobjcap.canwear, MJOLNIR_OBJ_ARMOR);

    return;
}

/* initialise bitmap for weapons you can wield */
void
mjolinitcanwield(void)
{
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_CHAIN);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_KNIFE);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_LASER);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_MACE);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_PIPE);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_PISTOL);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_SWORD);
    setbit(mjolobjcap.canwield, MJOLNIR_OBJ_CROSS);

    return;
}

void
mjolinitobj(void)
{
    mjolinitcanmove();
    mjolinitcanpick();
    mjolinitcanwear();
    mjolinitcanwield();
}

struct mjolobj *
mjolmkfloor(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val1 = 1L << (24 - 5);
    long            val2 = 1L << (24 - 7);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_FLOOR;
    if (l < val1) {
        /* 1/32 chance of a hidden object in the floor */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    } else if (l < val2) {
        /* 1/128 chance of a trap on the floor */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_TRAP;
    }

    return obj;
}

struct mjolobj *
mjolmksand(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val1 = 1L << (24 - 5);
    long            val2 = 1L << (24 - 7);
    long            val3 = 1L << (24 - 6);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_SAND;
    if (l < val1) {
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    } else if (l < val2) {
        /* 1/128 chance of quick-sand */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_QUICK_SAND;
    } else if (l < val3) {
        /* 1/64 chance of a trap */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_TRAP;
    }

    return obj;
}

struct mjolobj *
mjolmkwall(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val1 = 1L << (24 - 6);
    long            val2 = 1L << (24 - 7);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_WALL;
    if (l < val1) {
        /* 1/64 chance of a hidden carving in the wall */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_CARVING;
    } else if (l < val2) {
        /* 1/128 chance of a hidden trap in the wall */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_TRAP;
    }

    return obj;
}

struct mjolobj *
mjolmkdoor(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val = 1L << (24 - 6);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_DOOR;
    if (l < val) {
        /* 1/64 chance of a hidden door */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN_DOOR;
    }

    return obj;
}

struct mjolobj *
mjolmkstairdown(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val = 1L << (24 - 7);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_STAIR_DOWN;
    if (l < val) {
        /* 1/128 chance of a hidden stairway */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    }

    return obj;
}

struct mjolobj *
mjolmkstairup(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() >> 7;
    long            val = 1L << (24 - 7);

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_STAIR_UP;
    if (l < val) {
        /* 1/16 chance of a hidden stairway */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    }

    return obj;
}

long
mjolhit(struct mjolchr *src, struct mjolchr *dest)
{
    long   retval = 0;
    long (*func)(void *, void *);

    func = src->func.hit;
    if (func) {
        retval = func(src, dest);
    }

    return retval;
}

long
mjoltrap(struct mjolobj *trap, struct mjolchr *dest)
{
    long   retval = 0;
    long (*func)(void *, void *);

    func = trap->func.hit;
    if (func) {
        retval = func(trap, dest);
    }

    return retval;
}

