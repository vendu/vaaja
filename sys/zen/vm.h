#ifndef SYS_ZEN_VM_H
#define SYS_ZEN_VM_H

#include <mach/param.h>
#include <zen/bitbang.h>

#if (MACH_WORD_SIZE == 4)
#define zenvmqueueid            m_ctz32
#define ZEN_VM_QUEUES           32
#elif (MACH_WORD_SIZE == 8)
#define zenvmqueueid            m_ctz64
#define ZEN_VM_QUEUES           64
#endif

/* FIXME: these should probably not use underscore-prefixes */
#define _zencalcvmqueue(page)   ceilpow2_32(page->qcnt)
#define _zenvmlkqueue(queue)    mtlktkt(&queue->lk)
#define _zenvmunlkqueue(queue)  mtunlktkt(&queue->lk)

struct zenvmqueue {
    mttkt                       lk;
    m_atomic_t                  n;
    struct zenvmpage           *head;
    struct zenvmpage           *tail;
};

struct zenvmseg {
    const char                 *name;
    m_adr_t                     ptr;
    m_size_t                    size;
    m_word_t                    flag;
};

#define MEM_NULL_FLAGS          0
#define MEM_CODE_FLAGS          (MEM_EXEC | MEM_READ)
#define MEM_RODATA_FLAGS        MEM_READ
#define MEM_DATA_FLAGS          (MEM_WRITE | MEM_READ | MEM_ZERO)
#define MEM_HEAP_FLAGS          (MEM_WRITE | MEM_READ | MEM_DYNAMIC)
#define MEM_USRSTK_FLAGS        (MEM_WRITE | MEM_READ           \
                                 | MEM_GROW_DOWN)
#define MEM_SYS_FLAGS           (MEM_EXEC | MEM_READ | MEM_SYS)
#define MEM_SYSSTK_FLAGS        (MEM_WRITE | MEM_READ           \
                                 | MEM_GROW_DOWN | MEM_SYS)
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
    struct zenpage     *prev;
    struct zenpage     *next;
    zenpage_t          *pte;    // pointer to page-table entry
    zenblk_t            blk;    // page-device block ID
    zendev_t            dev;    // page-device
    uint32_t            flags;  // page-flags
    uint32_t            nflt;   // # of page-faults
    uint32_t            qid;    // lruq, sizeof(uint32_t) * CHAR_BIT - clz(nflt)
};

#endif /* SYS_ZEN_VM_H */

