#ifndef __STDIO_H__
#define __STDIO_H__

#include <features.h>
#if !defined(NULL)
#include <share/null.h>
#endif
#if !defined(__size_t_defined)
#include <share/size.h>
#endif
#if (_XOPEN_SOURCE) || (__STDC__)
#if defined(__GNUC__)
#include <stdarg.h>
typedef va_list __stdio_va_list;
#endif
#endif
#if !defined(__off_t_defined)
#include <bits/off_t.h>
#endif /* !defined(__off_t_defined) */
#include <fcntl.h>
//#include <sys/types.h>
#if (_ZERO_SOURCE)
#include <mach/param.h>
//#include <kern/io.h>
//#include <kern/io/buf.h>
#endif

#if defined(__GLIBC__)
typedef struct _IO_FILE FILE;
#else
typedef struct file     FILE;
#endif

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

typedef off_t         fpos_t;
#if (_FILE_OFFSET_BITS == 64)
typedef off_t         fpos64_t;
#else
typedef int64_t       fpos64_t;
#endif

#include <bits/stdio.h>

#define EOF           (-1)
#define BUFSIZ        BUFSIZE
#define FILENAME_MAX  NAME_MAX
#define FOPEN_MAX     OPEN_MAX
/* setvbuf() arguments */
#define _IOFBF        0
#define _IOLBF        1
#define _IONBF        2
#define L_ctermid     128
#define L_cuserid     128
#define L_tmpnam      PATH_MAX
#if (_XOPEN_SOURCE) || (USESVID)
#define P_tmpdir      "/tmp"
#endif

#if !defined(__KERNEL__)

/* buffering */
extern void  setbuf(FILE *__restrict stream, char *__restrict buf);
extern int   setvbuf(FILE *__restrict stream, char **__restrict ptr,
                     int modes, size_t len);
#if (_BSD_SOURCE)
extern void  setbuffer(FILE *__restrict stream, char *__restrict buf,
                       size_t len);
extern void  setlinebuf(FILE *stream);
#endif

/* file operations */
extern int   remove(const char *file);
extern int   rename(const char *file, const char *newname);
extern FILE *fopen(const char *__restrict name, const char *__restrict mode);
extern FILE *freopen(const char *__restrict name, const char *__restrict mode,
                     FILE *__restrict stream);
#if (_POSIX_SOURCE)
extern FILE *fdopen(int fd, const char *mode);
#endif

/* stream operations for <stdio.h> */
extern int   fclose(FILE *stream);
extern int   fflush(FILE *stream);
extern int   fflush_unlocked(FILE *stream);

#if (_GNU_SOURCE) || (_XOPEN_SOURCE >= 700 || _POSIX_C_SOURCE >= 200809L)
/* GNU operations */
extern FILE *fopencookie(void *__restrict cookie, const char *__restrict mode,
                         cookie_io_functions_t iofuncs);
extern FILE *fmemopen(void *buf, size_t len, const char *mode);
extern FILE *open_memstream(char **ptr, size_t *size);
extern int   fcloseall(void);
#endif

#if (_GNU_SOURCE)
extern int   renameat(int oldfd, const char *oldname,
                      int newfd, const char *newname);

#endif

extern FILE *tmpfile(void);
extern char *tmpnam(char *str);

#if (_REENTRANT)
extern char *tmpnam_r(char *str);
#endif

#if (USESVID) || (_XOPEN_SOURCE)
/*
 * generate unique temporary filename
 * - use up to five characters of prefix if not NULL
 * - pick a directory for this file
 *   - check TMPDIR environment variable
 *     - if a writable directory, use it
 *   - if dir is not NULL, check the value
 *   - try P_tmpdir
 *   - try "/tmp"
 * - allocate storage for filename with malloc()
 */
extern char *tempnam(const char *dir, const char *prefix);
#endif

/* formatted I/O */
extern int   fprintf(FILE *__restrict stream, const char *__restrict fmt, ...);
extern int   printf(const char *__restrict fmt, ...);
extern int   sprintf(char *__restrict buf, const char *__restrict fmt, ...);
extern int   vfprintf(FILE *__restrict buf, const char *__restrict fmt,
                      __stdio_va_list arg);
extern int   vprintf(const char *__restrict fmt, __stdio_va_list arg);
extern int   vsprintf(char *__restrict buf, const char *__restrict fmt,
                      __stdio_va_list arg);
#if (_BSD_SOURCE) || (_ISOC99_SOURCE) || (USEUNIX98)
extern int   snprintf(char *__restrict buf, size_t len,
                      const char *__restrict fmt, ...);
extern int   vsnprintf(char *__restrict buf, size_t len,
                       const char *__restrict fmt, __stdio_va_list arg);
#endif
#if (_GNU_SOURCE)
extern int   vasprintf(char **__restrict ptr, const char *__restrict fmt,
                       __stdio_va_list arg);
extern int   asprintf(char **__restrict ptr, const char *__restrict fmt, ...);
extern int   vdprintf(int fd, const char *__restrict fmt, __stdio_va_list arg);
extern int   dprintf(int fd, const char *__restrict fmt, ...);
#endif

