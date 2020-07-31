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
    uint8_t mjolcanmovetomap[256 / CHAR_BIT];
    uint8_t mjolcanpickupmap[256 / CHAR_BIT];
    uint8_t mjolcanwearmap[256 / CHAR_BIT];
    uint8_t mjolcanwieldmap[256 / CHAR_BIT];
};

struct mjolobjcap   mjolobjcaps C_ALIGNED(MACH_PAGE_SIZE);

/* initialise bitmap for objects you can move on top of */
void
mjolinitcanmoveto(void)
{
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_FLOOR);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_SAND);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_DOOR);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_FOOD);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_WATER);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_FOUNTAIN);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_GOLD);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_BULLET);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_POTION);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_PLANT);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_PUNCHCARD);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_TAPE);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_STAIR_DOWN);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_STAIR_UP);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_STATUE);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_TRAP);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_WAND);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_SCROLL);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_RING);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_WHIP);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_ARMOR);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_CHAIN);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_CHEST);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_HONEY);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_KNIFE);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_LOCKPICK);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_LASER);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_MACE);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_MAINFRAME);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_PIPE);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_PISTOL);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_SWORD);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_WELL);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_CROSS);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_ALTAR);

    return;
}

/* initialise bitmap for objects you can pick up */
void
mjolinitcanpickup(void)
{
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_FOOD);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_WATER);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_GOLD);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_BULLET);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_POTION);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_PLANT);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_PUNCHCARD);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_TAPE);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_STATUE);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_WAND);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_SCROLL);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_RING);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_ARMOR);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_CHAIN);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_CHEST);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_HONEY);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_KNIFE);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_LOCKPICK);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_LASER);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_MACE);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_MAINFRAME);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_PIPE);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_PISTOL);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_SWORD);
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_CROSS);

    return;
}

/* initialise bitmap for objects you can wear */
void
mjolinitcanwear(void)
{
    setbit(mjolcanwearmap, MJOLNIR_OBJ_ARMOR);

    return;
}

/* initialise bitmap for weapons you can wield */
void
mjolinitcanwield(void)
{
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_CHAIN);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_SUBMACHINE_GUN);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_KNIFE);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_LASER);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_MACE);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_PIPE);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_PISTOL);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_LONGBOW);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_SWORD);
    setbit(mjolcanwieldmap, MJOLNIR_OBJ_CROSS);

    return;
}

void
mjolinitobj(void)
{
    mjolinitcanmoveto();
    mjolinitcanpickup();
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

