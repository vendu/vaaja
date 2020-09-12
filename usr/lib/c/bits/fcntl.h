#ifndef __BITS_FCNTL_H__
#define __BITS_FCNTL_H__

#include <features.h>
#if !defined(__off_t_defined)
#include <share/off_t.h>
#endif

#define O_ACCMODE        000003
#define O_RDONLY         000000 // read-only
#define O_WRONLY         000001 // write-only
#define O_RDWR           000002 // read-write
#define O_SHLOCK         000004 // shared lock
#define O_EXLOCK         000010 // exclusive lock
#define O_EVTONLY        000020 // descriptor for event notifications only
#define O_SYMLINK        000040 // allow opening symlinks
#define O_CREAT          000100 // create if nonexistent
#define O_EXCL           000200 // error if already exists (with O_CREAT)
#define O_NOCTTY         000400 // don't assign controlling terminal
#define O_TRUNC          001000 // truncate to zero length
#define O_APPEND         002000 // set append mode
#define O_NONBLOCK       004000 // don't block on I/O operations
#define O_NDELAY         O_NONBLOCK
#define O_SYNC           010000 // allow simultaneous inclusion of <aio.h>
#define O_DSYNC          O_SYNC
#define O_ASYNC          020000
#define O_FSYNC          O_SYNC
#if defined(_GNU_SOURCE)
#define O_DIRECT         040000 // open for direct I/O
#define O_NOFOLLOW       100000 // don't follow symlinks
#define O_NOATIME        200000 // don't update access-time on read
#define O_LARGEFILE      400000 // utilise 64-bit off_t
#endif
#if (_POSIX_SOURCE >= 199309L) || defined(_USE_UNIX98)
#define O_DSYNC          O_SYNC // synchronised writes
#define O_RSYNC          O_SYNC // synchronised reads
#endif
#if defined(_BSD_SOURCE)
#define O_POPUP          0x80000000
#define O_ALERT          0x40000000
#endif
#define O_SEARCH         0x20000000 // open directory for search only
#define O_EXEC           0x10000000 // open for execute-only (non-directories)
#define O_DIRECTORY      0x08000000 // open directory
#define O_TTY_INIT       0x04000000 // initialize pseudo-terminal
#define O_RSHARE         0x02000000 // read shared
#define O_NSHARE         O_RSHARE
#define O_DEFER          0x01000000 // deferred update
#define O_DELAY          0x00800000 // open with delay

#if defined(_BSD_SOURCE)
/* old names for flags */
#define FREAD            0x0001
#define FWRITE           0x0002
#define FAPPEND          O_APPEND
#define FASYNC           O_ASYNC
#define FFSYNC           O_FSYNC
#define FFDSYNC          O_DSYNC
#define FDATASYNC        O_DSYNC
#define FREADSYNC        O_RSYNC
#define FNONBLOCK        O_NONBLOCK
#define FNDELAY          O_NONBLOCK
#endif

/* bits to save after open */
#define FMASK            (FREAD | FWRITE | FAPPEND | FASYNC             \
                          | FFSYNC | FFDSYNC | FNONBLOCK)
/* bits settable byt fcntl(F_SETLF, ...) */
#define FCNTLFLAGS       (FAPPEND | FASYNC | FFSYNC | FFDSYNC | FNONBLOCK)

