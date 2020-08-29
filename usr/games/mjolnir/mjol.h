#ifndef __MJOLNIR_MJOL_H__
#define __MJOLNIR_MJOL_H__

#if (TEST)
#include <stdio.h>
#endif
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <zero/trix.h>
#include <mjolnir/conf.h>
#if (MJOLNIR_USE_ZERO_RANDMT32)
#include <zero/randmt32.h>
#endif
#if (MJOLNIR_CURSES)
#include <curses.h>
#endif
#include <dungeon/dng.h>
#include <mjolnir/obj.h>
#include <mjolnir/chr.h>

extern struct mjolgame             *mjolgame;
extern struct mjolchr              *mjolchaseq;

#define mjolismove(ch)              bitset(mjolcmdismovemap, ch)
#define mjolsetmove(ch)             setbit(mjolcmdismovemap, ch)
#define mjolhasdir(ch)              bitset(mjolcmdhasdirmap, ch)
#define mjolsetdir(ch)              setbit(mjolcmdhasdirmap, ch)
#define mjolhasarg(ch)              bitset(mjolcmdhasargmap, ch)
#define mjolsetarg(ch)              setbit(mjolcmdhasargmap, ch)
extern uint8_t                  	mjolcmdismovemap[128];
extern uint8_t                  	mjolcmdhasdirmap[128];
extern uint8_t                  	mjolcmdhasargmap[128];

#define mjolcanmoveto(type)         bitset(mjolcanmovetomap, type)
#define mjolcanpickup(type)         bitset(mjolcanpickupmap, type)
#define mjolcanwear(type)           bitset(mjolcanwearmap, type)
#define mjolcanwield(type)          bitset(mjolcanwieldmap, type)
extern uint8_t                  	mjolcanmovetomap[128 / CHAR_BIT];
extern uint8_t                  	mjolcanpickupmap[128 / CHAR_BIT];
extern uint8_t                  	mjolcanwearmap[128 / CHAR_BIT];
extern uint8_t                  	mjolcanwieldmap[128 / CHAR_BIT];

#define MJOLNIR_GAME_NAME           "mjolnir"

#define MJOLNIR_DEF_NICK            "johndoe"
#define MJOLNIR_NICK_MAX            16

#define MJOLNIR_LEVEL_HEAVEN        (-1)
#define MJOLNIR_LEVEL_GROUND        0       // Transylvania, Monastery
#define MJOLNIR_LEVEL_DUNGEON_MIN   1
#define MJOLNIR_LEVEL_DUNGEON_MAX   8
#define MJOLNIR_LEVEL_ORACLE1       9
#define MJOLNIR_LEVEL_ATLANTIS      10
#define MJOLNIR_LEVEL_LABYRINTH_MIN 11
#define MJOLNIR_LEVEL_LABYRINTH_MAX 14
#define MJOLNIR_LEVEL_ORACLE2       15
#define MJOLNIR_LEVEL_HELL_MIN      16
#define MJOLNIR_LEVEL_HELL_MAX      19
#define MJOLNIR_LEVEL_MAZE_MIN      20
#define MJOLNIR_LEVEL_MAZE_MAX      23
#define MJOLNIR_LEVEL_ORACLE3       24
#define MJOLNIR_LEVEL_CYBERIA_MIN   25
#define MJOLNIR_LEVEL_CYBERIA_MAX   28
#define MJOLNIR_LEVEL_ORACLE4       29
#define MJOLNIR_LEVEL_UNDERWORLD    30
#define MJOLNIR_LEVEL_VALHALLA      31
#define MJOLNIR_LEVELS              32
#define MJOLNIR_DEF_WIDTH           80
#define MJOLNIR_DEF_HEIGHT          48

#define MJOLNIR_SCR_NONE            0
#define MJOLNIR_SCR_TTY             1
#define MJOLNIR_SCR_NCURSES         2
#define MJOLNIR_SCR_SDL2            3

struct mjolgame {
    //    struct dnggame      data;
    const char         *name;
    volatile long       quit;
    int                 argc;
    char              **argv;
    struct mjolchr     *player;
    struct mjolchr     *chaseq;
    unsigned char     **nicktab;        // names of players
    struct mjolobj     *inventory;      // inventory
    long                scrtype;        // type of screen to use
    struct mjolscr     *scr;            // screen interface
    long                nlvl;           // # of levels
    long                lvl;            // current level
    struct map         *lvltab;         // level data
};

typedef long             mjolcmdfunc(struct mjolchr *src,
                                     struct mjolobj *dest);
typedef struct mjolobj * mjolcmdmovefunc(struct mjolgame *game,
                                         struct mjolchr *chr);

#endif /* __MJOLNIR_MJOL_H__ */

