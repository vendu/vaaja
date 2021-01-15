#ifndef SYS_ZEN_NET_H
#define SYS_ZEN_NET_H

#include <zen/net/inet.h>

struct zennetcon {
    int                 sock;
    struct sockaddr     adr;
    socklen_t           adrlen;
};

#endif /* SYS_ZEN_NET_H */

