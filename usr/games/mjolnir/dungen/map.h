#ifndef __DUNGEN_MAP_H__
#define __DUNGEN_MAP_H__

struct map {
    uint8_t        *bmap;
    char          **wall;
    int             nempty;
    int             w;
    int             h;
    unsigned int    seed;
};

struct map  mapgen(const int w, const int h, const int grid, const int max);
void        mapclose(const struct map);
void        mapprint(const struct map);

#endif /* __DUNGEN_MAP_H__ */

