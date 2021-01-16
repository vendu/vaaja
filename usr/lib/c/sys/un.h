#ifndef __SYS_UN_H__
#define __SYS_UN_H__

#include <stddef.h>
#include <sys/socket.h>
#include <zero/cdefs.h>
#include <zero/param.h>
#if (_UNIX_SOURCE)
#include <string.h>
#endif

#define _SUN_PATH_SIZE (256 - CLSIZE)
struct sockaddr_un {
    sa_family_t sun_family;     // AF_UNIX
    socklen_t   sun_len;        // address size in bytes
    uint8_t     _res[CLSIZE - sizeof(sa_family_t) - sizeof(socklen_t)];
    char        sun_path[_SUN_PATH_SIZE] ALIGNED(CLSIZE);
};

#if (_UNIX_SOURCE)
#define SUN_LEN(ptr)                                                    \
    (offsetof(struct sockaddr_un, sun_path)                             \
     + strnlen(ptr->sun_path, sizeof(ptr->sun_path)))
#endif

#endif /* __SYS_UN_H__ */

