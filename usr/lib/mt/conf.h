#ifndef MT_CONF_H
#define MT_CONF_H

/* velho libmt configuration file */

#define STDC_THREAD             0
#define POSIX_THREAD            0
#define ZERO_THREAD             1
#define ZERO_MUTEX              1
/* #define PTHREAD     1 */

#if (ZERO_MUTEX)
#include <thr/mtx.h>
#endif
#if (ZERO_THREAD)
#include <thr/thr.h>
#endif
#if (POSIX_THREAD)
#include <pthread.h>
#endif

#endif /* MT_CONF_H */

