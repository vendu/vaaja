#ifndef __ZEN_NET_H__
#define __ZEN_NET_H__

#include <zen/net/inet.h>

struct zennetcon {
    int                 sock;
    struct sockaddr     adr;
    socklen_t           adrlen;
};

#endif /* __ZEN_NET_H__ */

