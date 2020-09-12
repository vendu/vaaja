#ifndef __SYS_UIO_H__
#define __SYS_UIO_H__

#include <features.h>
#include <sys/types.h>

#if !defined(_POSIX_SOURCE)
#define UIO_MAXIOV 1024
#endif /* !defined(_POSIX_SOURCE) */

struct iovec {
    void         *iov_base;
    size_t        iov_len;
};

#if !defined(__zen__)

extern ssize_t readv(int fd, const struct iovec *iovec, int cnt);
extern ssize_t writev(int fd, const struct iovec *iovec, int cnt);
#if defined(_BSD_SOURCE)
extern ssize_t preadv(int fd, const struct iovec *iovec, int cnt, off_t ofs);
extern ssize_t pwritev(int fd, const struct iovec *iovec, int cnt, off_t ofs);
#endif /* defined(_BSD_SOURCE) */

#endif /* !defined(__zen__) */

#endif /* __SYS_UIO_H__ */

