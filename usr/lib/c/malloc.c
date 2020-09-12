#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <mem.h>
#include <zero/cdefs.h>
#include "_malloc.h"

void *
malloc(size_t size)
{
    void *ptr = memget(size, 0, NULL);

    return ptr;
}

void *
calloc(size_t n, size_t size)
{
    size_t       blksz = n * size;
    void        *ptr = NULL;

    if ((blksz) && blksz >= size) {
        /* n > 0 && size > 0, n * size didn't overflow */
        ptr = memget(size, 0, NULL);
    }
    memset(ptr, 0, blksz);

    return ptr;
}

void *
realloc(void *ptr, size_t size)
{
    void       *adr;

    if (!ptr) {
        adr = memget(size, 0, NULL);
    } else {
        adr = memresize(ptr, size, 0, 0);
    }

    return ptr;
}

void *
reallocf(void *adr, size_t size)
{
    void       *ptr = memresize(adr, size, 0,
                                MEM_FREE_ON_FAILURE
                                | MEM_ALLOC_ON_NULL
                                | MEM_ERRNO_ON_RESIZE);

    return ptr;
}

void
free(void *ptr)
{
    if (ptr) {
        memput(ptr);
    }

    return;
}

void *
aligned_alloc(size_t align, size_t size)
{
    void       *ptr = memget(size, align, NULL);

    return ptr;
}

int
posix_memalign(void **memptr, size_t align, size_t size)
{
    void       *ptr;

    if (!size
        || (align & (CHAR_BIT * sizeof(void *) - 1))
        || !powerof2(align)) {

        return EINVAL;
    } else {
        ptr = memget(size, align, NULL);
    }
    if (!ptr) {

        return ENOMEM;
    }
    *memptr = ptr;

    return 0;
}

void *
memalign(size_t align, size_t size)
{
    void       *ptr;

    if (!powerof2(align)) {

        return NULL;
    }
    ptr = memget(size, align, NULL);

    return ptr;
}

void *
valloc(size_t size)
{
    void       *ptr = memget(size, MACH_PAGE_SIZE, NULL);

    return ptr;
}

void *
pvalloc(size_t size)
{
    size_t      blksz = roundup2(size, MACH_PAGE_SIZE);
    void       *ptr = memget(blksz, MACH_PAGE_SIZE, NULL);

    return ptr;
}

void *
reallocarray(void *adr, size_t n, size_t size)
{
    size_t      tabsz = n * size;
    void       *ptr;

    if (tabsz < size) {
        errno = ENOMEM;

        return NULL;
    }
    ptr = memresize(adr, tabsz, 0, 0);
    if (!ptr) {
        errno = ENOMEM;
    }

    return ptr;
}

