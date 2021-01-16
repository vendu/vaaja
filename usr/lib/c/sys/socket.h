/* zero c library socket network programming interface */

/* http://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html */

#ifndef __SYS_SOCKET_H__
#define __SYS_SOCKET_H__

#include <features.h>
#include <stddef.h>
#if !defined(__KERNEL__)
#include <string.h>
#endif
#if (USEGNU)
#include <sys/types.h>
#endif
#include <zero/cdefs.h>
#include <mach/param.h>
#include <sys/bits/socket.h>

#if defined(__KERNEL__)
#include <kern/malloc.h>
#Define CALLOC(n, sz) kcalloc(n * sz)
#define FREE(adr)     kfree(adr)
#else
#include <stdlib.h>
#define CALLOC(n, sz) calloc(n, sz)
#define FREE(adr)     free(adr)
#endif

#if !defined(__socklen_t_defined)
typedef uint16_t socklen_t;
#define __socklen_t_defined 1
#endif
typedef uint16_t sa_family_t;

extern const socklen_t sockaddrlentab[AF_NFAMILY];

#define SOCK_MAXADDRLEN 255
struct sockaddr {
    sa_family_t sa_family;                      // family
    socklen_t   sa_len;                         // address size
    uint8_t     _res[CLSIZE - sizeof(sa_family_t) - sizeof(socklen_t)];
    char        sa_data[SOCK_MAXADDRLEN];       // address
};

/* control information in raw sockets */
struct sockproto {
    sa_family_t    sp_family;
    unsigned short sp_protocol;
};

#if (USEBSD)

struct osockaddr {
    unsigned short sa_family;
    unsigned char  sa_data[14];
};

struct omsghdr {
    caddr_t       msg_name;
    int           msg_namelen;
    struct iovec *msg_iov;
    int           msg_iovlen;
    caddr_t       msg_accrights;
    int           msg_accrightslen;
};

#endif

#if (USEGNU)
struct ucred {
    pid_t pid;
    uid_t uid;
    gid_t gid;
};
#endif

#define SA_LEN(sa)      ((sa)->sa_len)
struct sockaddr_storage {
    sa_family_t   ss_family;
    socklen_t     sa_len;
    uint8_t       _res[CLSIZE - sizeof(sa_family_t) - sizeof(socklen_t)];
    unsigned char _data[SOCK_MAXADDRLEN];
};

struct msghdr {
    void         *msg_name;	// optional address
    socklen_t     msg_namelen;	// address length
    struct iovec *msg_iov;	// scatter-gather I/O structures
    int           msg_iovlen;	// number of scatter-gather I/O structures
    void         *msg_control;	// ancillary data
    socklen_t     msg_controllen; // ancillary data size
    int           msg_flags;    // flags
};

struct cmsghdr {
    socklen_t      cmsg_len;
    int            cmsg_level;
    int            cmsg_type;
    unsigned char  cmsg_data[1];
};

struct linger {
    int l_onoff;	// on/off
    int l_linger;	// linger time in seconds
};

#if !defined(__KERNEL__)

extern int     accept(int sock, struct sockaddr *adr, socklen_t *adrlen);
extern int     bind(int sock, const struct sockaddr *adr, socklen_t adrlen);
extern int     connect(int sock, const struct sockaddr *adr, socklen_t adrlen);
extern int     getpeername(int sock, struct sockaddr *adr, socklen_t *adrlen);
extern int     getsockname(int sock, struct sockaddr *adr, socklen_t *adrlen);
extern int     getsockopt(int sock, int level, int name, void *val, socklen_t *len);
extern int     listen(int sock, int backlog);
extern ssize_t recv(int sock, void *buf, size_t len, int flags);
extern ssize_t recvfrom(int sock, void *buf, size_t len, int flags,
                        struct sockaddr *adr, socklen_t *adrlen);
extern ssize_t recvmsg(int sock, struct msghdr *msg, int flags);
extern ssize_t send(int sock, const void *msg, size_t len, int flags);
extern ssize_t sendmsg(int sock, const struct msghdr *msg, int flags);
extern ssize_t sendto(int sock, const void *msg, size_t len, int flags,
                      const struct sockaddr *destadr, socklen_t destlen);
extern int     setsockopt(int sock, int level, int name,
                          const void *val, socklen_t len);
extern int     shutdown(int sock, int how);
extern int     socket(int domain, int type, int proto);
extern int     socketpair(int domain, int type, int proto, int sockvec[2]);
#if (USEXOPEN2K)
/* determine wheter socket is at an out-of-band mark */
extern int     sockatmark(int fd);
#endif

static __inline__ struct sockaddr *
sockaddr_alloc(sa_family_t af)
{
    struct sockaddr *adr = NULL;

    if (__saisfamily(af)) {
        adr = CALLOC(1, sizeof(struct sockaddr));
        if (adr) {
            adr->sa_family = af;
            adr->sa_len = sockaddrlentab[af];
        }
    }

    return adr;
}

static __inline__ struct sockaddr *
sockaddr_copy(struct sockaddr *dest, const struct sockaddr *src)
{
    struct sockaddr *adr = NULL;
    sa_family_t      af = dest->sa_family;
    socklen_t        len = sizeof(struct sockaddr);

    if (dest->sa_family == src->sa_family) {
        memcpy(dest, src, len);
        adr = dest;
    }

    return adr;
}

static __inline__ struct sockaddr *
sockaddr_dup(const struct sockaddr *src)
{
    struct sockaddr *adr = CALLOC(1, sizeof(struct sockaddr));

    if (adr) {
        adr->sa_len = sockaddrlentab[src->sa_family];
        memcpy(adr, src, offsetof(struct sockaddr, sa_data) + src->sa_len);
    }

    return adr;
}

static __inline__ int
sockaddr_cmp(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
    sa_family_t af1 = sa1->sa_family;
    sa_family_t af2 = sa2->sa_family;
    socklen_t   len1 = sa1->sa_len;
    socklen_t   len2 = sa2->sa_len;
    int         res;

    if (af1 != af2) {

        return af1 - af2;
    }
    res = memcmp(sa1, sa2, min(len1, len2));
    if (!res) {

        return res;
    }
    res = len1 - len2;

    return res;
}

#define sockaddr_free(sa) FREE(sa)

static __inline__ void *
sockaddr_addr(struct sockaddr *adr, socklen_t *retlen)
{
    void *ret = NULL;

    if (__saisfamily(adr->sa_family)) {
        *retlen = adr->sa_len;
        ret = adr->sa_data;
    }

    return ret;
}

static __inline__ const void *
sockaddr_const_addr(const struct sockaddr *adr, socklen_t *retlen)
{
    const void *ret = NULL;

    if (__saisfamily(adr->sa_family)) {
        *retlen = adr->sa_len;
        ret = adr->sa_data;
    }

    return ret;
}

#endif /* !defined(__KERNEL__) */

#endif /* __SYS_SOCKET_H__ */

