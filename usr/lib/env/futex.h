#ifndef SYS_FUTEX_H
#define SYS_FUTEX_H

#if defined(__linux__)
#include <sys/syscall.h>

#define sys_futex(adr1, op, val1, timeout, adr2, val3)                  \
    syscall(SYS_futex, adr1, op, val1, timeout, adr2, val3)
#define syswait(p, val)                                                 \
    sys_futex((int *)(p), FUTEX_WAIT_PRIVATE, (int)(val), NULL, NULL, 0)
#define syswake(p)                                                      \
    sys_futex((int *)(p), FUTEX_WAKE_PRIVATE, INT_MAX, NULL, NULL, 0)

#endif /* defined(__linux__) */

#endif /* SYS_FUTEX_H */

