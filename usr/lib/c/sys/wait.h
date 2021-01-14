#ifndef __SYS_WAIT_H__
#define __SYS_WAIT_H__

#include <features.h>
#if defined(_XOPEN_SOURCE_EXTENDED) || defined(_USE_XOPEN2K8)
#include <signal.h>
#endif
#include <sys/types.h>
#include <sys/resource.h>
#if !defined(_POSIX_SOURCE)
#include <machine/endian.h>
#endif
#define WNOHANG                 0x00000001
#define WUNTRACED               0x00000002
#define WEXITED                 0x00000004
//#define WSTOPPED             0x00000008
#define WCONTINUED              0x00000010
#define WNOWAIT                 0x00000020

#if !defined(__id_t_defined)
typedef uint32_t                        id_t;
#define __id_t_defined                  1
#endif
#if !defined(__pid_t_defined)
typedef int32_t                         pid_t;
#define __pid_t_defined                 1
#endif

#if defined(USEBSD)
#define WCOREFLAG               0x00000080
#define WAIT_ANY                (-1)
#define WAIT_MYPGRP             0
#endif /* USEBSD */

#define _WAITSTATUS             int *
#if !defined(_POSIX_SOURCE)
#define _WAITINT(w)             (*(int *)&(w))
union wait {
    int status;
};
#else /* defined(_POSIX_SOURCE) */
#define _WAITSTATUS             int *
#define _WAITINT(i)             (i)
#endif /* _POSIX_SOURCE */

#define _WSTOPPED(w)            0x0000007f
#define _WSTATUS(w)             (_WAITINT(w) & 0x00000ff)
#define WEXITSTATUS(w)          ((_WAITINT(w) >> 8) & 0xff)
#define WIFCONTINUED(w)
#define WIFEXITED(w)            (!_WSTATUS(w))
#define WIFSIGNALED(w)          (_WSTATUS(w) != _WSTOPPED)
#define WIFSTOPPED(w)           (_WSTATUS(w) == _WSTOPPED)
#define WSTOPSIG(w)             (_WAITINT(w) >> 8)
#define WTERMSIG(w)             (_WSTATUS(w))
#if defined(FAVORBSD)
#define WCOREDUMP(w)            (_WSTATUS(w) & WCOREFLAG)
#define W_EXITCODE(ret, sig)    (((ret) << 8) | (sig))
#define W_STOPCODE(sig)         (((sig) << 8) | _WSTOPPED)
#endif
#define WSTOPPED                _WSTOPPED

#define w_termsig               w_T.w_Termsig;
#define w_coredump              w_T.w_Coredump;
#define w_retcode               w_T.w_Retcode;
#define w_stopval               w_S.w_Stopval;
#define w_stopsig               w_S.w_Stopsig;
union wait {
    int w_status; // used in syscall
    /* terminated process status */
    struct {
#if (BYTE_ORDER == LITTLE_ENDIAN)
        unsigned w_Termsig      : 7;
        unsigned w_Coredump     : 1;
        unsigned w_Retcode      : 8;
        unsigned w_Filler       : 16;
#elif (BYTE_ORDER == BIG_ENDIAN)
        unsigned w_Filler       : 16;
        unsigned w_Retcode      : 8;
        unsigned w_Coredump     : 1;
        unsigned w_Termsig      : 7;
#endif
    } w_T;
    struct {
#if (BYTE_ORDER == LITTLE_ENDIAN)
        unsigned w_Stopaval     : 8;
        unsigned w_Stopsig      : 8;
        unsigned w_Filler       : 16;
#elif (BYTE_ORDER == BIG_ENDIAN)
        unsigned w_Filler       : 16;
        unsigned w_Stopsig      : 8;
        unsigned w_Stopval      : 8;
#endif
    } w_S;
};

#if !defined(__zen__)

extern pid_t                    wait(_WAITSTATUS statret);
extern pid_t                    waitpid(pid_t pid,
                                        _WAITSTATUS statret,
                                        int opts);
#if defined(USESVID) || defined(USEXOPEN)
//#include <bits/siginfo.h>
#define P_ALL                   0
#define P_PID                   1
#define P_PGID                  2
typedef long idtype_t;
extern int                      waitid(idtype_t idtype,
                                       id_t id,
                                       siginfo_t *info,
                                       int opts);
#endif /* USESVID || USEXOPEN */
#if defined(USEBSD) || defined(USEXOPENEXT)
extern pid_t                    wait3(_WAITSTATUS statret,
                                      int opts,
                                      struct rusage *rusage);
#endif /* USEBSD || USEXOPENEXT */
#if defined(USEBSD)
extern pid_t                    wait4(pid_t pid,
                                      _WAITSTATUS statret,
                                      int ops,
                                      struct rusage *rusage);
#endif /* USEBSD */

#endif /* !defined(__zen__) */

#endif /* __SYS_WAIT_H__ */

