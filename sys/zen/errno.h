#ifndef __ZEN_ERRNO_H__
#define __ZEN_ERRNO_H__

#include <zero/cdefs.h>
#include <sys/zen/bits/errno.h>

C_CONST int            *__errnoloc(void);

extern int              __zenerrno;
#define errno         (*__errnoloc())

#define kseterrno(e)  (*__errnoloc() = (e))
#define kclrerrno(e)  (*__errnoloc() = 0)

#endif /* __ZEN_ERRNO_H__ */

