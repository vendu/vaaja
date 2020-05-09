#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#if !defined(__dev_t_defined)
typedef uint32_t                dev_t;
#define __dev_t_defined         1
#endif
#if !defined(__uid_t_defined)
typedef uint32_t                uid_t;
#define __uid_t_defined         1
#endif
#if !defined(__gid_t_defined)
typedef uint32_t                gid_t;
#define __gid_t_defined         1
#endif
#if !defined(__blkcnt_t_defined)
typedef uint32_t                blkcnt_t;
#define __blkcnt_t_defined      1
#endif
#if !defined(__blksize_t_defined)
typedef uint32_t                blksize_t;
#define __blksize_t_defined     1
#endif
#if !defined(__ino_t_defined)
typedef uint32_t                ino_t;
#define __ino_t_defined         1
#endif
#if !defined(__mode_t_defined)
typedef uint32_t                mode_t;
#define __mode_t_defined        1
#endif
#if !defined(__nlink_t_defined)
typedef uint32_t                nlink_t;
#define __nlink_t_defined       1
#endif
#if !defined(__off_t_defined)
typedef int64_t                 off_t;
#define __off_t_defined         1
#endif
#if !defined(__size_t_defined)
typedef uintptr_t               size_t;
#define __size_t_defined        1
#endif
#if !defined(__time_t_defined)
typedef uint64_t                time_t;
#define __time_t_defined        1
#endif

struct stat {
    dev_t       st_dev;
    ino_t       st_ino;
    mode_t      st_mode;
    nlink_t     st_nlink;
    uid_t       st_uid;
    gid_t       st_gid;
    dev_t       st_rdev;
    off_t       st_size;
    time_t      st_atime;
    time_t      st_mtime;
    time_t      st_ctime;
    blksize_t   st_blksize;
    blkcnt_t    st_blocks;
};

#endif __SYS_STAT_H__