/* descriptor flags */
#define FD_CLOEXEC       (1 << 0) // argument for F_SETFD
/* FD_GETFD may return these in addition to FD_CLOEXEC */
#define FD_MANDATORYLOCK (1 << 1)
#define FD_ADVISORYLOCK  (1 << 2)
#define FD_DIRECTORY     (1 << 3)
/* cmd-arguments for fcntl() */
#define F_DUPFD          0      // duplicate file descriptor
#define F_GETFD          1      // get file descriptor flags
#define F_SETFD          2      // set file descriptor flags
#define F_GETFL          3      // get file status flags
#define F_SETFL          4      // set file status flags
#if (_FILE_OFFSET_BITS == 64)
#define F_GETLK          5      // get record-locking information
#define F_SETLK          6      // set record-locking information
#define F_SETLKW         7
#else
#define F_GETLK          F_GETLK64
#define F_SETLK          F_SETLK64
#define F_SETLKW         F_SETLKW64
#endif
#define F_GETLK64        12
#define F_SETLK64        13
#define F_SETLKW64       14
#if (_BSD_SOURCE) || defined(_USE_UNIX98)
#define F_SETOWN         8      // set SIGIO/SIGURG proc/pgrp
#define F_GETOWN         9      // set SIGIO/SIGURG proc/pgrp
#endif
#if defined(_GNU_SOURCE)
#define F_SETSIG         10
#define F_GETSIG         11
#define F_SETLEASE       1024
#define F_GETLEASE       1025
#define F_NOTIFY         1026
#endif
#if defined(_BSD_SOURCE)
#define FIOSETOWN        F_SETOWN
#define FIOGETOWN        F_GETOWN
#define FIOASYNC         15
#define F_CHKCLEAN       16     // used for regression test
#define F_PREALLOCATE    17	// preallocate storage
#define F_DEALLOCATE     18     // release storage
#define F_SETSIZE        19	// truncate a file without zeroing space
#define F_RDADVISE       20     // Issue an advisory read async with no copy to user
#define F_RDAHEAD        21     // turn read ahead off/on for this fd
#define F_READBOOTSTRAP  22     // Read bootstrap from disk
#define F_WRITEBOOTSTRAP 23     // Write bootstrap on disk
#define F_NOCACHE        24     // turn data caching off/on for this fd
#define F_LOG2PHYS	 25	// file offset to device offset
#define F_GETPATH        26     // return the full path of the fd
#define F_FULLFSYNC      27     // fsync + ask the drive to flush to the media
#define F_PATHPKG_CHECK  28     // find which component (if any) is a package
#define F_FREEZE_FS      29     // "freeze" all fs operations
#define F_THAW_FS        30     // "thaw" all fs operations
#define	F_GLOBAL_NOCACHE 31     // turn data caching off/on for this file
#define F_ADDSIGS        32     // add detached signatures
#define F_MARKDEPENDENCY 33     // process hosts device supporting fs-backing fd
#define F_ADDFILESIGS    34     // add [shared-lib] signature fromm same file
#define FCNTL_FS_SPECIFIC_BASE 0x00010000
/* flags for F_PREALLOCATE */
#define F_ALLOCATECONTIG (1 << 1) // allocate contiguous space
#define F_ALLOCATEALL    (1 << 2) // allocate all requested space or nothing
#endif /* defined(_BSD_SOURCE) */
/* lockf() */
#define F_RDLCK          1
#define F_UNLCK          2
#define F_WRLCK          3
/* old BSD flock() */
#define F_EXLCK          4
#define F_SHLCK          8
#if (_BSD_SOURCE)
/* op-arguments for flock() */
#define LOCK_SH          1
#define LOCK_EX          2
#define LOCK_NB          4
#define LOCK_UN          8
#endif
#if defined(_GNU_SOURCE)
#define LOCK_MAND        0x00000020
#define LOCK_READ        0x00000040
#define LOCK_WRITE       0x00000080
#define LOCK_RW          (LOCK_READ | LOCK_WRITE)
#endif

#if defined(_GNU_SOURCE)
#define DN_ACCESS        0x00000001
#define DN_MODIFY        0x00000002
#define DN_CREATE        0x00000004
#define DN_DELETE        0x00000008
#define DN_RENAME        0x00000010
#define DN_ATTRIB        0x00000020
#define DN_MULTISHOT     0x80000000
#endif

/* these constants are in <unistd.h> as well */
#if !defined(R_OK)
#define R_OK             4 // check for read permission
#define W_OK             2 // check for write permission
#define X_OK             1 // check for execute permission
#define F_OK             0 // check for existence
#endif

#if !defined(F_LOCK) && defined(_XOPEN_SOURCE_EXTENDED) && !defined(_POSIX_SOURCE)
/* these constants also appear in <unistd.h> */
#define F_ULOCK          0
#define F_LOCK           1
#define F_TLOCK          2
#define F_TEST           3
#endif

#if defined(_GNU_SOURCE)
#define AT_FDCWD            (-1)
#define AT_SYMLINK_NOFOLLOW 0x100
#define AT_REMOVEDIR        0x200
#define AT_SYMLINK_FOLLOW   0x400
#define AT_EACCESS          0x200
#endif

/* if l_start == 0 && l_len == 0 && l_whence == SEEK_SET, lock whole file */
struct flock {
#if (_FILE_OFFSET_BITS == 64)
    off_t   l_start;    // relative offset in bytes
    off_t   l_len;      // size; if 0 then until EOF
#else
    off64_t l_start;
    off64_t l_len;
#endif
    pid_t   l_pid;      // process ID of the lock-holder (returned with F_GETLK)
    short   l_type;     // F_RDLCK, F_WRLCK, F_UNLCK
    short   l_whence;   // SEEK_SET, SEEK_CUR, SEEK_END
};

#if (_LARGEFILE64_SOURCE)
struct flock64 {
    off64_t l_start;
    off64_t l_len;
    pid_t   l_pid;
    short   l_type;
    short   l_whence;
};
#endif

