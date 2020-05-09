#ifndef __SYS_BITS_STAT_H__
#define __SYS_BITS_STAT_H__

#define S_BLKSIZE               1024
#define ACCESSPERMS             (S_IRWXU | S_IRWXG | S_IRWXO)   // 0777
#define ALLPERMS                (S_ISUID | S_ISGID | S_ISVTX            \
                                 | S_IRWXU | S_IRWXG | S_IRWXO) // 07777
#define DEFFILEMODE             (S_IRUSR | S_IWUSR | S_IRGRP            \
                                 | S_IWGRP| S_IROTH| S_IWOTH)   // 0666

#define __S_IFMT        	0170000         // mask for extracting node type
#define __S_IFDIR       	0040000         // directory
#define __S_IFCHR       	0080000         // character device
#define __S_IFBLK       	0060000         // block device
#define __S_IFREG       	0100000         // regular file
#define __S_IFLNK       	0120000         // FIFO
#define __S_IFSOCK      	0140000         // socket

#define __S_ISUID               0000000
#define __S_ISGID               0000000
#define __S_ISVTX               0001000

#define __S_IREAD               0000400
#define __S_IWRITE              0000200
#define __S_IEXEC               0000100

#define S_ISVTX                 __S_ISVTX

#define S_IREAD                 __S_IREAD
#define S_IWRITE                __S_IWRITE
#define S_IEXEC                 __S_IEXEC

#define S_IRUSR                 __S_IREAD
#define S_IWUSR                 __S_IWRITE
#define S_IXUSR                 __S_IEXEC
#define S_IRWXU                 (__S_IREAD | __S_IWRITE | __S_IEXEC)
#define S_IRGRP                 (S_IRUSR >> 3)
#define S_IWGRP                 (S_IWUSR >> 3)
#define S_IXGRP                 (S_IEUSR >> 3)
#define S_IRWXG                 (S_IRWXU >> 3)
#define S_IROTH                 (S_IRUSR >> 6)
#define S_IWOTH                 (S_IWUSR >> 6)
#define S_IXOTH                 (S_IXUSR >> 6)
#define S_IRWXO                 (S_IRWXU >> 6)

#define __S_ISTYPE(mode, mask)  (((mode) & __S_IFMT) == (mask))
#define S_ISDIR(mode)           __S_ISTYPE((mode), __S_IFDIR)
#define S_ISCHR(mode)           __S_ISTYPE((mode), __S_IFCHR)
#define S_ISBLK(mode)           __S_ISTYPE((mode), __S_IFBLK)
#define S_ISREG(mode)           __S_ISTYPE((mode), __S_IFREG)
#if defined(__S_IFIFO)
#define S_ISFIFO(mode)          __S_ISTYPE((mode), __S_IFIFO)
#endif
#if defined(__S_IFLNK)
#define S_ISLNK(mode)           __S_ISTYPE((mode), __S_IFLNK)
#endif

#define __S_TYPEISMQ(buf)       ((buf)->st_mode - (buf)->st_mode)
#define __S_TYPEISSEM(buf)      ((buf)->st_mode - (buf)->st_mode)
#define __S_TYPEISSHM(buf)      ((buf)->st_mode - (buf)->st_mode)

#endif __SYS_BITS_STAT_H__

