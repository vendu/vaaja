#ifndef __KERN_MEM_SLAB_H__
#define __KERN_MEM_SLAB_H__

#include <stdint.h>
#include <limits.h>

#define MEMSLABSIZE             (2 * 1024 * 1024)

struct memslab {
    void                       *adr;
    m_size_t                    info;
    m_size_t                    size;
    struct memslab             *prev;
    struct memslab             *next;
};

#define memgethdrnum(hdr, zone)                                         \
    ((uintptr_t)(hdr) - (uintptr_t)(zone)->hdrtab)
#define memgetblknum(ptr, zone)                                         \
    (((uintptr_t)(ptr) - (zone)->base) >> MEMSLABSHIFT)
#define memgetadr(hdr, zone)                                            \
    ((void *)((uint8_t *)(zone)->base                                   \
              + (memgethdrnum(hdr, zone) << MEMMINSHIFT)))
#define memgetslab(ptr, zone)                                           \
    ((struct memslab *)(zone)->hdrtab + memgetblknum(ptr, zone))
#define memgetmag(ptr, zone)                                            \
    ((struct memmag *)(zone)->hdrtab + memgetblknum(ptr, zone))

#endif /* __KERN_MEM_SLAB_H__ */

