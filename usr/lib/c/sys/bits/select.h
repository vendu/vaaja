#ifndef __SYS_SELECT_H__
#define __SYS_SELECT_H__

#include <features.h>
#include <api/zen/sigset.h>

extern int                  select(int nfds,
                                   fd_set *C_RESTRICT readfds,
                                   fd_set *C_RESTRICT writefds,
                                   fd_set *C_RESTRICT errorfds,
                                   struct timeval *C_RESTRICT timeout);
#if defined(_USE_XOPEN2K)
extern int                  pselect(int nfds,
                                    fd_set *C_RESTRICT readfds,
                                    fd_set *C_RESTRICT writefds,
                                    fd_set *C_RESTRICT errorfds,
                                    const struct timespec *C_RESTRICT timeout,
                                    const sigset_t *C_RESTRICT sigmask);
#endif /* defined(_USE_XOPEN2K */

#endif /* __SYS_SELECT_H__ */

