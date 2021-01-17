#ifndef DECK_LIB_HT_H
#define DECK_LIB_HT_H

#include <stddef.h>
#include <deck/lib/asm.h>
#include <deck/lib/hash.h>

#if !defined(DECK_HT_SLOTS)
#   define DECK_HT_SLOTS            1024    // must be power of two
#endif

#define deckcalchtkey(u)            (tmhash32(u) & (DECK_HT_SLOTS - 1))

struct deckhtitem {
    uintptr_t                       key;
    uintptr_t                       val;
};

#define DECK_HT_QUEUE_SIZE          (2 * MACH_CL_SIZE)
#define DECK_HT_QUEUE_ITEMS         30
struct deckhtqueue {
    volatile struct deckhtqueue    *prev;
    volatile struct deckhtqueue    *next;
    volatile m_atomic_t             ncur;
    volatile m_atomic_t             nmax;
    struct deckhtitem               items[DECK_HT_QUEUE_ITEMS];
};

#endif /* DECK_LIB_HT_H */

