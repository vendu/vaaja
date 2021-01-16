#ifndef __EXECINFO_H__
#define __EXECINFO_H__

#include <features.h>

#if defined(_BSD_SOURCE)
/* use size_t-arguments instead of GNU-style int */
#include <stddef.h>
typedef size_t       __btsize_t;
typedef size_t       __btret_t;
typedef int          __btsymfdret_t;
#else /* !defined(_BSD_SOURCE) */
typedef int          __btsize_t;
typedef int          __btret_t;
typedef void         __btsymfdret_t;
#endif
typedef void *const *__btsymadrlist_t;

#if defined(_GNU_SOURCE) ||defined(_BSD_SOURCE) || defined(_ZERO_SOURCE)
extern uintptr_t _backtrace(void **buf, __btsize_t size, long syms, int fd);
#if defined(_BSD_SOURCE)
extern uintptr_t _btprintf(__btsymadrlist_t buf, __btsize_t size,
                           const char *fmt, int fd);
#endif
#define          backtrace(buf, size)                                   \
    (__btret_t)_backtrace(buf, size, 0, -1)
#define          backtrace_symbols(buf, size)                           \
    (char **)_backtrace(NULL, size, 1, -1)
#define          backtrace_symbols_fd(buf, size, fd)                    \
    (__btsymfdret_t)_backtrace(NULL, size, 1, fd)
#if defined(_BSD_SOURCE)
#define          backtrace_symbols_fmt(buf, size, fmt)                  \
    (char **)_btprintf((__btsymadrlist_t)buf, size, fmt, -1)
#define          backtrace_symbols_fmt_fd(buf, size, fmt, fd)           \
    (int)_btprintf((__btsymadrlist_t)buf, size, fmt, fd)
#endif
#endif /* defined(_GNU_SOURCE) || defined(_BSD_SOURCE) */

#endif /* __EXECINFO_H__ */

