#include <mt/mt.h>

long
mtwaitsem(mtsem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            mtyieldthr();
        }
#elif defined(MTFMTX)
        while (!mttryfmtx(&sem->lk)) {
            mtyieldthr();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif
            mtyieldthr();
        }
    } while (1);

    return 0;
}

long
semtrywait(mtsem *sem)
{
    do {
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            mtyieldthr();
        }
#elif defined(MTfMTX)
        while (!mttryfmtx(&sem->lk)) {
            mtyieldthr();
        }
#endif
        if (sem->val > 0) {
            sem->val--;
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif

            return 0;
        } else {
#if defined(PTHREAD)
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
#if defined(PTHREAD)
        while (!pthread_mutex_trylock(&sem->lk)) {
            mtyieldthr();
        }
#elif defined(MTFMTX)
        while (mttryfmtx(&sem->lk)) {
            mtyieldthr();
        }
#endif
        if (!sem->val) {
#if defined(PTHREAD)
            pthread_mutex_unlock(&sem->lk);
#elif defined(MTFMTX)
            mtunlkfmtx(&sem->lk);
#endif
            mtyieldthr();
        } else if (sem->val != MTSEM_INITVAL) {
            sem->val++;
#if defined(PTHREAD)
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

