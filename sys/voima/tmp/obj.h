#ifndef __VOIMA_OBJ_H__
#define __VOIMA_OBJ_H__

struct voimaxy {
    int32_t     x;
    int32_t     y;
};

struct voimaxyz {
    int32_t     x;
    int32_t     y;
    int32_t     z;
};

struct voimaxyzt {
    int32_t     x;
    int32_t     y;
    int32_t     z;
    int32_t     t;
};

typedef voimaline_t {
    struct voimaxy  r;
    struct voimaxy  l;
};

typedef voimarect_t {
    struct voimaxy  topr;
    struct voimaxy  bottomr;
    struct voimaxy  topl;
    struct voimaxy  bottoml;
};

struct voimaobj {
    uint32_t    id;
};

#endif /* __VOIMA_OBJ_H__ */

