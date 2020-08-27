#ifndef __STDIO_H__
#define __STDIO_H__

#include <mach/param.h>

#if !defined(NULL)
#define NULL            ((void *)0)
#endif
#define BUFSIZ          PAGESIZE
#define FILENAME_MAX    255
#define FOPEN_MAX       256
#define _IONBF          0
#define _IOLBF          1
#define _IOFBF          2
#define L_ctermid       PATH_MAX
#define L_tmpnam        PATH_MAX
#define SEEK_CUR        0
#define SEEK_SET        1
#define SEEK_END        2
#define TMP_MAX         16384
#define EOF             (-1)
#define P_tmpdir        "/tmp"

extern char            *optarg;
extern int              opterr;
extern int              optind;
extern int              optopt;

#endif /* __STDIO_H__ */

