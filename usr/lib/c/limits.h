/* http://pubs.opengroup.org/onlinepubs/009695399/basedefs/limits.h.html */

/* TODO: add loads of POSIX stuff... =) */

#ifndef __LIMITS_H__
#define __LIMITS_H__

#include <features.h>
#include <mach/param.h>

/*
 * ISO limits.
 */
#define CHAR_BIT                8
#if defined(__GNUC__)
#if defined(__arm__)
#define CHAR_MAX                UCHAR_MAX
#define CHAR_MIN                0
#else /* !defined(__arm__) */
#define CHAR_MAX                SCHAR_MAX
#define CHAR_MIN                SCHAR_MIN
#endif /* __arm__ */
#elif defined(_MSC_VER)
#if defined(_CHAR_UNSIGNED)
#define CHAR_MAX                UCHAR_MAX
#define CHAR_MIN                0
#else
#define CHAR_MAX                SCHAR_MAX
#define CHAR_MIN                SCHAR_MIN
#endif
#endif /* __GNUC__ */
#define SCHAR_MAX               0x7f
#define SCHAR_MIN               (-0x7f - 1)
#define UCHAR_MAX               0xffU
#define SHRT_MIN                (-0x7fff - 1)
#define SHRT_MAX                0x7fff
#define USHRT_MAX               0xffffU
#define INT_MAX                 0xffffffff
#define INT_MIN                 (-0x7fffffff - 1)
#define UINT_MAX                (0xffffffffU)
#if (MACH_WORD_SIZE == 4) || defined(_MSC_VER)
#define LONG_MAX                0xffffffffL
#define LONG_MIN                (-0x7fffffff - 1L)
#define ULONG_MAX               0xffffffffUL
#elif (MACH_WORD_SIZE == 8)
#define LONG_MAX                0x7fffffffffffffffL
#define LONG_MIN                (-0x7fffffffffffffffL - 1L)
#define ULONG_MAX               0xffffffffffffffffUL
#endif
#define LLONG_MAX               0x7fffffffffffffffLL
#define LLONG_MIN               (-0x7fffffffffffffffLL - 1LL)
#define ULLONG_MAX              0xffffffffffffffffULL
#define MB_LEN_MAX              1

#include <limits-posix.h>

/*
 * Default values for possibly indeterminate run-time invariant values
 */
/*
 * POSIX values.
 */
#if !defined(__zen__)
#define ARG_MAX                         _POSIX_ARG_MAX // arg and env bytes to execve()
#define LINK_MAX                        _POSIX_LINK_MAX     // # of links per file
#define OPEN_MAX                        _POSIX_OPEN_MAX // open files per process
#endif
#define ATEXIT_MAX                      32
#define CHILD_MAX                       _POSIX_CHILD_MAX   // processes per real user ID
#define DELAYTIMER_MAX                  _POSIX_DELAYTIMER_MAX
#define HOST_NAME_MAX                   _POSIX_HOSTNAME_MAX
#define LOGIN_NAME_MAX                  _POSIX_LOGIN_NAME_MAX
#define MAX_CANON                       _POSIX_MAX_CANON // canonical input queue size
#define MAX_INPUT                       _POSIX_MAX_INPUT // size of terminal input queue
#define NGROUPS_MAX                     _POSIX_NGROUPS_MAX // # of extra group ID per process
#define PAGE_SIZE                       MACH_PAGE_SIZE
#define PAGESIZE                        MACH_PAGE_SIZE
#define PIPE_BUF                        _POSIX_PIPE_BUF // bytes in atomic write to pipe
#define RE_DUP_MAX                      _POSIX_REDUP_MAX
#define STREAM_MAX                      _POSIX_STREAM_MAX // # of open I/O streams per process
#define SYMLOOP_MAX                     _POSIX_SYMLOOP_MAX
#define NAME_MAX                        _POSIX_NAME_MAX // filename-size (excl. NUL)
#define PATH_MAX                        _POSIX_PATH_MAX // bits in path incl. NUL
#define TZNAME_MAX                      _POSIX_TZNAME_MAX // bytes in timezone names
#define TTY_NAME_MAX                    _POSIX_TTY_NAME_MAX
#define IOV_MAX                         _POSIX_UIO_MAXIOV

/* thread limits */
#define PTHREAD_KEYS_MAX                128
#define PTHREAD_DESTRUCTOR_ITERATIONS   8
#define _POSIX_THREAD_THREADS_MAX       16

#endif /* POSIX */

/*
 * Unix values
 */
#define PASS_MAX                        16 // password length incl. NULL

/*
 * Determinate (compile-time) values.
 */
#if !defined(FOPEN_MAX)
#define FOPEN_MAX               32
#endif
#define SSIZE_MAX               INT32_MAX

#define FD_SET_SIZE             _POSIX_FD_SETSIZE

#if defined(_ZERO_SOURCE)
#include <signal.h>
#endif
#define RTSIG_MAX               NRTSIG
/* TODO: SIGQUEUE_MAX */

#endif /* __LIMITS_H__ */

