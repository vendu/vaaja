/*
 * Copyright (C) 2007-2017 Tuomo Petteri Venäläinen. All rights reserved.
 *
 * system-programming utility routines for Unix-like systems
 */

#ifndef __VND_UNIX_H__
#define __VND_UNIX_H__

#include <features.h>
#include <stdint.h>
#include <signal.h>

/* posix standard header. */
#include <unistd.h>
#if defined(_POSIX_SOURCE) && !defined(_SC_OPEN_MAX)
#include <sys/resource.h>
#endif

/* i/o headers. */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SBRK_FAILED     ((void *)-1L)

#if !defined(MMAP_DEV_ZERO) || defined(MAP_ANON) || defined(MAP_ANONYMOUS)
#define MMAP_DEV_ZERO   0 /* set mmap to use /dev/zero; else, use MAP_ANON */
#endif

/* some systems may need MAP_FILE with MAP_ANON or MAP_ANONYMOUS. */
#ifndef MAP_FILE
#define MAP_FILE        0
#endif
#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#define MAP_ANON        MAP_ANONYMOUS
#endif
#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS   MAP_ANON
#endif
#if !defined(MAP_FAILED)
#define MAP_FAILED      ((void *)-1)
#endif
#if (defined(MMAP_DEV_ZERO) && (MMAP_DEV_ZERO))
#define mapanon(fd, size, flg)                                          \
    mmap(NULL, size, PROT_READ | PROT_WRITE,                            \
         MAP_PRIVATE | MAP_FILE | (flg),                                \
         fd,                                                            \
         0)
#else
#define mapanon(fd, size, flg)                                          \
    mmap(NULL,                                                          \
         size,                                                          \
         PROT_READ | PROT_WRITE,                                        \
         MAP_PRIVATE | MAP_ANON | MAP_FILE | (flg),                     \
         -1,                                                            \
         0)
#endif
#define unmapanon(ptr, size)    munmap(ptr, size)

//extern void * sbrk(intptr_t delta);

#define growheap(ofs) sbrk(ofs)

#define openrd(fname) open(fname, O_RDONLY)
#define openrdwr(fname) open(fname, O_RDWR)
#define openexec(fname) open(fname, O_RDONLY | O_EXEC)

void * readfile(char *filename, size_t *sizeret);

#if !defined(_SC_PAGESIZE)
#define _SC_PAGESIZE _SC_PAGE_SIZE
#endif

#if defined(_SC_PAGESIZE)
#define get_page_size() (sysconf(_SC_PAGESIZE)
#else
#define get_page_size() getpagesize()
#endif
#if defined(_SC_OPEN_MAX)
#define get_open_max()  (sysconf(_SC_OPEN_MAX))
#elif defined(RLIMIT_NOFILE)
static inline int
get_open_max(void)
{
    struct rlimit rlimit;
    int           retval = -1;

    if (!getrlimit(RLIMIT_NOFILE, &rlimit)
        && rlimit.rlim_cur != RLIM_INFINITY) {
        retval = rlimit.rlim_cur;
    }

    return retval;
}
#else
#define get_open_max() getdtablesize()
#endif

#endif /* __VND_UNIX_H__ */

