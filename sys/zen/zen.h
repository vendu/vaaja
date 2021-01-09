#ifndef ZEN_ZEN_H
#define ZEN_ZEN_H

#include <sys/zen/conf.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <mach/param.h>
#include <env/cdefs.h>
#include <env/trix.h>
#include <mach/types.h>
#include <mt/tkt.h>
#if defined(__voima__)
#include <sys/zen/bsp/voima.h>
#endif
#include <sys/zen/signal.h>
#include <sys/zen/types.h>
#include <sys/zen/var.h>
#include <sys/zen/mem.h>
//#include <sys/zen/fs0.h>
#include <sys/zen/vfs.h>
#include <sys/zen/util.h>
#if defined(ZEN_USE_PTHREAD)
#include <pthread.h>
#endif

/* system threads */
#define ZEN_KERNEL_THREAD       0       // kernel
#define ZEN_INIT_THREAD         1       // init
#define ZEN_PAGE_THREAD         2       // page-daemon
#define ZEN_BUF_THREAD          3       // buf-daemon
#define ZEN_SYS_THREADS         4       // # of predefined system threads
#define ZEN_USR_PID_MIN         ZEN_SYS_THREADS
#define ZEN_USR_PID_MAX         (ZEN_MAX_TASKS - 1)

extern struct zensys            k_zensys;

#if 0
struct zentask                 *k_getcurtask(void);
C_NORETURN C_INLINE void        k_jmptask(struct v0thr *thr);
#endif

#endif /* ZEN_ZEN_H */

