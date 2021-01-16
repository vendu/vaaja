#ifndef __BITS_STDIO_H__
#define __BITS_STDIO_H__

#include <features.h>
#include <stddef.h>
#include <sys/types.h>

#if (_GNU_SOURCE) || (_XOPEN_SOURCE >= 700) || (_POSIX_C_SOURCE >= 200809L)

typedef ssize_t cookie_read_function_t(void *, char *, size_t);
typedef ssize_t cookie_write_function_t(void *, const char *, size_t);
typedef int     cookie_seek_function_t(void *, off64_t *, int);
typedef int     cookie_close_function_t(void *cookie);

/* NOTE: make the seek-field be a NULL-pointer for non-seekable nodes */
struct cookie_io_functions {                // return values
    cookie_read_function_t  *read;      // # of bytes, 0 for EOF, -1 on error
    cookie_write_function_t *write;     // # of bytes, 0 on error
    cookie_seek_function_t  *seek;      // 0 on success, -1 on error
    cookie_close_function_t *close;
};
typedef struct cookie_io_functions cookie_io_functions_t;

#endif /* _GNU_SOURCE */

#endif /* __BITS_STDIO_H__ */

