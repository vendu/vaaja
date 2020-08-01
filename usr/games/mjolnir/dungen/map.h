#ifndef __DUNGEN_MAP_H__
#define __DUNGEN_MAP_H__

#include <stdint.h>

struct map {
    uint8_t        *bmap;
    char          **wall;
    int             nempty;
    int             w;
    int             h;
    unsigned int    seed;
};

struct map  dungenmap(const int w, const int h, const int grid, const int max);
void        dungenclose(const struct map);
void        dungenprint(const struct map);

#endif /* __DUNGEN_MAP_H__ */