#if defined(_BSD_SOURCE)

struct radvisory {
    off_t ra_offset;
    int   ra_count;
};

/* F_ADDSIGS and F_ADDFILESIGS */
typedef struct fsignatures {
    off_t  fs_file_start;
    void  *fs_blob_start;
    size_t fs_blob_size;
} fsignatures_t;

/* F_DEALLOCATE and F_PREALLOCATE */
struct fstore {
    unsigned int fst_flags;             // flag bits
    int          fst_posmode;           // indicates use of fs_offset
    off_t        fst_offset;            // start of the region
    off_t        fst_length;            // size of the region
    off_t        fst_bytesalloc;        // # of bytes
};

/* F_READBOOTSTRAP and F_WRITEBOOTSTRAP */
typedef struct fbootstraptransfer {
    off_t   fbt_offset;                 // start offset
    size_t  fbt_length;                 // # of bytes to transfer
    void   *fbt_buffer;                 // read/write buffer
} fbootstraptransfer_t;

struct fopenfrom {
    unsigned int  o_flags;
    mode_t        o_mode;
    char         *o_pathname;           // relative pathname
};

#define _FILESEC_UNSET_PROPERTY ((void *)0)
#define _FILESEC_REMOVE_ACL     ((void *)1)

struct _filesec;
typedef struct _filesec *filesec_t;
typedef enum {
    FILESEC_OWNER = 1,
    FILESEC_GROUP = 2,
    FILESEC_UUID  = 3,
    FILESEC_MODE  = 4,
    FILESEC_ACL   = 5,
    FILESEC_GRPUUID = 6,
    /* private */
    FILESEC_ACL_RAW = 100,
    FILESEC_ACL_ALLOCSIZE = 101
} filesec_property_t;
#define FILESEC_GUID FILESEC_UUID       // backwards compatibility

#endif /* defined(_BSD_SOURCE) */

#if defined(_USE_XOPEN2K)
#define POSIX_FADV_NORMAL     0
#define POSIX_FADV_RANDOM     1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED   3
#define POSIX_FADV_DONTNEED   4
#define POSIX_FADV_NOREUSE    5
#endif

#if defined(_GNU_SOURCE)
#define SYNC_FILE_RANGE_WAIT_BEFORE 1
#define SYNC_FILE_RANGE_WRITE       2
#define SYNC_FILE_RANGE_WAIT_AFTER  4

#define SPLICE_F_MOVE               1
#define SPLICE_F_NONBLOCK           2
#define SPLICE_F_MORE               4
#define SPLICE_F_GIFT               8
#endif

#if !defined(SEEK_SET)
#define SEEK_SET                    0
#define SEEK_CUR                    1
#define SEEK_END                    2
#if defined(_GNU_SOURCE)
#define SEEK_DATA                   3
#define SEEK_HOLE                   4
#endif
#endif
#if defined(_BSD_SOURCE)
/* position modes for F_PREALLOCATE */
#define F_PEOFPOSMODE               5   // past all SEEK modes
#define F_VOLPOSMODE                6   // specify volume starting position
#endif /* defined(_BSD_SOURCE) */

#if defined(_BSD_SOURCE)
#define L_SET                       SEEK_SET
#define L_INCR                      SEEK_CUR
#define L_XTND                      SEEK_END
#endif

/* file mode */
/* owner permissions */
#define S_IRWXU         0000700
#define S_IRUSR         0000400
#define S_IWUSR         0000200
#define S_IXUSR         0000100
/* group permissions */
#define	S_IRWXG		0000070		// RWX mask for group
#define	S_IRGRP		0000040		// R for group
#define	S_IWGRP		0000020		// W for group
#define	S_IXGRP		0000010		// X for group
/* Read, write, execute/search by others */
#define	S_IRWXO		0000007		// RWX mask for other
#define	S_IROTH		0000004		// R for other
#define	S_IWOTH		0000002		// W for other
#define	S_IXOTH		0000001		// X for other

#define S_IFMPX         0010000         // multiplexed character-device
#define S_IXACL         0020000         // extended access control list
#define S_ITCB          0040000         // trusted computing base
#define S_IJRNL         0100000         // journaled file
#define S_ICRYPTO       0200000         // encrypted file on a crypto-fs

#define S_ENFMT         S_ISGID

#if !defined(_BSD_SOURCE)
#define	S_ISTXT		S_ISVTX		// sticky bit: not supported
#define	S_IREAD		S_IRUSR		// backward compatability
#define	S_IWRITE	S_IWUSR		// backward compatability
#define	S_IEXEC		S_IXUSR		// backward compatability
#endif

#endif /* __BITS_FCNTL_H__ */

