#ifndef API_ERRNO_H
#define API_ERRNO_H

#if defined(__zen__)
#include <sys/zen/errno.h>
#endif

const char                     *const sys_errlist[];
const int                       sys_nerr;
int                             errno;      /* see errno(3) */

#endif /* API_ERRNO_H */

