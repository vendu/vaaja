#ifndef MT_API_H
#define MT_API_H

#include <mt/conf.h>

#include <mt/conf.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#if !defined(__zen__)
#include <stdlib.h>
#endif
#include <stdint.h>
#include <endian.h>
#include <errno.h>
#if defined(MT_POSIX_THREAD)
#define _GNU_SOURCE
#include <pthread.h>
#endif
#include <env/cdefs.h>
#include <mach/asm.h>
#include <mach/param.h>
#include <mach/atomic.h>
#include <mach/asm.h>
#include <mt/thr.h>
#include <mt/lk.h>
#include <mt/spin.h>
#include <mt/spinrw.h>
#include <mt/mtx.h>
#include <mt/cond.h>
#include <mt/bar.h>
#include <mt/sem.h>
#include <mt/tkt.h>
#include <mt/priolk.h>

/* zen thread abstraction */

#if defined(MT_POSIX_THREAD)
#include <mt/api-pthread.h>
#elif defined(__zen__)
#define zenyieldthr()               schedyield()
#elif defined(_WIN64) || defined(_WIN32)
#define zenyieldthr()               kYieldProcessor()
#elif defined(__linux__)
#define zenyieldthr()               sched_yield()
#endif

#endif /* MT_API_H */

