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

uint8_t mjolcanmovetomap[256 / CHAR_BIT] ALIGNED(PAGESIZE);
uint8_t mjolcanpickupmap[256 / CHAR_BIT];
uint8_t mjolcanwearmap[256 / CHAR_BIT];
uint8_t mjolcanwieldmap[256 / CHAR_BIT];

/* initialise bitmap for objects you can move on top of */
void
mjolinitcanmoveto(void)
{
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_FLOOR);
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_CORRIDOR);
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
    setbit(mjolcanmovetomap, MJOLNIR_OBJ_LONGBOW);
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
    setbit(mjolcanpickupmap, MJOLNIR_OBJ_LONGBOW);
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
//    long            l = mjolrand() & 0x0f;

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_FLOOR;
#if 0
    if (!l) {
        /* 1/16 chance of a hidden door */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    }
#endif

    return obj;
}

struct mjolobj *
mjolmkcorridor(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() & 0x0f;

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_CORRIDOR;
    if (!l) {
        /* 1/16 chance of a hidden door */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    }

    return obj;
}

struct mjolobj *
mjolmkdoor(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() & 0x0f;

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_DOOR;
    if (!l) {
        /* 1/16 chance of a hidden door */
        obj->data.flg |= MJOLNIR_OBJ_HIDDEN;
    }

    return obj;
}

struct mjolobj *
mjolmkhorizwall(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_HORIZONTAL_WALL;

    return obj;
}

struct mjolobj *
mjolmkvertwall(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_VERTICAL_WALL;

    return obj;
}

struct mjolobj *
mjolmkstair(void)
{
    struct mjolobj *obj = calloc(1, sizeof(struct mjolobj));
    long            l = mjolrand() & 0x0f;

    if (!obj) {
        fprintf(stderr, "memory allocation failure\n");

        exit(1);
    }
    obj->data.type = MJOLNIR_OBJ_STAIR_DOWN;
    if (!l) {
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

