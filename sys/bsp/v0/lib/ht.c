#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <v0/lib/cdefs.h>
#include <v0/lib/mach.h>
#include <v0/lib/bitmagic.h>
#include <v0/lib/lk.h>
#include <v0/lib/ht.h>

struct v0htqueue              **v0httab;

static void *
v0initht(long nslot)
{
    void                       *ptr;

    if (nslot <= MACH_PAGE_SIZE / CHAR_BIT) {
        nslot = MACH_PAGE_SIZE / CHAR_BIT;
    }
    ptr = calloc(nslot, sizeof(struct v0htqueue));
    if (!ptr) {
        fprintf(stderr, "%s: %s failed to allocated %ld bytes\n",
                __FILE__, __func__, nslot * sizeof(struct v0htqueue));

        exit(1);
    }

    return ptr;
}

#define V0_HT_REMOVE            (-1L)
#define V0_HT_SEARCH            (0L)
#define V0_HT_ADD               (1L)

#define v0freeht(ptr)           free((void *)(ptr))
long
v0htfind(struct v0htqueue ** C_RESTRICT tab,
           uintptr_t val,
           long op)
{
    uint32_t                        key = tmhash32(val);
    volatile struct v0htitem       *item;
    volatile struct v0htqueue      *head = NULL;
    volatile struct v0htqueue      *prev = NULL;
    volatile struct v0htqueue      *next = NULL;
    volatile struct v0htqueue      *ptr = tab[key];
    uintptr_t                       mask;
    size_t                          n;
    size_t                          nleft;

    m_lkbit((volatile m_atomic_t *)&tab[key], M_ADR_LK_BIT_OFS);
    ptr = (void *)((uintptr_t)tab[key] & ~M_ADR_LK_BIT_OFS);
    while (ptr) {
        nleft = ptr->ncur;
        while (nleft) {
            n = min(nleft, 8);
            nleft -= n;
            switch (n) {
                case 8:
                    item = &ptr->items[7];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 7:
                    item = &ptr->items[6];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 6:
                    item = &ptr->items[5];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 5:
                    item = &ptr->items[4];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 4:
                    item = &ptr->items[3];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 3:
                    item = &ptr->items[2];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 2:
                    item = &ptr->items[1];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 1:
                    item = &ptr->items[0];
                    mask = -!!(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 0:

                    break;
            }
            if (val) {
                if (op == V0_HT_SEARCH || V0_HT_ADD) {
                    m_unlkbit((volatile m_atomic_t *)&tab[key],
                              M_ADR_LK_BIT_OFS);

                    return val;
                } else {
                    /* op == V0_HT_REMOVE */
                    n = (struct v0htitem *)val - &ptr->items[0];
                    if (n == 1) {
                        prev = ptr->prev;
                        next = ptr->next;
                        if (prev) {
                            prev->next = ptr->next;
                        } else {
                            head = prev->next;
                        }
                        if (next) {
                            next->prev = prev;
                        }
                        v0freeht(ptr);
                    } if (n < val - 1) {
                        val = tab[key]->items[n].val;
                        memcpy(&tab[key]->items[n],
                               &tab[key]->items[n + 1], n - val - 1);
                        n--;
                        ptr->ncur = n;
                    }
                    m_atomwrite((volatile m_atomic_t *)&tab[key],
                                (volatile m_atomic_t *)head);

                    return val;
                }
            }
        }
        ptr = ptr->next;
    }
    m_unlkbit((volatile m_atomic_t *)&tab[key], M_ADR_LK_BIT_OFS);

    return 0;
}

long
v0htadd(struct v0htqueue ** C_RESTRICT tab,
          uintptr_t val)
{
    uint32_t                        key = tmhash32(val);
    volatile struct v0htqueue      *ptr;
    volatile struct v0htqueue      *ptr1;
    long                            ncur;

    if (!v0httab) {
        v0httab = v0initht(V0_HT_SLOTS);
    }
    m_lkbit((volatile m_atomic_t *)&tab[key], M_ADR_LK_BIT_OFS);
    ptr1 = (void *)((uintptr_t)tab[key] & ~M_ADR_LK_BIT_OFS);
    if (!ptr1) {
        ptr1 = calloc(V0_HT_SLOTS, sizeof(struct v0htqueue *));
        if (!ptr1) {
            fprintf(stderr, "%s: %s failed to allocated %u bytes\n",
                    __FILE__,
                    __func__,
                    V0_HT_SLOTS * sizeof(struct v0htqueue *));

            exit(1);
        }
        ptr1->ncur = 1;
        ptr1->nmax = V0_HT_QUEUE_ITEMS;
    } else {
        while ((ptr1->next) && ptr1->next->ncur == V0_HT_QUEUE_ITEMS) {
            ptr1 = ptr1->next;
        }
        if (ptr1->next) {
            ptr = ptr1->next;
        } else {
            ptr = calloc(V0_HT_SLOTS, sizeof(struct v0htqueue *));
        }
        if (!ptr) {
            fprintf(stderr, "%s: %s failed to allocated %u bytes\n",
                    __FILE__,
                    __func__,
                    V0_HT_SLOTS * sizeof(struct v0htqueue *));

            exit(1);
        }
        ncur = ptr->ncur;
        ptr->nmax = V0_HT_QUEUE_ITEMS;
        ncur++;
        ptr->ncur = ncur;
    }
    m_atomwrite((volatile m_atomic_t *)tab[key], ptr1);

    return 1;
}

