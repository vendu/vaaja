#ifndef __SYS_SELECT_H__
#define __SYS_SELECT_H__

#if defined(__zen__)
#include <zen/conf.h>
#endif

#if !defined(FD_SETSIZE)
#if defined(ZEN_OPEN_MAX) && !defined(FD_SETSIZE)
#define FD_SETSIZE ZEN_OPEN_MAX
#elif defined(_POSIX_SOURCE)
#define FD_SETSIZE _POSIX_FD_SETSIZE
#elif (USEBSD) && !defined(NFDBITS)
#include <sys/sysmacros.h>
#define FD_SETSIZE NFDBITS
#endif /* !defined(FD_SETSIZE) */

#if !defined(CHAR_BIT)
#include <share/limits.h>
#endif
typedef long       __fd_mask;
#define __NFDBITS               (sizeof(__fd_mask) * CHAR_BIT)
struct fd_set {
#if defined(_XOPEN_SOURCE)
    __fd_mask                   fds_bits[FD_SETSIZE / NFDBITS];
#else
    __fd_mask                   __fds_bits[FD_SETSIZE / NFDBITS];
#endif
};
typedef struct fd_set fd_set;

#if defined(_XOPEN_SOURCE)
#define FD_ZERO(set)            memset(set->fds_bits,                   \
                                       0,                               \
                                       sizeof(struct fd_set))
#define FD_SET(fd, set)         setbit(set->fds_bits, fd)
#define FD_CLR(fd, set)         clrbit(set->fds_bits, fd)
#define FD_ISSET(fd, set)       bitset(set->fds_bits, fd)
#else /* !defined(_XOPEN_SOURCE) */
#define FD_ZERO(set)            memset(set->__fds_bits,                 \
                                       0,                               \
                                       sizeof(struct fd_set))
#define FD_SET(fd, set)         setbit(set->__fds_bits, fd)
#define FD_CLR(fd, set)         clrbit(set->__fds_bits, fd)
#define FD_ISSET(fd, set)       bitset(set->__fds_bits, fd)
#endif
#if defined(_BSD_SOURCE)
#define FD_COPY(src, dest) memcpy(dest, src, sizeof(struct fd_set))
#endif

#endif /* !defined(FD_SETSIZE) */

#endif

