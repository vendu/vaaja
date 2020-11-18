#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#include <mjolnir/item.h>
#include <mjolnir/chr.h>
#include <mjolnir/ice.h>

#define MJOLNIR_OBJ_CHARACTER   1
#define MJOLNIR_OBJ_ITEM        2

struct objchr {
    char                       *name;   // character name
    long                        type;   // type/ASCII-presentation
    long                        cat;    // category, 'class'
    long                        flg;    // character status flags
    struct ice                  ice;
};

struct objitem {
    char                       *name;   // object name for inventory etc.
    long                        type;   // type/ASCII-presentation
    long                        cat;    // category/subtype
    long                        flg;    // object status flags
    long                        hp;     // e.g. +1 or -1 for armor
    long                        def;    // defense points
    long                        hit;    // hit points
};

#endif /* __MJOLNIR_OBJ_H__ */

