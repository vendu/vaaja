#ifndef __MJOLNIR_MAP_H__
#define __MJOLNIR_MAP_H__

#include <mjolnir/conf.h>
#include <stdlib.h>

#define MAP_DEF_LIFE_PROB       0x4fffffff
#define MAP_DEF_DEATH_LIMIT     3
#define MAP_DEF_BIRTH_MIN       5
//#define MAP_DEF_STARVE_LIMIT    6

typedef unsigned char mapitem_t;

struct map {
    unsigned int    seed;
    int             lifeprob;   // probability a cell starts alive
    int             deathlim;   // minimum number of live neighbors to live
    int             birthmin;   // minimum number of live neighbors to born
    int             w;          // level width
    int             h;          // level height
    mapitem_t      *tab;        // level items
};

#endif /* __MJOLNIR_MAP_H__ */

