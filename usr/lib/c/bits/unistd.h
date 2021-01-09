#ifndef __BITS_UNISTD_H__
#define __BITS_UNISTD_H__

#include <features.h>
#include <stdint.h>
#include <env/trix.h>

/* types (hopefully) not declared yet... :) */
#if (defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE)) && !defined(__socklen_t_defined)
/* TODO: check what this type is for and what it should be :) */
typedef uint16_t socklen_t;
#define __socklen_t_defined 1
#endif

/* name-arguments for sysconf() */
/*
 * TODO: POSIX.1-2004
 * ------------------
 * NOTE: POSIX.2-stuff is not included yet
 *
 * AIO_LISTIO_MAX
 * AIO_MAX
 * AIO_PRIODELTA_MAX
 * ATEXIT_MAX
 * DELAYTIMER_MAX
 * IOV_MAX
 * NGROUPS_MAX
 * _SC_GETR_R_SIZE_MAX  // maximum buffer size for getgrgid_r(), getgrnam_r()
 * _C_GETPW_R_SIZE_MAX  // maximum buffer size for getpwuid_r(), getpwnam_r()
 * MQ_OPEN_MAX
 * MQ_PRIO_MAX
 * _POSIX_ADVISORY_INFO
 * _POSIX_BARRIERS
 * _POSIX_ASYNCHRONOUS_IO
 * _POSIX_CLOCK_SELECTION
 * _POSIX_CPUTIME
 * _POSIX_FSYNC
 * _POSIX_IPV6
 * _POSIX_JOB_CONTROL
 * _POSIX_MAPPED_FILES
 * _POSIX_MEMLOCK
 * _POSIX_MEMLOCK_RANGE
 * _POSIX_MEMORY_PROTECTION
 * _POSIX_MESSAGE_PASSING
 * _POSIX_MONOTONIC_CLOCK
 * _POSIX_PRIORITIZED_IO
 * _POSIX_PRIORITY_SCHEDULING
 * _POSIX_RAW_SOCKETS
 * _POSIX_READER_WRITER_LOCKS
 * _POSIX_REALTIME_SIGNALS
 * _POSIX_REGEXP
 * _POSIX_SAVED_IDS
 * _POSIX_SEMAPHORES
 * _POSIX_SHARED_MEMORY_OBJECTS
 * _POSIX_SHELL
 * _POSIX_SPAWN
 * _POSIX_SPIN_LOCKS
 * _POSIX_SPORADIC_SERVER
 * _POSIX_SS_REPL_MAX
 * _POSIX_SYNCHRONIZED_IO
 * _POSIX_THREAD_ATTR_STACKADDR
 * _POSIX_THREAD_ATTR_STACKSIZE
 * _POSIX_THREAD_CPUTIME
 * _POSIX_THREAD_PRIO_INHERIT
 * _POSIX_THREAD_PRIO_PROTECT
 * _POSIX_THREAD_PRIORITY_SCHEDULING
 * _POSIX_THREAD_PROCESS_SHARED
 * _POSIX_THREAD_SAFE_FUNCTIONS
 * _POSIX_THREAD_SPORADIC_SERVER
 * _POSIX_THREADS
 * _POSIX_TIMEOUTS
 * _POSIX_TIMERS
 * _POSIX_TRACE
 * _POSIX_TRACE_EVENT_FILTER
 * _POSIX_TRACE_EVENT_NAME_MAX
 * _POSIX_TRACE_INHERIT
 * _POSIX_TRACE_LOG
 * _POSIX_TRACE_NAME_MAX
 * _POSIX_TRACE_SYS_MAX
 * _POSIX_TRACE_USER_EVENT_MAX
 * _POSIX_TYPED_MEMORY_OBJECTS
 * _POSIX_VERSION
 * _POSIX_V6_ILP32_OFF32
 * _POSIX_V6_ILP32_OFFBIG
 * _POSIX_V6_LP64_OFF64
 * _POSIX_V6_LPBIG_OFFBIG
 * _POSIX2_C_BIND
 * _SC_2_C_BIND
 * _POSIX2_CHAR_TERM
 * PTHREAD_DESTRUCTOR_ITERATIONS
 * PTHREAD_KEYS_MAX
 * PTHREAD_STACK_MIN
 * PTHREAD_THREADS_MAX
 * RTSIG_MAX
 * SEM_NSEMS_MAX
 * SEM_VALUE_MAX
 * SIGQUEUE_MAX
 * STREAM_MAX
 * TIMER_MAX
 * _XBS5_ILP32_OFF32 (LEGACY)
 * _SC_XBS5_ILP32_OFF32 (LEGACY)
 * _XBS5_ILP32_OFFBIG (LEGACY)
 * _XBS5_LP64_OFF64 (LEGACY)
 * _XBS5_LPBIG_OFFBIG (LEGACY)
 * _XOPEN_CRYPT
 * _XOPEN_ENH_I18N
 * _XOPEN_LEGACY
 * _XOPEN_REALTIME
 * _XOPEN_REALTIME_THREADS
 * _XOPEN_SHM
 * _XOPEN_STREAMS
 * _XOPEN_UNIX
 * _XOPEN_VERSION
 */
