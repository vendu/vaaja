#ifndef __MT_LFDEQ_H__
#define __MT_LFDEQ_H__

#include <stdint.h>
#include <string.h>
#include <mach/atomic.h>
#include <mach/asm.h>

#define LFDEQ_MAX_ITEMS      32768
#define LFDEQ_STABLE         0
#define LFDEQ_PUSH_BIT       (1U << 15)
#define LFDEQ_ADR_LK_BIT_POS 0

struct lfdeqitem {
    struct lfdeqitem *prev;
    struct lfdeqitem *next;
    union {
        int32_t       i32;
        struct {
            int16_t   left;
            int16_t   right;
        } link;
    } data;
    int32_t           _pad;
};

#define lfdeqislpush(item)  ((item)->data.link.left & LFDEQ_PUSH_BIT)
#define lfdeqisrpush(item)  ((item)->data.link.right & LFDEQ_PUSH_BIT)
#define lfdeqisstable(item) (!lfdeqislpush(item) && !lfdeqisrpush(item))
struct lfdeq {
    volatile union {
        int32_t       i32;
        struct {
            int16_t   left;
            int16_t   right;
        } link;
    } data;
};

extern struct lfdeqitem *lfdeqbase;
extern struct lfdeq      lfdeq;

int16_t lfdeqallocitem(void);

#define lfdeqsetdata(item, data, len) memcpy(item, data, len)

static __inline__ void
lfdeqfixright(struct lfdeq *deq, struct lfdeqitem *item)
{
    struct lfdeqitem *node;
    struct lfdeqitem *prev;
    struct lfdeqitem *next;
    int16_t           left;
    int16_t           right;

    right = item->data.link.right;
    node->data.i32 = item->data.i32;
    if (deq->data.i32 != item->data.i32) {

        return;
    }
    next = &lfdeqbase[right];
    node->data.link.right |= LFDEQ_PUSH_BIT;
    left = next->data.link.left;
    prev = &lfdeqbase[left];
    if (deq->data.i32 != node->data.i32) {

        return;
    }
    right = prev->next->data.link.right;
    prev->next->data.link.right |= LFDEQ_PUSH_BIT;
    if (prev->next != next) {
        if (deq->data.i32 != item->data.i32) {

            return;
        }
        if (m_cmpswap32((m_atomic32_t *)&prev->data.i32,
                        (m_atomic32_t)prev->next->data.i32,
                        (m_atomic32_t)prev->next->data.i32 | LFDEQ_PUSH_BIT)) {

            return;
        }
    }
}

static __inline__ void
lfdeqfix(struct lfdeq *deq, struct lfdeqitem *item)
{
    if (lfdeqisrpush(item)) {
        lfdeqfixright(deq, item);
        //    } else if (lfdeqislpush(item)) {
        //        lfdeqfixleft(item);
    }

    return;
}

static __inline__ void
lfdeqpushtail(struct lfdeq *deq, void *data, size_t len)
{
    struct lfdeqitem  head;
    int16_t           slot = lfdeqallocitem();
    struct lfdeqitem *item = &lfdeqbase[slot];

    lfdeqsetdata(item, data, len);
    do {
        head.data.i32 = deq->data.i32;
        if (!item->data.link.right) {
            if (m_cmpswap32((m_atomic32_t *)&deq->data.i32,
                            (m_atomic32_t)head.data.i32,
                            (m_atomic32_t)item->data.i32)) {

                return;
            }
        } else if (lfdeqisstable(&lfdeq)) {
            item->data.link.left = deq->data.link.right;
            item->data.link.right |= LFDEQ_PUSH_BIT;
            if (m_cmpswap32((m_atomic32_t *)&deq->data.i32,
                            (m_atomic32_t)head.data.i32,
                            (m_atomic32_t)item->data.i32)) {
                lfdeqfixright(deq, item);

                return;
            }
        } else {
            lfdeqfix(deq, &head);
        }
    } while (1);

    return;
}

#endif /* __MT_LFDEQ_H__ */

