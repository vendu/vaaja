#ifndef __SYS_STATFS_H__
#define __SYS_STATFS_H__

#include <features.h>
#include <sys/types.h>

struct statfs {
	long        f_type;      // type of file system
	long        f_bsize;     // optimal transfer block size
#if (_FILE_OFFSET_BITS == 32)
	fsblkcnt_t  f_blocks;    // # of blocks in file system
	fsblkcnt_t  f_bfree;     // # of free blocks in file system
	fsblkcnt_t  f_bavail;    // # of free blocks for non-root user
	fsfilcnt_t  f_files;     // # of file nodes in file system
	fsfilcnt_t  f_ffree;     // # of free file nodes in file system
#else
	fsblkcnt64_t f_blocks;    // # of blocks in file system
	fsblkcnt64_t f_bfree;     // # of free blocks in file system
	fsblkcnt64_t f_bavail;    // # of free blocks for non-root user
	fsfilcnt64_t f_files;     // # of file nodes in file system
	fsfilcnt64_t f_ffree;     // # of free file nodes in file system
#endif
	fsid_t       f_fsid;      // file system ID
	long         f_namelen;   // maximum length of filenames
	long         f_frsize;    // fragment size
	long         f_spare[5];
};

struct statfs64 {
	long        f_type;      // type of file system
	long        f_bsize;     // optimal transfer block size
	fsblkcnt64_t f_blocks;    // # of blocks in file system
	fsblkcnt64_t f_bfree;     // # of free blocks in file system
	fsblkcnt64_t f_bavail;    // # of free blocks for non-root user
	fsfilcnt64_t f_files;     // # of file nodes in file system
	fsfilcnt64_t f_ffree;     // # of free file nodes in file system
	fsid_t       f_fsid;      // file system ID
	long         f_namelen;   // maximum length of filenames
	long         f_frsize;    // fragment size
	long         f_spare[5];
};

#if !defined(__KERNEL__)

#if defined(_LARGEFILE64_SOURCE)
#define statfs  statfs64
#define fstatfs fstatfs64
#endif
extern int statfs(const char *file, struct statfs *buf);
extern int fstatfs(int fd, struct statfs *buf);

#endif /* !__KERNEL__ */

#endif /* __SYS_STATFS_H__ */

