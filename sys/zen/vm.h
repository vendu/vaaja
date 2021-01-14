#ifndef ZEN_VM_H
#define ZEN_VM_H

#include <zen/zen.h>

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
    mttktlk                     lk;
    m_atomic_t                  n;
    struct zenvmpage           *head;
    struct zenvmpage           *tail;
};

struct zenvmpage {
    struct zenvmqueue          *queue;
    struct zenvmpage           *prev;
    struct zenvmpage           *next;
    m_adr_t                     ptr;
    m_word_t                    flg;
    m_word_t                    qofs;
};

struct zenvmseg {
    const char                 *name;
    m_adr_t                     ptr;
    m_size_t                    size;
    m_word_t                    flag;
#if defined(__v0__)
    m_word_t                    pad;
#endif
};

#endif /* ZEN_VM_H */

