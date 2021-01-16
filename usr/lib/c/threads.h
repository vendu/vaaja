#ifndef __THREADS_H__
#define __THREADS_H__

#if !defined(__KERNEL__)

/* https://github.com/jtsiomb/c11threads */

#if (__STDC_VERSION__ < 201112L) || defined(__STDC_NO_THREADS__)

#include <c11/pthread.h>

#endif /* __STDC_VERSION < 201112L || __STDC_NO_THREADS__ */

#endif /* !defined(__KERNEL__) */

#endif /* __THREADS_H__ */

