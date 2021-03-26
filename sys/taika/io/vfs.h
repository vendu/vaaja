#ifndef TAIKA_IO_VFS_H
#define TAIKA_IO_VFS_H

/* VFS-flags and attributes */
#define VFS_LOCK_BIT            (1 << 0)
#define VFS_MECH_BIT            (1 << 1)        // mechanical (seeking) device
#define VFS_SOFTUP_BIT          (1 << 2)        // soft-updates
#define VFS_JOURNAL_BIT         (1 << 3)        // filesystem journaling
#define VFS_BUSY_BIT            (1 << 4)        // filesystem being checked
#define VFS_DIRTY_BIT           (1 << 5)        // filesystem dirty
#define VFS_MOUNT_BIT           (1 << 6)        // filesystem mounted
#define VFS_MAGIC               0x13620348      // filesystem recognition node
#define VFS_BLOCK_SIZE          4096            // filesystem block size
#define VFS_DIRECT_BLOCKS       12              // # of direct blocks/inode
#define VFS_INDIRECT_BLOCKS     4               // # of direct block IDs/inode
#define VFS_NAME_LEN            255             // filename w/o terminating '\0'

/* file types */
#define VFS_UNKNOWN_NODE        0               // uninitialised type
#define VFS_CHR_NODE            1               // character device
#define VFS_BLK_NODE            2               // block device
#define VFS_REG_NODE            3               // regular file object
#define VFS_DIR_NODE            4               // directory object
#define VFS_FIFO_NODE           5               // pipe/FIFO object
#define VFS_SOCK_NODE           6               // socket object
#define VFS_SYMLiNK_NODE        7               // symbolic link
struct vnodeops {
    int                         creat(const char *pathname,
                                      mode_t mode);
    int                         open(const char *pathname,
                                     int flags,
                                     ... /* mode_t mode */);
    int                         close(fd);
    off_t                       seek(int fd, off_t ofs, int whence);
    ssize_t                     read(int fd,
                                     void *buf,
                                     size_t nb);
    ssize_t                     write(int fd,
                                      void *buf,
                                      size_t nb);
    int                         fsync(int fd);
    int                         fdatasync(int fd);
    int                         rename(const char *oldpath,
                                       const char *newpath);
    int                         link(const char *oldpath,
                                     const char *newpath);
    int                         unlink(const char *pathname);
    int                         mkdir(const char *pathname,
                                      mode_t mode);
    int                         rmdir(const char *pathname);
    int                         ioctl(int fd,
                                      unsigned long req,
                                      ... /* char *argp */);
};

/* flags for mount() */
// NOTE: MS_NOSUID not implemented
#define VFS_MOUNT_REMOUNT       (1 << 0)
#define VFS_MOUNT_BIND          (1 << 1)
#define VFS_MOUNT_SHARED        (1 << 2)
#define VFS_MOUNT_PRIVATE       (1 << 3)
#define VFS_MOUNT_SLAVE         (1 << 4)
#define VFS_MOUNT_UNBINDABLE    (1 << 5)
#define VFS_MOUNT_MOVE          (1 << 6)
#define VFS_DIRSYNC             (1 << 7)        // per-superblock
#define VFS_LAZYTIME            (1 << 8)        // per-superblock
#define VFS_STRICTATIME         (1 << 9)        // overrides NOATIME & RELATIME
#define VFS_NOATIME             (1 << 10)       // per-mountpoint
#define VFS_RELATIME            (1 << 11)       // per-mountpoint
#define VFS_NODIRATIME          (1 << 12)       // per-mountpoint
#define VFS_NODEV               (1 << 13)       // per-mountpoint
#define VFS_NOEXEC              (1 << 14)       // per-mountpoint
#define VFS_NOSUID              (1 << 15)       // per-mountpoint
#define VFS_RDONLY              (1 << 15)
#define VFS_REC                 (1 << 16)
#define VFS_SILENT              (1 << 17)       // per-superblock
#define VFS_SYNCHRONOUS         (1 << 18)       // per-superblock
#define VFS_NOSYMFOLLOW         (1 << 19)
#define VFS_MANDLOCK            (1 << 20)       // per-superblock
/* flags for umount2() */
#define VFS_UMOUNT_FORCE        (1 << 0)
#define VFS_UMOUNT_DETACH       (1 << 1)
#define VFS_UMOUNT_EXPIRE       (1 << 2)
#define VFS_UMOUNT_NOFOLLOW     (1 << 3)
struct vfsops {
    int                         mount(const char *src,
                                      const char *dest,
                                      const char *fstype,
                                      unsigned long mntflags,
                                      const void *data);
    int                         umount2(const char *dest,
                                        int flags);
    int                         statfs(const char *buf, struct statfs *buf);
    int                         fstatfs(int fd, struct statfs *buf);
    void                        sync(void);
    int                         syncfs(int fd);
};

#endif /* TAIKA_IO_VFS_H */

