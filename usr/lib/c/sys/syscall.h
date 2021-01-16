#ifndef __SYS_SYSCALL_H__
#define __SYS_SYSCALL_H__

#include <features.h>
#if defined(_GNU_SOURCE)

#include <kern/syscallnum.h>

long syscall(long num, ...);

#endif /* defined(_GNU_SOURCE) */

#endif /* __SYS_SYSCALL_H__ */

