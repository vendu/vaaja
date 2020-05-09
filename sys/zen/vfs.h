#ifndef __ZEN_VFS_H__
#define __ZEN_VFS_H__

#include <stddef.h>
#include <stdint.h>

/* filesystem types */
#define ZEN_VFS_UNKNOWN_FS      0
#define ZEN_VFS_ZEN_FS0         1
/* virtual filesystem flags */
#define ZEN_VFS_EXEC            (1 << 0)
#define ZEN_VFS_WRITE           (1 << 1)
#define ZEN_VFS_READ            (1 << 2)
#define ZEN_VFS_BUSY            (1 << 29)
#define ZEN_VFS_DIRTY           (1 << 30)
#define ZEN_VFS_MOUNTED         (1 << 31)
/* node types */
#define ZEN_VFS_UNKNOWN_NODE    0
#define ZEN_VFS_CHR_NODE        1
#define ZEN_VFS_BLK_NODE        2
#define ZEN_VFS_FILE_NODE       3
#define ZEN_VFS_DIR_NODE        4
#define ZEN_VFS_FIFO_NODE       5
#define ZEN_VFS_SOCK_NODE       6
#define ZEN_VFS_SYMLINK_NODE    7

typedef intptr_t        vfsword_t;
typedef uintptr_t       vfsuword_t;
typedef uintptr_t       vfssize_t;
typedef intptr_t        vfsoff_t;
typedef intptr_t        vfsdev_t;
typedef uintptr_t       vfsino_t;
typedef uintptr_t       vfsuid_t;
typedef uintptr_t       vfsgid_t;

struct zenvfsnode {
    vfsuword_t                  desc;
    vfsuword_t                  type;
    vfsuword_t                  flags;
    void                       *funcs;
    union {
#if defined(ZEN_FS)
        struct zenfscommon      zenfs;
#endif
    } node;
};

/* FIXME */
struct iovec;
typedef uint32_t DIR;
typedef uint32_t FILE;
typedef size_t socklen_t;
struct sockaddr;
struct msghdr;

struct zenvfsfuncs {
    int       (*open)(const char *path, int flg, ...);
    int       (*close)(int fd);
    ssize_t   (*read)(int fd, void *buf, size_t len);
    ssize_t   (*write)(int fd, void *buf, size_t len);
    off_t     (*seek)(int fd, off_t ofs, int whence);
    int       (*lock)(int fd, int cmd, off_t len);
    void      (*map)(void *adr, size_t len, int prot, int flags,
                     int fd, off_t ofs);
    void      (*unmap)(void *adr, size_t len);
    int       (*chmod)(const char *path, mode_t mode);
    int       (*chown)(const char *path, uid_t owner, gid_t grp);
    int       (*link)(const char *src, const char *dest);
    int       (*symlink)(const char *src, const char *dest);
    int       (*unlink)(const char *path);
    int       (*dup2)(int srcfd, int destfd);
    ssize_t   (*readv)(int fd, const struct iovec *iov, int niov);
    ssize_t   (*writev)(int fd, const struct iovec *iov, int niov);
    ssize_t   (*fsync)(int fd);
    int       (*falloc)(int fd, int mode, off_t ofs, off_t len);
    int       (*ftrunc)(int fd, off_t len);
    int       (*fadvise)(int fd, off_t ofs, off_t len, int advice);
};

struct zenvfsdevfuncs {
    int       (*mkdev)(const char *path, mode_t mode, dev_t dev);
    int       (*mount)(const char *devpath, const char *destpath,
                       const char *fstype, unsigned long flags,
                       const void *data);
    int       (*unmount)(const char *path, int flags);
};

struct zenvfsdirfuncs {
    DIR               (*opendir)(const char *path);
    int               (*closedir)(DIR *dir);
    int               (*mkdir)(const char *path,  mode_t mode);
    int               (*rmdir)(const char *path);
    struct dirent     (*readdir)(DIR *dir);
    void              (*seekdir)(DIR *dir, long loc);
    void              (*rewinddir)(DIR *dir);
    long              (*telldir)(DIR *dir);
};

struct zenvfsfifofuncs {
    FILE     (**popen)(const char *cmd, const char *type);
    int       (*pclose)(FILE *fp);
    ssize_t   (*pread)(int fd, void *buf, size_t len, off_t ofs);
    ssize_t   (*pwrite)(int fd, void *buf, size_t len, off_t ofs);
};

struct zenvfssockfuncs {
    int       (*socket)(int domain, int type, int protocol);
    int       (*bind)(int sockfd, const struct sockaddr *adr,
                      socklen_t adrlen);
    int       (*listen)(int sockfd, int backlog);
    int       (*accept)(int sockfd, struct sockaddr *RESTRICT adr,
                        socklen_t *RESTRICT adrlen);
    int       (*connect)(int sockfd, const struct sockaddr *adr,
                         socklen_t adrlen);
    int       (*getsockname)(int sockfd, struct sockaddr *RESTRICT adr,
                             socklen_t *RESTRICT adrlen);
    int       (*getsockopt)(int sockfd, int level, int optname,
                            void *RESTRICT optval, socklen_t *RESTRICT optlen);
    int       (*getpeername)(int sockfd, struct sockaddr *RESTRICT adr,
                             socklen_t *RESTRICT adrlen);
    ssize_t   (*recv)(int sockfd, void *buf, size_t len, int flags);
    ssize_t   (*recvfrom)(int sockfd, void *RESTRICT buf, size_t len, int flags,
                          struct sockaddr *RESTRICT adr,
                          socklen_t *RESTRICT adrlen);
    ssize_t   (*recvmsg)(int sockfd, struct msghdr *msg, int flags);
    ssize_t   (*send)(int sockfd, const void *buf, size_t len, int flags);
    int       (*shutdown)(int sockfd, int how);
};

struct zenvfsshmfuncs {
    int       (*shmopen)(const char *path, int oflag, mode_t mode);
    int       (*shmunlink)(const char *path);
    void      (*shmat)(int shmfd, const void *shmadr, int flags);
};

#endif /* __ZEN_VFS_H__ */

