#ifndef __FCNTL_H__
#define __FCNTL_H__

#include <features.h>
#include <sys/types.h>
#if (_XOPEN_SOURCE)
#include <sys/stat.h>
#endif
#if !defined(__off_t_defined)
#include <share/off_t.h>
#endif
#include <sys/uio.h>
#if (_XOPEN_SOURCE)
//#include <kern/io.h>
#endif
#include <bits/fcntl.h>

#if !defined(F_LOCK) && defined(_XOPEN_SOURCE_EXTENDED) && !defined(_POSIX_SOURCE)
int       lockf(int fd, int cmd, off_t len);
#endif

int       open(const char *file, int, ...);
int       creat(const char *file, mode_t mode);
int       fcntl(int fd, int cmd, ...);
#if (_XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L || defined(_USE_XOPEN2K))
int       posix_fallocate(int fd, off_t ofs, off_t len);
#endif
#if (_POSIX_C_SOURCE >= 200112L) || (USEXOPEN2K)
int       posix_fadvise(int fd, off_t ofs, size_t len, int advice);
#endif
#if defined(_GNU_SOURCE)
ssize_t   readahead(int fd, off64_t ofs, size_t count);
int       sync_file_range(int fd, off64_t from, off64_t to,
                          unsigned int flags);
int       vmsplice(int outfd, const struct iovec *iov, size_t cnt,
                   unsigned int flags);
int       splice(int infd, off64_t inofs, int outfd, off64_t outofs,
                 size_t len, unsigned int flags);
int       tee(int infd, int outfd, size_t len, unsigned int flags);
#endif

#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
int       openat(int fd, const char *file, int flg);
#endif

#if defined(_BSD_SOURCE)
int	  openx_np(const char *, int, filesec_t);
int	  flock(int, int);
filesec_t filesec_init(void);
filesec_t filesec_dup(filesec_t);
void	  filesec_free(filesec_t);
int	  filesec_get_property(filesec_t, filesec_property_t, void *);
int	  filesec_query_property(filesec_t, filesec_property_t, int *);
int	  filesec_set_property(filesec_t, filesec_property_t,
                               const void *);
int	  filesec_unset_property(filesec_t, filesec_property_t);
#endif

#endif /* __FCNTL_H__ */

