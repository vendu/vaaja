#ifndef ZEN_TYPES_H
#define ZEN_TYPES_H

#include <stdint.h>
#include <api/zen/types.h>

struct zendev {
    void                       *vfsfuncs;
    zendev_t                    dev;       // 16-bit major and minor IDs
    zenbus_t                    bus;
    zendevid_t                  id;
    uint32_t                    flg;
};

struct zencpu {
    zentime_t                   ntick;
};

struct zendesc {
    m_ptr_t                     buf;    // buffer [page] address
    zensize_t                   ofs;    // buffer offset
    uint32_t                    flg;    // object/descriptor flags
};

struct zentasktab {
    struct zentask             *tab;
    mttktlk                     tkt;
};

struct zennice {
    int_fast8_t                 nice;
    int_fast8_t                 slice;
};

#endif /* ZEN_TYPES_H */

