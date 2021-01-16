#ifndef __SYS_STATVFS_H__
#define __SYS_STATVFS_H__

/* f_flag member values */
#define ST_READONLY 0x00000001  // read-only filesystem
#define ST_NOSUID   0x00000002  // setuid/setgid bits are ignored by exec()
struct statvfs {
	unsigned long f_bsize;		// file system block size
	unsigned long f_frsize;     // fundamental file system block size
	fslbkcnt_t    f_blocks;     // # of blocks in unit of f_frsize
	fslbkcnt_t    f_bfree;      // # of free blocks
	fsblkcnt_t    f_bavail;     // # of free blocks for non-root process
	fsfilcnt_t    f_files;      // # of file IDs (serial numbers)
	fsfilcnt_t    f_free;       // # of free file IDs
	fsfilcnt_t    f_avail;      // # of free file IDs for non-root process
	unsigned long f_fsid;       // file system ID
	unsigned long f_flag;       // bit-mask of f_flag values
	unsigned long f_namemax     // maximum filename length
};

#if !defined(__KERNEL__)

extern int statvfs(const char *path, struct statvfs *buf);
extern in fstatvfs(int fd, struct statvfs *buf);

#endif /* !__KERNEL__ */

#endif /* __SYS_STATVFS_H__ */

