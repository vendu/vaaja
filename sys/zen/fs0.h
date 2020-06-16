#ifndef __ZEN_FS0_H__
#define __ZEN_FS0_H__

/* zenfs0 aims to be a somewhat-modernised version of berkeley ffs */

#include <stdint.h>
#include <zero/cdefs.h>
#include <zen/vfs.h>

#define ZEN_FS                  1
#define FS0_NAME                "zenfs0"

typedef int64_t zenfs0long_t;
typedef int32_t zenfs0word_t;
typedef int16_t zenfs0half_t;
typedef int8_t  zenfs0byte_t;
typedef int64_t zenfs0size_t;
typedef int64_t zenfs0off_t;
typedef int32_t zenfs0ino_t;
typedef int32_t zenfs0uid_t;
typedef int32_t zenfs0gid_t;
typedef int64_t zenfs0time_t;

#define ZEN_FS0_MECH_BIT        (1 << 0)        // mechanical (seeking) device
#define ZEN_FS0_SOFTUP_BIT      (1 << 1)        // soft-updates
#define ZEN_FS0_JOURNAL_BIT     (1 << 2)        // filesystem journaling
#define ZEN_FS0_BUSY_BIT        ZEN_VFS_BUSY    // filesystem being checked
#define ZEN_FS0_DIRTY_BIT       ZEN_VFS_DIRTY   // filesystem dirty
#define ZEN_FS0_MOUNT_BIT       ZEN_VFS_MOUNTED // filesystem mounted
#define ZEN_FS0_MAGIC           0x13620348
#define ZEN_FS0_BLOCK_SIZE      4096            // filesystem block size
#define ZEN_FS0_DIRECT_BLOCKS   12              // # of direct blocks/inode
#define ZEN_FS0_INDIRECT_BLOCKS 4               // # of direct block IDs/inode
#define ZEN_FS0_NAME_LEN        255             // filename w/o terminating '\0'
#define ZEN_FS0_UNKNOWN_NODE    ZEN_VFS_UNKNOWN_NODE    // uninitialised type
#define ZEN_FS0_CHR_NODE        ZEN_VFS_CHR_NODE        // character device
#define ZEN_FS0_BLK_NODE        ZEN_VFS_BLK_NODE        // block device
#define ZEN_FS0_REG_NODE        ZEN_VFS_FILE_NODE       // regular file object
#define ZEN_FS0_DIR_NODE        ZEN_VFS_DIR_NODE        // directory object
#define ZEN_FS0_FIFO_NODE       ZEN_VFS_FIFO_NODE       // pipe/FIFO object
#define ZEN_FS0_SOCK_NODE       ZEN_VFS_SOCK_NODE       // socket object
#define ZEN_FS0_SYMLiNK_NODE    ZEN_VFS_SYMLINK_NODE    // symbolic link
/* exactly 64 bytes */
#define ZEN_FS0_BLK_TAB_SIZE    64
struct zenfs0blktab {
    zenfs0ino_t         dir[ZEN_FS0_DIRECT_BLOCKS];      // direct blocks
    zenfs0ino_t         indir[ZEN_FS0_INDIRECT_BLOCKS];  // indirect blocks
};

/* exactly 64 bytes */
#define ZEN_FS0_LINK_SIZE       64
union zenfs0link {
    struct zenfs0blktab blktab;                  // fs block book-keeping
    uint8_t             name[ZEN_FS0_LINK_SIZE]; // CHR, BLK, FIFO, SOCK, LNK
};

/* struct zenfs0common is exactly 128 bytes and on-disk verbatim */
#define ZEN_FS0_COMMON_SIZE     128
struct zenfs0common {
    zenfs0size_t        size;   // file size in bytes                   0
    zenfs0size_t        nblk;   // # of 1K blocks used                  8
    zenfs0time_t        atime;  // access time                          16
    zenfs0time_t        mtime;  // modification time                    24
    zenfs0time_t        ctime;  // inode change time                    32
    zenfs0uid_t         uid;    // owner user ID                        40
    zenfs0gid_t         gid;    // owner group ID                       44
    zenfs0half_t        nlink;  // # of links to file                   48
    zenfs0byte_t        type;   // node type                            50
    zenfs0byte_t        ver;    // filesystem version                   51
    zenfs0word_t        gen;    // generation ID                        52
    zenfs0ino_t         meta;   // 0 or metadata inode ID               56
    zenfs0ino_t         extatr; // 0 or extended attributes inode ID    60
    union zenfs0link    link;   // block book-keeping or object name    64
};

struct zenfs0dirent {
    zenfs0size_t        ofs;            // 64-bit offset
    zenfs0ino_t         ino;            // 32-bit inode number
    zenfs0half_t        namelen;        // 16-bit length of name
    zenfs0byte_t        type;           // ZEN_FS0_*_NODE
    zenfs0byte_t        ver;            // filesystem version
    zenfs0byte_t        name[C_VLA];    // padded to 64-bit boundary
};

#endif /* __ZEN_FS0_H__ */

