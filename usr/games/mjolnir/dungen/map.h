#ifndef __DUNGEN_MAP_H__
#define __DUNGEN_MAP_H__

struct map {
    char  **wall;
    int     w;
    int     h;
};

struct map  mapgen(const int w, const int h, const int grid, const int max);
void        mapclose(const struct map);
void        mapprint(const struct map);

#endif /* __DUNGEN_MAP_H__ */

