#ifndef MT_CONF_H
#define MT_CONF_H

/* velho libmt configuration file */

#define MT_STDC_THREAD          0
#define MT_POSIX_THREAD         1
//#define MT_ZEN_THREAD           0
#define MT_ZEN_MUTEX            1
/* #define PTHREAD     1 */

#if 0
#if defined(ZEN_MUTEX)
#include <mt/mtx.h>
#endif
#if defined(ZEN_THREAD)
#include <mt/thr.h>
#endif
#endif

#endif /* MT_CONF_H */

