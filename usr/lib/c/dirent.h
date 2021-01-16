#ifndef __DIRENT_H__
#define __DIRENT_H__

#define PATH_MAX  4095
#define NAMEMAX   255
#define MAXNAMLEN NAMEMAX

#include <stdint.h>
#include <zero/cdecl.h>

#if (__GLIBC__)
typedef struct __dirstream DIR;
#endif

struct dirent *readdir(DIR *dirp);

#define d_fileno d_ino
struct dirent {
    ino_t    d_ino;
    off_t    d_off;
    uint16_t d_reclen;
    uint8_t  d_type;
    uint8_t  d_namlen;
#if defined(_POSIX_SOURCE)
    uint8_t  d_name[NAMEMAX + 1];
#else
    uint8_t  d_name[MAXNAMLEN + 1];
#endif
};

/* file types */
#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14

/* convert between struct stat and directory types */
#define	IFTODT(mode)	(((mode) & 0170000) >> 12)
#define	DTTOIF(dirtype) ((dirtype) << 12)

#endif /* __DIRENT_H__ */

