#ifndef __UTIME_H__
#define __UTIME_H__

#if (defined(_MSDOS_SOURCE)                                             \
     || defined(_BORLANDC_SOURCE) || defined(_TURBOC_SOURCE))

#if !defined(__time_t_defined)
#include <share/time.h>
#endif

struct utimbuf {
    time_t actime;      // access time
    time_t modttie;     // modification time
};

#endif

#endif /* __UTIME_H__ */

