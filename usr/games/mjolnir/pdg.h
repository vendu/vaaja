#ifndef __MJOLNIR_PDG_H__
#define __MJOLNIR_PDG_H__

#include <mjolnir/conf.h>
#include <stdlib.h>

#define PDG_DEF_LIFE_PROB       0x4fffffff
#define PDG_DEF_DEATH_LIMIT     3
#define PDG_DEF_BIRTH_MIN       5
//#define PDG_DEF_STARVE_LIMIT    6

#define pdgseed(x)              mjolsrand(x)
#define pdgrand()               (mjolrand() & 0x7fffffff)
#define pdgprob(x)              (mjolrand() < (x))

typedef unsigned char mapitem_t;

struct map {
    unsigned int    seed;
    int             lifeprob;   // probability a cell starts alive
    int             deathlim;   // minimum number of live neighbors to live
    int             birthmin;   // minimum number of live neighbors to born
    int             w;          // map width
    int             h;          // map height
    mapitem_t      *tab;        // map items
};

#endif /* __MJOLNIR_PDG_H__ */

