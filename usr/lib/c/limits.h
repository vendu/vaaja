/* http://pubs.opengroup.org/onlinepubs/009695399/basedefs/limits.h.html */

/* TODO: add loads of POSIX stuff... =) */

#ifndef __LIMITS_H__
#define __LIMITS_H__

#include <features.h>
#include <mach/param.h>

/*
 * ISO limits.
 */
#if (__GNUC__)
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
#define SHRT_MAX      		    0x7fff
#define USHRT_MAX     		    0xffffU
#define INT_MAX       		    0xffffffff
#define INT_MIN                 (-0x7fffffff - 1)
#define UINT_MAX                (0xffffffffU)
#if (LONGSIZE == 4)
#define LONG_MAX   		        0xffffffffL
#define LONG_MIN                (-0x7fffffff - 1L)
#define ULONG_MAX  		        0xffffffffUL
#elif (LONGSIZE == 8)
#define LONG_MAX   		        0x7fffffffffffffffL
#define LONG_MIN                (-0x7fffffffffffffffL - 1L)
#define ULONG_MAX  		        0xffffffffffffffffUL
#endif
#define LLONG_MAX     	        0x7fffffffffffffffLL
#define LLONG_MIN               (-0x7fffffffffffffffLL - 1LL)
#define ULLONG_MAX    		    0xffffffffffffffffULL
#define MB_LEN_MAX              1

/* TODO: feature-macro these out etc... */
#if defined(_ZERO_SOURCE) && defined(MACH_PAGE_SIZE)
#define PAGESIZE                MACH_PAGE_SIZE
#endif
#if defined(PAGESIZE)
#define PAGE_SIZE               PAGESIZE
#endif
#endif /* !defined(_POSIX_SOURCE) */

#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 200112L)

/*
 * POSIX limits.
 */
/*
 * Invariant minimum values.
 */
#define _POSIX_ARG_MAX          4096
#define _POSIX_CHILD_MAX        6
#define _POSIX_HOST_NAME_MAX    255
#define _POSIX_LINK_MAX         8
#define _POSIX_LOGIN_NAME_MAX   9         // includes terminating NUL
#define _POSIX_MAX_CANON        255
#define _POSIX_MAX_INPUT        255
#define _POSIX_NAME_MAX         14
#define _POSIX_NGROUPS_MAX      16	// number of supplementary group IDs if available
#define _POSIX_OPEN_MAX         16
#define _POSIX_PATH_MAX         255
#define _POSIX_PIPE_BUF         512
#define _POSIX_RE_DUP_MAX       255
#define _POSIX_SSIZE_MAX        32767
#define _POSIX_STREAM_MAX       8
#define _POSIX_SYMLINK_MAX      255     // # of bytes in symbolic link
#define _POSIX_SYMLOOP_MAX      8
#define _POXIX_TTY_NAME_MAX     9
#define _POSIX_TZNAME_MAX       3
/*
 * Default values for possibly indeterminate run-time invariant values
 */
/*
 * POSIX values.
 */
#define ARG_MAX                 65535 	// arg and env bytes to exec functions
#define ATEXIT_MAX              32
#define CHILD_MAX               256     // # of processes per real user ID
#define HOST_NAME_MAX           255
#define LINK_MAX                127     // # of links per file
#define LOGIN_NAME_MAX          9
#define MAX_CANON               255     // size of canonical input queue
#define MAX_INPUT               255     // # size of type-ahead buffer
#define NGROUPS_MAX             16      // # of extra group ID per process
#define OPEN_MAX                32768   // # of open files per process
#define PIPE_BUF                4096    // # of bytes in atomic write to pipe
#define RE_DUP_MAX              255
#define STREAM_MAX              FOPEN_MAX // # of open I/O streams per process
#define SYMLOOP_MAX             8
#define NAME_MAX                255     // # of bytes in file names
#define PATH_MAX                4096    // bits in path with terminating NUL
#define TZNAME_MAX              3       // # of bytes in timezone names
#define TTY_NAME_MAX            9
#define IOV_MAX                 1024

#endif /* POSIX */

/*
 * Unix values
 */
#define PASS_MAX                15      // significant characters in password
#define LOGIN_MAX               15      // significant characters in login name

/*
 * Determinate (compile-time) values.
 */
#define FOPEN_MAX               INT16_MAX
#define SSIZE_MAX               INT32_MAX

#if !defined(NFDBITS)
#define _POSIX_FD_SET_SIZE      32768
#else
#define _POSIX_FD_SET_SIZE      NFDBITS
#endif

#if (_ZERO_SOURCE)
#include <signal.h>
#endif
#define RTSIG_MAX               NRTSIG
/* TODO: SIGQUEUE_MAX */

#endif /* __LIMITS_H__ */

