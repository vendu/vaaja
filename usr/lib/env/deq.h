#include <stddef.h>

#undef deqinit
#undef deqisempty
#undef deqissingular

#define deqisemptyptr(deq)    (!(*(deq))->next)
#define deqissingularptr(deq) ((*(deq))->next == (*(deq))->prev)

#if defined(DEQ_SINGLE_TYPE)

#define deqinit(item)           ((item)->next = (item), (item)->prev = (item))
#define deqisempty(item)        (!(item)->next)
#define deqissingular(item)     ((item)->next == (item)->prev)

/* add item to beginning of deq */
static __inline__ void
deqpush(DEQ_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_TYPE *list = *deq;

    if (list) {
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
    } else {
        deqinit(item);
        *deq = item;
    }

    return;
}

/* get/remove item from beginning of deq */
static __inline__ DEQ_TYPE *
deqpop(DEQ_TYPE **deq)
{
    DEQ_TYPE *item = *deq;

    if (item) {
        if (!deqissingular(item)) {
            item->prev->next = item->next;
            item->next->prev = item->prev;
            *deq = item->next;
        } else {
            *deq = NULL;
        }
    }

    return item;
}

/* add item to end of deq */
static __inline__ void
deqappend(DEQ_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_TYPE *list = *deq;

    if (list) {
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
    } else {
        deqinit(item);
        *deq = item;
    }

    return;
}

/* get/remove item from end of deq */
static __inline__ DEQ_TYPE *
deqgetlast(DEQ_TYPE **deq)
{
    DEQ_TYPE *list = *deq;
    DEQ_TYPE *item = NULL;

    if (list) {
        if (!deqissingular(list)) {
            item = list->prev;
            item->prev->next = item->next;
            item->next->prev = item->prev;
            list->prev = item->prev;
        } else {
            item = list;
            *deq = NULL;
        }
    }

    return item;
}

/* remove item from deq */
static __inline__ void
deqrmitem(DEQ_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_TYPE *list = *deq;

    if (!deqissingular(item)) {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        if (item == list) {
            *deq = item->next;
        }
    } else if (item == list) {
        *deq = NULL;
    }
}

#else /* !defined(DEQ_SINGLE_TYPE) */

#define deqinit(item, deq)                                              \
    ((*(deq))->next = (item), (*(deq))->prev = (item))
#define deqisempty(deq)    deqisemptyptr(deq)
#define deqissingular(deq) deqissingularptr(deq)
#if 0
#define deqisempty(deq)    (!(*(deq))->next)
#define deqissingular(deq) ((*(deq))->next == (*(deq))->prev)
#endif

/* add item to beginning of deq */
static __inline__ void
deqpush(DEQ_ITEM_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_ITEM_TYPE *list = NULL;

    if (*deq) {
        list = (*deq)->next;
    }
    if (list) {
        item->prev = (*deq)->prev;
        item->prev->next = item;
        list->prev = item;
        item->next = list;
        (*deq)->next = item;
    } else {
        deqinit(item, deq);
        (*deq)->prev = item;
        (*deq)->next = item;
    }

    return;
}

/* get/remove item from beginning of deq */
static __inline__ DEQ_ITEM_TYPE *
deqpop(DEQ_TYPE **deq)
{
    DEQ_ITEM_TYPE *item = NULL;

    if (*deq) {
        item = (*deq)->next;
    }
    if (item) {
        if (!deqissingular(deq)) {
            (*deq)->prev->next = item->next;
            item->next->prev = (*deq)->prev;
            (*deq)->next = item->next;
        } else {
            (*deq)->prev = NULL;
            (*deq)->next = NULL;
        }
    }

    return item;
}

/* add item to end of deq */
static __inline__ void
deqappend(DEQ_ITEM_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_ITEM_TYPE *list = NULL;

    if (*deq) {
        list = (*deq)->prev;
    }
    if (list) {
        item->prev = list;
        list->next = item;
        item->next = (*deq)->next;
        item->next->prev = item;
        (*deq)->prev = item;
    } else {
        deqinit(item, deq);
        (*deq)->prev = item;
        (*deq)->next = item;
    }

    return;
}

/* get/remove item from end of deq */
static __inline__ DEQ_ITEM_TYPE *
deqgetlast(DEQ_TYPE **deq)
{
    DEQ_ITEM_TYPE *item = NULL;

    if (*deq) {
        item = (*deq)->prev;
    }
    if (item) {
        if (!deqissingular(deq)) {
            item->prev->next = item->next;
            item->next->prev = item->prev;
        } else {
            (*deq)->prev = NULL;
            (*deq)->next = NULL;
        }
    }

    return item;
}

/* remove item from deq */
static __inline__ void
deqrmitem(DEQ_ITEM_TYPE *item, DEQ_TYPE **deq)
{
    DEQ_ITEM_TYPE *list = (*deq)->next;

    if (list) {
        if (!deqissingular(deq)) {
            item->prev->next = item->next;
            item->next->prev = item->prev;
            if (item == list) {
                list = item->next;
            }
        } else {
            (*deq)->prev = NULL;
            (*deq)->next = NULL;
        }
    }

    return;
}

#endif /* DEQ_SINGLE_TYPE */

