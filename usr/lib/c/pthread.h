#ifndef __PTHREAD_H__
#define __PTHREAD_H__

/* mutexes */

#include <features.h>
#include <mt/mtx.h>
#include <mt/spin.h>
#include <bits/pthread.h>

#if !defined(PTHREAD_ONCE_INIT)
#define PTHREAD_ONCE_INIT 0
#endif

#define pthread_mutex_init(mp, atr) mtxinit(mp, atr)
#define pthread_mutex_destroy(mp)   mtxfree(mp)
#define pthread_mutex_trylock(mp)   mtxtrylk(mp)
#define pthread_mutex_lock(mp)      mtxlk(mp)
#define pthread_mutex_unlock(mp)    mtxunlk(mp)

/* spin locks */
int pthread_spin_destroy(pthread_spinlock_t *spin);
int pthread_spin_init(pthread_spinlock_t *spin, int pshared);
int pthread_spin_lock(pthread_spinlock_t *spin);
int pthread_spin_trylock(pthread_spinlock_t *spin);
int pthread_spin_unlock(pthread_spinlock_t *spin);

/* mutex locks */
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                       const pthread_mutexattr_t *restrict atr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/* miscellaneous routines */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int pthread_atfork(void (*prepare)(void),
                   void (*parent)(void), void (*child)(void));
int pthread_setspecific(pthread_key_t key, const void *val);

#endif /* __PTHREAD_H__ */

