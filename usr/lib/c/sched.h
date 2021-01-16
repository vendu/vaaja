#ifndef __SCHED_H__
#define __SCHED_H__

#if !defined(__KERNEL__)

#include <bits/sched.h>

extern int sched_yield(void);

#endif /* !defined(__KERNEL__) */

#define CPU_SETSIZE __CPU_SETSIZE

#endif /* __SCHED_H__ */

