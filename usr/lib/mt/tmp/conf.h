#ifndef __MT_CONF_H__
#define __MT_CONF_H__

#define ZERO_THREADS    0
#define ZERO_MUTEX      1
#if defined(ZERO_THREADS) || defined(ZERO_MUTEX)
#include <mach/asm.h>
#include <mt/mtx.h>
#define ZERO_MUTEX_TYPE zerofmtx
#elif defined(PTHREAD_MUTEX)
#define ZERO_MUTEX_TYPE pthread_mutex_t
#include <pthread.h>
#endif

#endif /* __MT_CONF_H__ */

