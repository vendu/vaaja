#ifndef __MJOLNIR_DNG_H__
#define __MJOLNIR_DNG_H__

#include <stdint.h>

struct room {
    struct room        *prev;
    struct room        *next;
    int                 x;
    int                 y;
    int                 w;
    int                 h;
};

struct dng {
    struct room        *roomlist;
    uint8_t            *bmap;
    char               *celltab;
    int                 nempty;
    int                 w;
    int                 h;
    unsigned int        seed;
};

struct dng              dnggen(const int w,
                               const int h,
                               const int grid,
                               const int max);
void                    dngclose(const struct dng);
void                    dngprint(const struct dng);

#endif /* __MJOLNIR_DNG_H__ */

