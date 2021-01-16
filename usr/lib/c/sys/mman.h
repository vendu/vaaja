#ifndef __SYS_MMAN_H__
#define __SYS_MMAN_H__

#include <features.h>
#include <stddef.h>
#include <sys/types.h>
#if (_ZERO_SOURCE)
#include <kern/syscall.h>
#endif

#define MAP_FAILED            ((void *)-1L)
/* protection */
#define PROT_NONE             0x00              // cannot be accessed
#define PROT_EXEC             0x01              // can be executed
#define PROT_WRITE            0x02              // can be written
#define PROT_READ             0x04              // can be read

#if (_BSD_SOURCE)
/* madvise() */
#define MADV_NORMAL           MEM_NORMAL
#define MADV_RANDOM           MEM_RANDOM
#define MADV_SEQUENTIAL       MEM_SEQUENTIAL
#define MADV_WILLNEED         MEM_WILLNEED
#define MADV_DONTNEED         MEM_DONTNEED
#define MADV_REMOVE           MEM_REMOVE
#define MADV_DONTFORK         MEM_DONTFORK
#define MADV_DOFORK           MEM_DOFORK
#endif

#if (USEXOPEN2K)
/* posix_madvise() */
#define POSIX_MADV_NORMAL     MEM_NORMAL
#define POSIX_MADV_RANDOM     MEM_RANDOM
#define POSIX_MADV_SEQUENTIAL MEM_SEQUENTIAL
#define POSIX_MADV_WILLNEED   MEM_WILLNEED
#define POSIX_MADV_DONTNEED   MEM_DONTNEED
/* posix_typed_mem_open() */
#define POSIX_TYPED_MEM_ALLOCATE        1
#define POSIX_TYPED_MEM_ALLOCATE_CONTIG 2
#define POSIX_TYPED_MEM_MAP_ALLOCATABLE 3
#endif

#if (_GNU_SOURCE)
typedef int error_t;
#endif

#if !defined(__KERNEL__)

int   mlock(const void *adr, size_t len);
int   munlock(const void *adr, size_t len);
int   mlockall(int flg);
int   munlockall(void);
int   mprotect(void *adr, size_t len, int prot);
void *mmap(void *adr, size_t len, int prot, int flg, int fd, off_t ofs);
int   munmap(void *adr, size_t len);
int   msync(void *adr, size_t len, int flg);
#if (_BSD_SOURCE)
int   madvise(void *adr, size_t len, int advice);
#endif
/* FIXME: what version of XOPEN to check for? */
#if (_XOPEN_SOURCE)
int   posix_madvise(void *adr, size_t len, int advice);
#endif
/* FIXME: what systems is mincore() around on? */
int   mincore(void *adr, size_t len, unsigned char *_vec);
#if (_GNU_SOURCE)
void *mremap(void *adr, size_t len, size_t newlen, int flg);
int   remap_file_pages(void *adr, size_t len,
                       int prot, size_t ofs, int flg);
#endif
int   shm_open(const char *name, int flg, mode_t mode);
int   shm_unlink(const char *name);

#endif /* !defined(__KERNEL__) */

#if (USEXOPEN2K)
struct posix_typed_mem_info {
    size_t posix_tmi_length;    // maximum allocation from typed memory object
};
#endif

#endif /* __SYS_MMAN_H__ */

