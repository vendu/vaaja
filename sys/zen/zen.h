#ifndef __SYS_ZEN_ZEN_H__
#define __SYS_ZEN_ZEN_H__

#include <sys/zen/types.h>

/* system threads */
#define ZEN_KERNEL_THREAD       0       // kernel
#define ZEN_INIT_THREAD         1       // init
#define ZEN_PAGE_THREAD         2       // page-daemon
#define ZEN_BUF_THREAD          3       // buf-daemon
#define ZEN_SYS_THREADS         4       // # of predefined system threads
#define ZEN_USR_PID_MIN         ZEN_SYS_THREADS
#define ZEN_USR_PID_MAX         (ZEN_MAX_TASKS - 1)

extern struct zensys            k_zensys;

struct zentask                 *k_getcurtask(void);
C_NORETURN C_INLINE void        k_jmptask(struct v0thr *thr);

#endif /* __SYS_ZEN_ZEN_H__ */

