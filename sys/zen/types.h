#ifndef ZEN_TYPES_H
#define ZEN_TYPES_H

#include <stdint.h>
#include <api/zen/types.h>

typedef uint32_t                zenintrdesc_t;
typedef uint32_t                zenpagedesc_t;

struct zencred {
    zenuid_t                    uid;
    zengid_t                    gid;
};

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

#define MEM_NULL_FLAGS          0
#define MEM_CODE_FLAGS          (K_MEM_EXEC | K_MEM_READ)
#define MEM_RODATA_FLAGS        K_MEM_READ
#define MEM_DATA_FLAGS          (K_MEM_WRITE | K_MEM_READ | K_MEM_ZERO)
#define MEM_HEAP_FLAGS          (K_MEM_WRITE | K_MEM_READ | K_MEM_DYNAMIC)
#define MEM_USRSTK_FLAGS        (K_MEM_WRITE | K_MEM_READ           \
                                 | K_MEM_GROW_DOWN)
#define MEM_SYS_FLAGS           (K_MEM_EXEC | K_MEM_READ | K_MEM_SYS)
#define MEM_SYSSTK_FLAGS        (K_MEM_WRITE | K_MEM_READ           \
                                 | K_MEM_GROW_DOWN | K_MEM_SYS)
#if 0
#define ZEN_NULL_SEG            0
#define ZEN_CODE_SEG            1
#define ZEN_RODATA_SEG          2
#define ZEN_DATA_SEG            3
#define ZEN_HEAP_SEG            4
#define ZEN_USRSTK_SEG          5
#define ZEN_SYS_SEG             6
#define ZEN_SYSSTK_SEG          7
#define ZEN_MEM_DYNAMIC         ZEN_MEM_FLAG
#define ZEN_MEM_ZERO            (ZEN_MEM_FLAG << 1)
struct zenseg {
    m_ptr_t             base;
    m_ptr_t             lim;
    zensize_t           size;
    uint32_t            flags;
};

struct zenmap {
    struct zencred      cred;
    m_ptr_t             adr;
    zensize_t           size;
    uint32_t            flags;
};

struct zenpage {
    struct zenpage     *prev;   // previous in queue
    struct zenpage     *next;   // next in queue
    zenpage_t          *pte;    // pointer to page-table entry
    zenblk_t            blk;    // page-device block ID
    zendev_t            dev;    // page-device
    uint32_t            flags;  // page-flags
    uint32_t            nflt;   // # of page-faults
    uint32_t            qid;    // lruq, sizeof(uint32_t) * CHAR_BIT - clz(nflt)
};

struct zendesc {
    m_ptr_t             buf;    // buffer [page] address
    zensize_t           ofs;    // buffer offset
    uint32_t            flg;    // object/descriptor flags
};

struct zentasktab {
    struct zentask     *tab;
    mttktlk             tkt;
};

#endif

struct zennice {
    int_fast8_t         nice;
    int_fast8_t         slice;
};

#endif /* ZEN_TYPES_H */

