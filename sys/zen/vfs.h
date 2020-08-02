#ifndef __SYS_ZEN_VFS_H__
#define __SYS_ZEN_VFS_H__

#include <stddef.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/types.h>

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
typedef uintptr_t       vfspid_t;
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
typedef uint32_t _DIR;
typedef uint32_t _FILE;
typedef size_t socklen_t;
struct sockaddr;
struct msghdr;

struct zenvfsfuncs {
    int       (*open)(const char *path, int flg, ...);
    int       (*close)(int fd);
    m_ssize_t (*read)(int fd, void *buf, size_t len);
    m_ssize_t (*write)(int fd, void *buf, size_t len);
    vfsoff_t  (*seek)(int fd, vfsoff_t ofs, int whence);
    int       (*lock)(int fd, int cmd, vfsoff_t len);
    void      (*map)(void *adr, size_t len, int prot, int flags,
                     int fd, vfsoff_t ofs);
    void      (*unmap)(void *adr, size_t len);
    int       (*chmod)(const char *path, m_mode_t mode);
    int       (*chown)(const char *path, vfsuid_t owner, vfsgid_t grp);
    int       (*link)(const char *src, const char *dest);
    int       (*symlink)(const char *src, const char *dest);
    int       (*unlink)(const char *path);
    int       (*dup2)(int srcfd, int destfd);
    m_ssize_t (*readv)(int fd, const struct iovec *iov, int niov);
    m_ssize_t (*writev)(int fd, const struct iovec *iov, int niov);
    m_ssize_t (*fsync)(int fd);
    int       (*falloc)(int fd, int mode, vfsoff_t ofs, vfsoff_t len);
    int       (*ftrunc)(int fd, vfsoff_t len);
    int       (*fadvise)(int fd, vfsoff_t ofs, vfsoff_t len, int advice);
};

struct zenvfsdevfuncs {
    int       (*mkdev)(const char *path, m_mode_t mode, vfsdev_t dev);
    int       (*mount)(const char *devpath, const char *destpath,
                       const char *fstype, unsigned long flags,
                       const void *data);
    int       (*unmount)(const char *path, int flags);
};

struct zenvfsdirfuncs {
    _DIR          (*opendir)(const char *path);
    int           (*closedir)(_DIR *dir);
    int           (*mkdir)(const char *path,  m_mode_t mode);
    int           (*rmdir)(const char *path);
    struct dirent (*readdir)(_DIR *dir);
    void          (*seekdir)(_DIR *dir, long loc);
    void          (*rewinddir)(_DIR *dir);
    long          (*telldir)(_DIR *dir);
};

struct zenvfsfifofuncs {
    _FILE     (**popen)(const char *cmd, const char *type);
    int       (*pclose)(_FILE *fp);
    m_ssize_t (*pread)(int fd, void *buf, size_t len, vfsoff_t ofs);
    m_ssize_t (*pwrite)(int fd, void *buf, size_t len, vfsoff_t ofs);
};

struct zenvfssockfuncs {
    int       (*socket)(int domain, int type, int protocol);
    int       (*bind)(int sockfd, const struct sockaddr *adr,
                      socklen_t adrlen);
    int       (*listen)(int sockfd, int backlog);
    int       (*accept)(int sockfd, struct sockaddr *C_RESTRICT adr,
                        socklen_t *C_RESTRICT adrlen);
    int       (*connect)(int sockfd, const struct sockaddr *adr,
                         socklen_t adrlen);
    int       (*getsockname)(int sockfd, struct sockaddr *C_RESTRICT adr,
                             socklen_t *C_RESTRICT adrlen);
    int       (*getsockopt)(int sockfd, int level, int optname,
                            void *C_RESTRICT optval,
                            socklen_t *C_RESTRICT optlen);
    int       (*getpeername)(int sockfd, struct sockaddr *C_RESTRICT adr,
                             socklen_t *C_RESTRICT adrlen);
    m_ssize_t (*recv)(int sockfd, void *buf, size_t len, int flags);
    m_ssize_t (*recvfrom)(int sockfd, void *C_RESTRICT buf, size_t len,
                          int flags,
                          struct sockaddr *C_RESTRICT adr,
                          socklen_t *C_RESTRICT adrlen);
    m_ssize_t (*recvmsg)(int sockfd, struct msghdr *msg, int flags);
    m_ssize_t (*send)(int sockfd, const void *buf, size_t len, int flags);
    int       (*shutdown)(int sockfd, int how);
};

struct zenvfsshmfuncs {
    int       (*shmopen)(const char *path, int oflag, m_mode_t mode);
    int       (*shmunlink)(const char *path);
    void      (*shmat)(int shmfd, const void *shmadr, int flags);
};

#endif /* __SYS_ZEN_VFS_H__ */

