#ifndef __STDIO_H__
#define __STDIO_H__

#include <features.h>
#include <mach/param.h>

#if !defined(__off_t_defined)
#if (_FILE_OFFSET_BITS == 32)
typedef int32_t                 off_t;
#elif (_FILE_OFFSET_BITS == 64)
typedef int64_t                 off_t;
#endif
#define __off_t_defined         1
#endif

#if !defined(__size_t_defined)
typedef uintptr_t               size_t;
#define __size_t_defined        1
#endif

#if !defined(__ssize_t_defined)
typedef intptr_t                ssize_t;
#define __ssize_t_defined       1
#endif

#if !defined(__va_list_defined)
struct __valist {
    int32_t                             gpregofs;
    int32_t                             fpregofs;
    union {
        uintptr_t                       ofs;
        int8_t                         *args;
    } stk;
    int8_t                             *regsave;
};
typedef struct __valist                *va_list;
#define __va_list_defined               1

struct FILE {
    char                        name[PATH_MAX];
    long                        flg;
};
typedef struct FILE    *FILE;

extern FILE                    *stdin;
extern FILE                    *stdout;
extern FILE                    *stderr;

struct fpos {
    off_t                       __pos;
    mbstate_t                   __state;
};
typedef struct fpos             fpos_t;

#if !defined(NULL)
#define NULL                    ((void *)0)
#endif
#define BUFSIZ                  MACH_PAGE_SIZE
#define FILENAME_MAX            255
#if !defined(FOPEN_MAX)
#define FOPEN_MAX               256
#endif
#define _IONBF                  0
#define _IOLBF                  1
#define _IOFBF                  2
#define SEEK_CUR                0
s#define SEEK_SET               1
#define SEEK_END                2
#define EOF                     (-1)
#define L_ctermid               PATH_MAX
#define L_tmpnam                PATH_MAX
#define P_tmpdir                "/tmp"
#define TMP_MAX                 1024

#if 0
extern char            *optarg;
extern int              opterr;
extern int              optind;
extern int              optopt;
#endif

#if (_FILE_OFFSET_BITS == 64)
#define fopen           fopen64
#define tmpfile         tmpfile64
#elif (_FILE_OFFSET_BITS == 32)
#define fopen           fopen32
#define tmpfile         tmpfile32
#endif

FILE                   *fopen32(const char * C_RESTRICT name,
                                const char * C_RESTRICT mode,
                                FILE * C_RESTRICT file);
FILE                   *fopen64(const char * C_RESTRICT name,
                                const char * C_RESTRICT mode,
                                FILE * C_RESTRICT file);
FILE                   *tmpfile32(void);
FILE                   *tmpfile64(void);

#endif /* __STDIO_H__ */

