#ifndef __ZERO_SYS_H__
#define __ZERO_SYS_H__

#if defined(__linux__)
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <limits.h>
#include <sys/time.h>
#endif

#if defined(__linux__)
#define sys_futex(adr1, op, val1, timeout, adr2, val3)                  \
    syscall(SYS_futex, adr1, op, val1, timeout, adr2, val3)
#define syswait(p, val)                                                 \
    sys_futex((int *)(p), FUTEX_WAIT_PRIVATE, (int)(val), NULL, NULL, 0)
#define syswake(p)                                                      \
    sys_futex((int *)(p), FUTEX_WAKE_PRIVATE, INT_MAX, NULL, NULL, 0)
#endif /* defined(__linux__) */

#endif /* __ZERO_SYS_H__ */

