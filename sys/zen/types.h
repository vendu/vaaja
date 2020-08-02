#ifndef __SYS_ZEN_TYPES_H__
#define __SYS_ZEN_TYPES_H__

#include <sys/zen/conf.h>
//#include <stddef.h>
#include <stdint.h>
#include <mt/tktlk.h>
#include <mach/types.h>

typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef int64_t         zenoff_t;       // file-system offset
typedef int32_t         zenpid_t;       // process ID
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID
typedef uint32_t        zenperm_t;      // I/O permission flags

struct zencred {
    zenuid_t            uid;
    zengid_t            gid;
};

struct zendev {
    m_adr_t     vfsfuncs;
    m_word_t    dev;    // 16-bit major + 16-bit minor device IDs
    m_uhalf_t   bus;
    m_uhalf_t   flg;
};

struct zencpu {
    m_time_t            ntick;
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
    m_adr_t             base;
    m_adr_t             lim;
    m_uword_t           size;
    m_uword_t           flags;
};

struct zenmap {
    m_adr_t             adr;
    m_uword_t           size;
    m_uword_t           flags;
    struct zencred      cred;
};

struct zenpage {
    m_uword_t           blk;    // page-device block ID
    zendev_t            dev;    // page-device
    m_adr_t            *pte;    // pointer to page-table entry
    m_uword_t           flags;  // page-flags
    m_uword_t           nflt;   // # of page-faults
    m_uword_t           qid;    // lruq, sizeof(m_word_t) * CHAR_BIT - clz(nflt)
#if defined(__v0__)
    m_word_t            pad;
#endif
    struct zenpage     *prev;   // previous in queue
    struct zenpage     *next;   // next in queue
};

struct zendesc {
    m_adr_t    buf;             // buffer [page] address
    m_adr_t    ofs;             // buffer offset
    uint32_t   flg;
};

struct zennice {
    int_fast8_t         nice;
    int_fast8_t         slice;
};

struct zentasktab {
    struct zentask     *tab;
    mttktlk             tkt;
#if defined(__v0__)
    m_word_t            pad;
#endif
};

#endif /* __SYS_ZEN_TYPES_H__ */

