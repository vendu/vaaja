#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <features.h>
#include <env/cdefs.h>
#include <stddef.h>
#include <sys/wait.h>
#if defined(_MISC_SOURCE)
#include <alloca.h>
#endif

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define RAND_MAX        0x00ffffff  // 16 M
#define MB_CUR_MAX      1

#if !defined(NULL)
#define NULL            ((void *)0)
#endif

#if !defined(__size_t_defined)
typedef uintptr_t       size_t;
#endif

typedef struct {
    int                 quot;
    int                 rem;
} div_t;

typedef struct {
    long                quot;
    long                rem;
} ldiv_t;

typedef struct {
    long long           quot;
    long long           rem;
} lldiv_t;

void  * malloc(size_t size);
void  * calloc(size_t n, size_t size);
void  * realloc(void *ptr, size_t size);
#if defined(_MISC_SOURCE)
void  * reallocarray(void *ptr, size_t n, size_t size);
#endif
void    free(void *ptr);

#if defined(_ISOC11_SOURCE)
void  * aligned_alloc(size_t align, size_t size);
#endif
#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
int     posix_memalign(void **memptr, size_t align, size_t size);
#endif
#if ((defined(_XOPEN_SOURCE_EXTENDED) && !defined(_USE_XOPEN2K))        \
     || defined(_MISC_SOURCE))
void  * valloc(size_t size);
#endif

int     atexit(void (*func)(void)) C_NONNULL(1);
#if defined(_MISC_SOURCE)
int     on_exit(void (*func)(int, void *), void *arg) C_NONNULL(1);
#endif
int     at_quick_exit(void (*func)(void)) C_NONNULL(1);

C_NORETURN void abort(void);
C_NORETURN void _exit(int status);
#if (defined(_POSIX_SOURCE)                                             \
     && (_XOPEN_SOURCE >= 600 || ISOC99_SOURCE || _POSIX_C_SOURCE >= 200112L))
#define _Exit(status) _exit(status)
#endif
C_NORETURN void exit(int status);

#if defined(_ISOC11_SOURCE)
int     at_quick_exit(void (*func)(void));
C_NORETURN void quick_exit(int status);
#endif

#endif /* __STDLIB_H__ */

