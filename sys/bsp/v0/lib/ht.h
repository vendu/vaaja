#ifndef DECK_LIB_HT_H
#define DECK_LIB_HT_H

#include <stddef.h>
#include <v0/lib/asm.h>
#include <v0/lib/hash.h>

#if !defined(V0_HT_SLOTS)
#   define V0_HT_SLOTS              1024    // must be power of two
#endif

#define v0calchtkey(u)              (tmhash32(u) & (V0_HT_SLOTS - 1))

struct v0htitem {
    uintptr_t                       key;
    uintptr_t                       val;
};

#define V0_HT_QUEUE_SIZE            (2 * MACH_CL_SIZE)
#define V0_HT_QUEUE_ITEMS           30
struct v0htqueue {
    volatile struct v0htqueue      *prev;
    volatile struct v0htqueue      *next;
    volatile m_atomic_t             ncur;
    volatile m_atomic_t             nmax;
    struct v0htitem                 items[DECK_HT_QUEUE_ITEMS];
};

#endif /* DECK_LIB_HT_H */

