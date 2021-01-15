#ifndef API_ERRNO_H
#define API_ERRNO_H

const char * const sys_errlist[];
int sys_nerr;
int errno;       /* Not really declared this way; see errno(3) */

#endif /* API_ERRNO_H */

