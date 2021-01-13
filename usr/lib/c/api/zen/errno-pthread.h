#ifndef API_ZEN_ERRNO_PTHREAD_H
#define API_ZEN_ERRNO_PTHREAD_H

#include <env/cdefs.h>
#include <sys/zen/errno.h>

C_CONST int            *__errnoloc(void);

extern int             __zenerrno;
#define errno         (*__errnoloc())

#define kseterrno(e)  (*__errnoloc() = (e))
#define kclrerrno(e)  (*__errnoloc() = 0)

#endif /* API_ZEN_ERRNO_PTHREAD_H */