extern int   fscanf(FILE *__restrict stream, const char *__restrict fmt, ...);
extern int   scanf(const char *__restrict fmt, ...);
extern int   sscanf(const char *__restrict buf, const char *__restrict fmt,
                    ...);
extern int   vfscanf(FILE *__restrict stream, const char *__restrict fmt,
                     __stdio_va_list arg);
extern int   vscanf(const char *__restrict fmt, __stdio_va_list arg);
extern int   vsscanf(const char *__restrict buf, const char *__restrict fmt,
                     __stdio_va_list arg);

/* character and string operations */
extern int   fgetc(FILE *stream);
extern int   getc(FILE *stream);
extern int   getchar(void);
#define getc(fp) iogetc(fp)
#if (_POSIX_SOURCE)
extern int   getc_unlocked(FILE *stream);
extern int   getchar_unlocked(void);
#endif
extern int   fgetc_unlocked(FILE *stream);
extern int   fputc(int ch, FILE *stream);
extern int   putc(int ch, FILE *stream);
extern int   putchar(int ch);
#define putc(ch, fp) ioputc(ch, fp);
extern int   fputc_unlocked(int ch, FILE *stream);
extern int   putchar_unlocked(int ch);
#if (USESVID) || ((_XOPEN_SOURCE) && !(USEXOPEN2K))
extern int   getw(FILE *stream);
extern int   putw(int w, FILE *stream);
#endif
extern char *fgets(char *__restrict buf, int len, FILE *__restrict stream);
extern char *gets(char *buf);
#if (_GNU_SOURCE)
extern char *fgets_unlocked(char *__restrict buf, int len,
                            FILE *__restrict stream);
#endif
extern int   fputs(const char *__restrict buf, FILE *__restrict stream);
extern int   puts(const char *buf);
extern int   ungetc(int ch, FILE *stream);
#if (_GNU_SOURCE)
extern int   fputs_unlocked(const char *__restrict buf,
                            FILE *__restrict stream);
#endif

/* generic data operations */
extern size_t fread(void *__restrict buf, size_t size, size_t n,
                    FILE *__restrict stream);
extern size_t fwrite(const void *__restrict buf, size_t size, size_t n,
                     FILE *__restrict stream);
extern size_t fread_unlocked(void *__restrict buf, size_t size, size_t n,
                             FILE *__restrict stream);
extern size_t fwrite_unlocked(const void *__restrict buf, size_t size, size_t n,
                              FILE *__restrict stream);

/* seek operations */
extern int  fseek(FILE *stream, long ofs, int whence);
extern long ftell(FILE *stream);
extern void rewind(FILE *stream);
#if (USEXOPEN2K)
extern int fseeko(FILE *stream, off_t ofs, int whence);
extern off_t ftello(FILE *stream);
#endif
extern int fgetpos(FILE *__restrict stream, fpos_t *__restrict pos);
extern int fsetpos(FILE *stream, const fpos_t *pos);
#if (_FILE_OFFSET_BITS == 64)
#define fgetpos64(s, p) fgetpos(s, p)
#define fsetpos64(s, p) fsetpos(s, p)
#else
extern int fgetpos64(FILE *__restrict stream, fpos64_t *__restrict pos);
extern int fsetpos64(FILE *__restrict stream, fpos64_t *__restrict pos);
#endif

/* errors and file attributes */
extern void clearerr(FILE *stream);
extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void clearerr_unlocked(FILE *stream);
extern int feof_unlocked(FILE *stream);
extern int ferror_unlocked(FILE *stream);
extern void perror(const char *str);
#if (_POSIX_SOURCE)
extern int fileno(FILE *stream);
#endif
extern int fileno_unlocked(FILE *stream);
#if (USEPOSIX2) || (USESVID) || (USEBSD)
extern FILE *popen(const char *cmd, const char *mode);
extern int pclose(FILE *stream);
#endif

/* miscellaneous operations */
#if (_POSIX_SOURCE)
extern char *ctermid(char *buf);
#endif
#if (_XOPEN_SOURCE)
extern char *cuserid(char *buf);
#endif

/* lock operations */
#if (_POSIX_SOURCE) || (_XOPEN_SOURCE) || (_BSD_SOURCE) || (_SVID_SOURCE)
extern void flockfile(FILE *stream);
extern int ftrylockfile(FILE *stream);
extern void funlockfile(FILE *stream);
#endif

/* <obstack.h> */
#if (_GNU_SOURCE) && 0
struct obstack; // forward declaration
extern int obstack_printf(struct obstack *__restrict stk,
                          const char *__restrict fmt);
extern int obstack_vprintf(struct obstack *__restrict stk,
                           const char *__restrict fmt,
                           __stdio_va_list arg);
#endif

#if (_GNU_SOURCE)
extern ssize_t getdelim(char **__restrict ptr, size_t *__restrict n,
                        FILE *__restrict stream);
extern ssize_t readline(char **__restrict ptr, size_t *__restrict n,
                        FILE *__restrict stream);
#endif

#endif /* !defined(__KERNEL__) */

#endif /* __STDIO_H__ */

