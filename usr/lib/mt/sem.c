#include <mt/conf.h>

#if defined(MT_SEMAPHORE)

#include <mt/sem.h>

long
mtwaitsem(mtsem *sem)
{
    do {
#if defined(MT_POSIX_THREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            m_waitspin();
        }
#elif defined(MTFMTX)
        while (!mttryfmtx(&sem->lk)) {
            m_waitspin();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif

            return 0;
        } else {
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif
        }
    } while (1);

    return 0;
}

long
semtrywait(mtsem *sem)
{
    do {
#if defined(MT_POSIX_THREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            m_waitspin();
        }
#elif defined(MTfMTX)
        while (!mttryfmtx(&sem->lk)) {
            m_waitspin();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif

            return 0;
        } else {
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif
            errno = EAGAIN;

            return -1;
        }
    } while (1);

    return 0;
}

long
sempost(mtsem *sem)
{
    do {
#if defined(MT_POSIX_THREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            m_waitspin();
        }
#elif defined(MTFMTX)
        while (mttryfmtx(&sem->lk)) {
            m_waitspin();
        }
#endif
        if (!sem->val) {
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif
        } else if (sem->val != MTSEM_INITVAL) {
            sem->val++;
#if defined(MT_POSIX_THREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif

            return 0;
        } else {
#if defined(EOVERFLOW)
            errno = EOVERFLOW;
#endif

            return -1;
        }
    } while (1);

    return 0;
}

#endif /* defined(MT_SEMAPHORE) */

