#ifndef TAIKA_FS0_H
#define TAIKA_FS0_H

#include <stdint.h>
#include <env/cdefs.h>
#include <internal/types.h>
#include <internal/mach/types.h>

/* fs0 aims to be a somewhat-modernised version of berkeley ffs */

#define TAIKA_FS0_VERSION       1
#define TAIKA_FS0_NAME          "tfs0"

typedef __INT64_T               tfs0quad_t;
typedef __INT32_T               tfs0word_t;
typedef __INT16_T               tfs0half_t;
typedef __INT8_t                tfs0byte_t;
typedef __SIZE_T                tfs0size_t;
typedef __OFF_T                 tfs0off_t;
typedef __INO_T                 tfs0ino_t;
typedef __UID_T                 tfs0uid_t;
typedef __GID_T                 tfs0gid_t;
typedef __TIME_T                tfs0time_t;

/* exactly 64 bytes */
#define TFS0_BLK_TAB_SIZE       64
struct tfs0blktab {
    fs0ino_t                    dir[TFS0_DIRECT_BLOCKS];     // direct blocks
    fs0ino_t                    indir[TFS0_INDIRECT_BLOCKS]; // indirect
};

/* exactly 64 bytes */
#define TFS0_LINK_SIZE          64
union tfs0link {
    struct tfs0blktab           blktab;             // fs block book-keeping
    uint8_t                     type;               // chr, blk, fifo, sock, lnk
    uint8_t                     name[TFS0_LINK_NAME_MAX]; // link name
};

/* struct fs0common is exactly 128 bytes and on-disk verbatim */
#define TFS0_COMMON_SIZE        128
struct tfs0common {
    tfs0size_t                  size;   // file size in bytes
    tfs0size_t                  nblk;   // # of 1K blocks used
    tfs0time_t                  atime;  // access time
    tfs0time_t                  mtime;  // modification time
    tfs0time_t                  ctime;  // inode change time
    tfs0uid_t                   uid;    // owner user ID
    tfs0gid_t                   gid;    // owner group ID
    tfs0half_t                  nlink;  // # of links to file
    tfs0byte_t                  type;   // node type
    tfs0byte_t                  ver;    // filesystem version
    tfs0word_t                  gen;    // generation ID
    tfs0ino_t                   meta;   // 0 or metadata inode ID
    tfs0ino_t                   extatr; // 0 or extended attributes inode ID
    union tfs0link              link;   // block book-keeping or object name
};

struct tfs0dirent {
    tfs0size_t                  ofs;            // 64-bit offset
    tfs0ino_t                   ino;            // 32-bit inode number
    tfs0half_t                  namelen;        // 16-bit length of name
    tfs0byte_t                  type;           // VFS_*_NODE
    tfs0byte_t                  ver;            // filesystem version
    tfs0byte_t                  name[C_VLA];    // padded to 64-bit boundary
};

#endif /* TAIKA_FS0_H */

