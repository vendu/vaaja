#include <api/zen/conf.h>
#include <stdlib.h>
#include <errno.h>
#include <env/cdefs.h>
#include <sys/zen/errno.h>

#define kseterrno(e)  (*__errnoloc() = (e))
#define kclrerrno(e)  (*__errnoloc() = 0)

#if ((!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 201112L)        \
      || defined(__STDC_NO_THREADS__))                                  \
     && defined(ZEN_POSIX_THREAD))

#include <pthread.h>

const char                     *const sys_errlist[];
int                             sys_nerr;
int                             errno;       /* Not really declared this way; see errno(3) */

static pthread_once_t   __once = PTHREAD_ONCE_INIT;
static pthread_key_t    __key;
int                    *__zenerrno;

static void
__errnoinit(void)
{
    pthread_key_create(&__key, free);

    return;
}

C_CONST int *
__errnoloc(void)
{
    int    *ptr = NULL;

    pthread_once(&__once, __errnoinit);
    ptr = pthread_get_specific(__key);
    if (!ptr) {
        ptr = malloc(sizeof(int));
        if (ptr) {
            pthread_setspecific(__key, ptr);
            __zenerrno = ptr;
        }
    }

    return __zenerrno;
}

#else

C_CONST int *
__errnoloc(void)
{
    int    *ptr = &__zenerrno;

    return ptr;
}

