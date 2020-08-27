#ifndef __SYS_ZEN_TYPES_H__
#define __SYS_ZEN_TYPES_H__

#include <stdint.h>
//#include <mach/types.h>
#include <mt/tktlk.h>
#include <sys/zen/bsp/v0.h>

/* sized types */
typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef uint32_t        zenblk_t;       // block ID
typedef uint16_t        zenbus_t;       // bus ID
typedef uint16_t        zendevid_t;       // device ID
typedef int32_t         zenpid_t;       // process ID
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID
typedef int32_t         zenmode_t;      // file access mode
typedef uint32_t        zenperm_t;      // I/O permission flags
typedef int32_t         zentime_t;      // FIXME: wraps around in 2038 :)
/* system types */
typedef m_ptr_t         zenptr_t;       // void * or char * (compiler-type)
typedef m_size_t        zensize_t;      // size for memory regions
typedef m_ssize_t       zenssize_t;     // signed size for I/O
typedef m_off_t         zenoff_t;       // file-system offset
/* hardware types */
typedef m_page_t        zenpage_t;      // page-table/directory entry

struct zencred {
    zenuid_t            uid;
    zengid_t            gid;
};

struct zendev {
    zenptr_t            vfsfuncs;
    zendev_t            dev;        // 16-bit major + 16-bit minor device IDs
    zenbus_t            bus;
    zendevid_t          id;;
    uint32_t            flg;
};

struct zencpu {
    zentime_t           ntick;
};

#define MEM_NULL_FLAGS          0
#define MEM_CODE_FLAGS          (ZEN_MEM_EXEC | ZEN_MEM_READ)
#define MEM_RODATA_FLAGS        ZEN_MEM_READ
#define MEM_DATA_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_ZERO)
#define MEM_HEAP_FLAGS          (ZEN_MEM_WRITE | ZEN_MEM_READ | ZEN_MEM_DYNAMIC)
#define MEM_USRSTK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ           \
                                 | ZEN_MEM_GROW_DOWN)
#define MEM_SYS_FLAGS           (ZEN_MEM_EXEC | ZEN_MEM_READ | ZEN_MEM_SYS)
#define MEM_SYSSTK_FLAGS        (ZEN_MEM_WRITE | ZEN_MEM_READ           \
                                 | ZEN_MEM_GROW_DOWN | ZEN_MEM_SYS)
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
    zenptr_t            base;
    zenptr_t            lim;
    zensize_t           size;
    uint32_t            flags;
};

struct zenmap {
    struct zencred      cred;
    zenptr_t            adr;
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
    zenptr_t            buf;    // buffer [page] address
    zensize_t           ofs;    // buffer offset
    uint32_t            flg;    // object/descriptor flags
};

struct zennice {
    int_fast8_t         nice;
    int_fast8_t         slice;
};

struct zentasktab {
    struct zentask     *tab;
    mttktlk             tkt;
};

#endif /* __SYS_ZEN_TYPES_H__ */

