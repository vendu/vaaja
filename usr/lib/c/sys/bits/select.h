#ifndef __SYS_BITS_SELECT_H__
#define __SYS_BITS_SELECT_H__

#if !defined(__struct_timeval_defined)
#include <sys/bits/timeval.h>
#endif

#if defined(__zen__)
#include <zen/conf.h>
#endif
#if !defined(CHAR_BIT)
#include <share/limits.h>
#endif

#if !defined(FD_SETSIZE)
#if (defined(ZEN_OPEN_MAX) && !defined(FD_SETSIZE))
#define FD_SETSIZE ZEN_OPEN_MAX
#elif defined(_POSIX_SOURCE)
#define FD_SETSIZE _POSIX_FD_SETSIZE
#elif (USEBSD) && !defined(NFDBITS)
#include <sys/sysmacros.h>
#define FD_SETSIZE NFDBITS
#endif /* !defined(FD_SETSIZE) */

typedef long       fd_mask;
#define NFDBITS    (sizeof(fd_mask) * CHAR_BIT)
struct fd_set {
#if (USEXOPEN)
    fd_mask fds_bits[FD_SETSIZE / NFDBITS];
#else
    fd_mask __fds_bits[FD_SETSIZE / NFDBITS];
#endif
};
typedef struct fd_set fd_set;

#define FD_SET(fd, set)    setbit(set->fd_bits, fd)
#define FD_CLR(fd, set)    clrbit(set->fd_bits, fd)
#define FD_ISSET(fd, set)  bitset(set->fd_bits, fd)
#define FD_ZERO(set)       memset(set->fd_bits, 0, FD_SETSIZE / CHAR_BIT)
#if (USEBSD)
#define FD_COPY(src, dest) memcpy(dest, src, sizeof(fd_set))
#endif

#endif /* !defined(FD_SETSIZE) */

#endif

#endif /* __SYS_BITS_SELECTf_H__ */

