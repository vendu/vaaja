#ifndef __SYS_ZEN_VM_H__
#define __SYS_ZEN_VM_H__

/* zen kernel virtual memory interface */
#if defined(__v0__)
//#include <zen/bsp/v0.h>
#endif
#include <zero/trix.h>
#include <mach/param.h>
#include <mt/tktlk.h>
#include <sys/zen/types.h>

#if (MACH_WORD_SIZE == 4)
#define zenvmcalcqid    lzero32
#define ZEN_VM_QUEUES   32
#elif (MACH_WORD_SIZE == 8)
#define zenvmcalcqid    lzero64
#define ZEN_VM_QUEUES   64
#endif

#define _zencalcvmqueue(page)   ceilpow2l(page->nflt)
#define _zenvmlkqueue(queue)    mtlktkt(&queue->lk)
#define _zenvmunlkqueue(queue)  mtunlktkt(&queue->lk)
struct zenvmqueue {
    mttktlk             lk;
    m_atomic_t          n;
    struct zenvmpage   *head;
    struct zenvmpage   *tail;
};

struct zenvmpage {
    struct zenvmqueue  *queue;
    struct zenvmpage   *prev;
    struct zenvmpage   *next;
    m_adr_t             ptr;
    m_word_t            flg;
    m_word_t            nflt;
#if defined(__v0__)
    m_word_t            pad;
#endif
};

struct zenvmseg {
    const char         *name;
    int8_t             *ptr;
    m_size_t            size;
    m_word_t            flag;
};

#endif /* __SYS_ZEN_VM_H__ */

