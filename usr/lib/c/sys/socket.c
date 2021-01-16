#if defined(__ZEROLIBC__)

#include <sys/socket.h>
#include <netinet/in.h>

const socklen_t sockaddrlentab[AF_NFAMILY]
= {
    0,                  // AF_UNSPEC
    0,                  // AF_RAW
    0,                  // AF_LINK
    0,                  // AF_ROUTE
    0,                  // AF_UNIX
    __INADDRSZ,         // AF_INET
    __IN6ADDRSZ,        // AF_INET6
    0,                  // AF_ISO
    0                   // AF_BLUETOOTH
};

#endif /* defined(__ZEROLIBC__) */

