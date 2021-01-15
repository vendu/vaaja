#ifndef __SYS_ZEN_VFS_H__
#define __SYS_ZEN_VFS_H__

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

typedef intptr_t                vfsword_t;
typedef uintptr_t               vfsuword_t;
typedef uintptr_t               vfssize_t;
typedef intptr_t                vfsoff_t;
typedef intptr_t                vfsdev_t;
typedef uintptr_t               vfsino_t;
typedef uintptr_t               vfspid_t;
typedef uintptr_t               vfsuid_t;
typedef uintptr_t               vfsgid_t;

struct zenvfsnode {
    void                       *funcs;
    vfsuword_t                  desc;
    vfsuword_t                  type;
    vfsuword_t                  flags;
    union {
#if defined(ZEN_FS0)
        struct zenfs0common     zenfs;
#endif
    } node;
};

/* FIXME */
struct iovec;
typedef uint32_t                _DIR;
typedef uint32_t                _FILE;
typedef size_t                  socklen_t;
struct sockaddr;
struct msghdr;

struct zenvfsfuncs {
    int                         (*vfs_open)(const char *path, int flg, ...);
    int                         (*vfs_close)(int fd);
    m_ssize_t                   (*vfs_read)(int fd, void *buf, size_t len);
    m_ssize_t                   (*vfs_write)(int fd, void *buf, size_t len);
    vfsoff_t                    (*vfs_seek)(int fd, vfsoff_t ofs, int whence);
    int                         (*vfs_lock)(int fd, int cmd, vfsoff_t len);
    void                        (*vfs_map)(void *adr, size_t len, int prot, int flags,
                                           int fd, vfsoff_t ofs);
    void                        (*vfs_unmap)(void *adr, size_t len);
    int                         (*vfs_chmod)(const char *path, zenmode_t mode);
    int                         (*vfs_chown)(const char *path, vfsuid_t owner, vfsgid_t grp);
    int                         (*vfs_link)(const char *src, const char *dest);
    int                         (*vfs_symlink)(const char *src, const char *dest);
    int                         (*vfs_unlink)(const char *path);
    int                         (*vfs_dup2)(int srcfd, int destfd);
    m_ssize_t                   (*vfs_readv)(int fd, const struct iovec *iov, int niov);
    m_ssize_t                   (*vfs_writev)(int fd, const struct iovec *iov, int niov);
    m_ssize_t                   (*vfs_fsync)(int fd);
    int                         (*vfs_falloc)(int fd, int mode, vfsoff_t ofs, vfsoff_t len);
    int                         (*vfs_ftrunc)(int fd, vfsoff_t len);
    int                         (*vfs_fadvise)(int fd, vfsoff_t ofs, vfsoff_t len, int advice);
};

struct zenvfsdevfuncs {
    int                         (*vfs_mkdev)(const char *path, zenmode_t mode, vfsdev_t dev);
    int                         (*vfs_mount)(const char *devpath, const char *destpath,
                                             const char *fstype, unsigned long flags,
                                             const void *data);
    int                         (*vfs_unmount)(const char *path, int flags);
};

struct zenvfsdirfuncs {
    _DIR                        (*vfs_opendir)(const char *path);
    int                         (*vfs_closedir)(_DIR *dir);
    int                         (*vfs_mkdir)(const char *path,  zenmode_t mode);
    int                         (*vfs_rmdir)(const char *path);
    struct dirent               (*vfs_readdir)(_DIR *dir);
    void                        (*vfs_seekdir)(_DIR *dir, long loc);
    void                        (*vfs_rewinddir)(_DIR *dir);
    long                        (*vfs_telldir)(_DIR *dir);
};

struct zenvfsfifofuncs {
    _FILE                       (*vfs_popen)(const char *cmd, const char *type);
    int                         (*vfs_pclose)(_FILE *fp);
    m_ssize_t                   (*vfs_pread)(int fd, void *buf, size_t len, vfsoff_t ofs);
    m_ssize_t                   (*vfs_pwrite)(int fd, void *buf, size_t len, vfsoff_t ofs);
};

struct zenvfssockfuncs {
    int                         (*vfs_socket)(int domain, int type, int protocol);
    int                         (*vfs_bind)(int sockfd, const struct sockaddr *adr,
                                         socklen_t adrlen);
    int                         (*vfs_listen)(int sockfd, int backlog);
    int                         (*vfs_accept)(int sockfd, struct sockaddr *C_RESTRICT adr,
                                           socklen_t *C_RESTRICT adrlen);
    int                         (*vfs_connect)(int sockfd, const struct sockaddr *adr,
                                            socklen_t adrlen);
    int                         (*vfs_getsockname)(int sockfd, struct sockaddr *C_RESTRICT adr,
                                                socklen_t *C_RESTRICT adrlen);
    int                         (*vfs_getsockopt)(int sockfd, int level, int optname,
                                               void *C_RESTRICT optval,
                                               socklen_t *C_RESTRICT optlen);
    int                         (*vfs_getpeername)(int sockfd, struct sockaddr *C_RESTRICT adr,
                                                socklen_t *C_RESTRICT adrlen);
    m_ssize_t                   (*vfs_recv)(int sockfd, void *buf, size_t len, int flags);
    m_ssize_t                   (*vfs_recvfrom)(int sockfd, void *C_RESTRICT buf, size_t len,
                                             int flags,
                                             struct sockaddr *C_RESTRICT adr,
                                             socklen_t *C_RESTRICT adrlen);
    m_ssize_t                   (*vfs_recvmsg)(int sockfd, struct msghdr *msg, int flags);
    m_ssize_t                   (*vfs_send)(int sockfd, const void *buf, size_t len, int flags);
    int                         (*vfs_shutdown)(int sockfd, int how);
};

struct zenvfsshmfuncs {
    int                         (*vfs_shmopen)(const char *path, int oflag, zenmode_t mode);
    int                         (*vfs_shmunlink)(const char *path);
    void                        (*vfs_shmat)(int shmfd, const void *shmadr, int flags);
};

#endif /* __SYS_ZEN_VFS_H__ */

