
#ifndef MT_API_PTHREAD_H
#define MT_API_PTHREAD_H

#include <mt/conf.h>

#if defined(MT_POSIX_THREAD)
#define zenthrself()                ((uintptr_t)pthread_self())
#define zenyieldthr()               pthread_yield()
#endif

#endif /* MT_API_PTHREAD_H */
