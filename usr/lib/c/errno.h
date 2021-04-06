#ifndef ERRNO_H
#define ERRNO_H

//#include <api/errno.h>

const char                     *const sys_errlist[];
int                             sys_nerr;
int                             errno;       /* Not really declared this way; see errno(3) */

#if defined(__zen__)
#   include <sys/zen/errno.h>
#endif

#endif /* ERRNO_H */

