#ifndef __VAS_IO_H__
#define __VAS_IO_H__

void vasinitbuf(void);

#define VASBUFSIZE 131072

/* zero assembler I/O facilities */

#if (VASMMAP)
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#elif (VASBUF)
#include <errno.h>
#include <fcntl.h>
#endif

#define VASLINELEN     4096

#if (VASMMAP)
struct vasmap {
    char   *adr;
    char   *cur;
    char   *lim;
    size_t  sz;
};
#elif (VASBUF)
struct readbuf {
    void *data;
    char *cur;
    char *lim;
};
#endif

#if (VASMMAP)
#define vasgetc(map) ((map)->cur < (map)->lim ? *(map)->cur++ : EOF)
#elif (VASBUF)
static int vasgetc(int fd, int bufid);
#else

#endif

#endif /* __VAS_IO_H__ */

