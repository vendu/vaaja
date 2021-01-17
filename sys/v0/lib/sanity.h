#ifndef DECK_LIB_SANITY_H
#define DECK_LIB_SANITY_H

#include <deck/lib/ht.h>

#if (2 * MACH_POINTER_SIZE + 2 * MACH_ATOMIC_SIZE                       \
     + 30 * (2 * MACH_PTR_SIZE)                                         \
     != DECK_HT_QUEUE_SIZE)
#   error sizeof(struct deckhtqueue) != DECK_HT_QUEUE_SIZE in <deck/lib/sanity.h>
#endif

#endif /* DECK_LIB_SANITY_H */

