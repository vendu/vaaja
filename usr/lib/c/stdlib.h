#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <features.h>
#include <stddef.h>
#if defined(_MISC_SOURCE)
#include <alloca.h>
#endif

void  * malloc(size_t size) C_MALLOC C_ALLOC_SIZE(1);
void  * calloc(size_t n, size_t size) C_ALLOC_SIZE((1, 2));
void  * realloc(void *ptr, size_t size) C_WARN_UNUSED_RESULT C_ALLOC_SIZE(2);
#if defined(_MISC_SOURCE)
void  * reallocarray(void *ptr, size_t n, size_t size)
    C_WARN_UNUSED_RESULT C_ALLOC_SIZE(2);
void    free(void *ptr);

#if defined(_ISOC11_SOURCE)
void  * aligned_alloc(size_t align, size_t size)
    C_ALLOC_ALIGN(1) C_ALLOC_SIZE(2);
#endif
#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
int     posix_memalign(void **memptr, size_t align, size_t size)
    C_ALLOC_SIZE(2) C_NONNULL(1);
#endif
#if ((defined(_XOPEN_SOURCE_EXTENDED) && !defined(_USE_XOPEN2K))        \
     || defined(_MISC_SOURCE))
void  * valloc(size_t size) C_MALLOC C_ALLOC_SIZE(1);
#endif

C_NORETURN void abort(void);
int     atexit(void (*func)(void)) C_NONNULL(1);
C_NORETURN void exit(int status);

#if defined(_MISC_SOURCE)
int     on_exit(void (*func)(int, void *), void *arg) C_NONNULL(1);
#endif

#if defined(_ISOC11_SOURCE)
int     at_quick_exit(void (*func)(void)) C_NONNULL(1);
C_NORETURN void quick_exit(int status);
#endif

C_NORETURN void exit(int status);
#if defined(_ISOC99_SOURCE)
C_NORETURN void _Exit(int status);
#endif

#endif /* __STDLIB_H__ */

