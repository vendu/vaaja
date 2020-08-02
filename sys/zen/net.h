#ifndef __SYS_ZEN_NET_H__
#define __SYS_ZEN_NET_H__

#include <zen/net/inet.h>

struct zennetcon {
    int                 sock;
    struct sockaddr     adr;
    socklen_t           adrlen;
};

#endif /* __SYS_ZEN_NET_H__ */

