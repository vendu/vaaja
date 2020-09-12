#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <features.h>
#include <stddef.h>

#if defined(_ISOC11_SOURCE)
void  * aligned_alloc(size_t align, size_t size);
#endif
#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
int     posix_memalign(void **memptr, size_t align, size_t size);
#endif
#if (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)                   \
     && !(_POSIX_C_SOURCE >= 200112L))
void  * valloc(size_t size);
#endif

#endif /* __STDLIB_H__ */