/* POSIX.1 values */
#define _SC_OS_VERSION       0  // OS [kernel] version
#define _SC_VERSION          1  // POSIX version
#define _SC_ARG_MAX          2  // minimum of _POSIX_ARG_MAX
#define _SC_CHILD_MAX        3  // minimum of _POSIX_CHILD_MAX
#define _SC_HOST_NAME_MAX    4  // minimum of _POSIX_LOGIN_NAME_MAX
#define _SC_LOGIN_NAME_MAX   5  // # of bytes in login name
#define _SC_CLK_TCK          6  // # of ticks per second
#define _SC_OPEN_MAX         7  // # of open files
#define _SC_PAGE_SIZE        8  // page size in bytes
#define _SC_PAGESIZE         _SC_PAGESIZE // synonymous with _SC_PAGESIZE
#define _SC_RE_DUP_MAX       9  // regexec(), regcomp()
#define _SC_STREAM_MAX       10 // FOPEN_MAX
#define _SC_SYMLOOP_MAX      11 // max # of symlinks in pathnames
#define _SC_TTY_NAME_MAX     12 // # of bytes in a terminal device name
#define _SC_TZNAME_MAX       13 // # of bytes in timezone name
/* POSIX.2 values */
#define _SC_BC_BASE_MAX      14
#define _SC_BC_DIM_MAX       15
#define _SC_BC_SCALE_MAX     16
#define _SC_BC_STRING_MAX    17
#define _SC_COLL_WEIGHTS_MAX 18
#define _SC_EXPR_NEST_MAX    19
#define _SC_LINE_MAX         20
#define _SC_2_VERSION        21
#define _SC_2_C_DEV          22
#define _SC2_FORT_DEV        23
#define _SC_2_LOCALEDEF      24
#define _SC2_SW_DEV          25
#define MAXSYSCONF           25
/* non-standard values */
/* Unix/Linux extensions */
#if defined(_GNU_SOURCE)
#define _SC_PHYS_PAGES       -1         // # of pages of physical memory
#define _SC_AVPHYS_PAGES     -2         // # of available physical pages
#define _SC_NPROCESSORS_CONF -3         // # of processors configured
#define _SC_NPROCESSORS_ONLN -4         // # of processors currently online
#endif
/* Zero extensions */
#if defined(_ZERO_SOURCE)
#define _SC_CACHELINE_SIZE   (-5)       // size of [memory] cacheline in bytes
#define _SC_NTLB             (-6)       // size of [memory] cacheline in bytes
#define _SC_L1_INST_SIZE     (-7)       // L1 cache size in bytes
#define _SC_L1_INST_NWAY     (-8)       // L1 cache parameter
#define _SC_L1_DATA_SIZE     (-9)       // L1 cache size in bytes
#define _SC_L1_DATA_NWAY     (-10)      // L1 cache parameter
#define _SC_L2_SIZE          (-11)      // L2 cache size in bytes
#define _SC_L2_NWAY          (-12)      // L2 cache parameter
#define _SC_BUF_BLK_SIZE     (-13)      // buffer block size
#define MINSYSCONF           (-13)
#endif
//#define NNEGSYSCONF          (-MINSYSCONF)
#define NSYSCONF             (26 + zeroabs(MINSYSCONF)) // 1 bigger than any ID

#if defined(_XOPEN_SOURCE_EXTENDED) && !defined(F_LOCK)
/* these macros also appear in <fcntl.h> - keep the files consistent */
#define F_ULOCK 0       // unlock section of file
#define F_LOCK  1       // lock section
#define F_TLOCK 2       // try lock
#define F_TEST  3       // test lock
#endif

#if defined(_GNU_SOURCE)
#define TEMP_FAILURE_RETRY(expr)                                        \
    (__extension__                                                      \
     ({ long _res;                                                      \
         do {                                                           \
             _res = (long)(expr);                                       \
         } while (_res == -1L && errno == EINTR);                       \
         _res;                                                          \
     }))
#endif

#endif /* __BITS_UNISTD_H__ */

