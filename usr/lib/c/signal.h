#ifndef __SIGNAL_H__
#define __SIGNAL_H__

/* TODO: sysconf(): _SC_SIGRT_MIN, _SC_SIGRT_MAX */

#include <features.h>
#include <stdint.h>
#if !defined(__size_t_defined)
typedef uintptr_t               size_t;
#define __size_t_defined        1
#endif
#if !defined(__pid_t_defined)
typedef uint32_t                pid_t;
#define __pid_t_defined         1
#endif
#include <bits/signal.h>
#include <sys/zen/signal.h>
#if defined(_POSIX_SOURCE) && (_POSIX_C_SOURCE >= 199309L)
struct timespec;
#include <time.h>
#endif
#if ((_POSIX_C_SOURCE >= 199506L) || (_XOPEN_SOURCE >= 500) && 0)
//#include <pthread.h>
#endif
#include <mach/param.h>
#include <mach/atomic.h>
#if defined(__zen__)
#include <sys/zen/errno.h>
#endif

typedef void                    (*sighandler_t)(int);

typedef m_atomic_t              sig_atomic_t;

/* special values; standard ones */
#define SIG_ERR                 ((__sighandler_t)-1)
#define SIG_DFL                 ((__sighandler_t)0)
#define SIG_IGN                 ((__sighandler_t)1)
#define SIG_HOLD                ((__sighandler_t)2)
/* commands for sigprocmask() */
#define SIG_BLOCK               0
#define SIG_UNBLOCK             1
#define SIG_SETMASK             2

/* set handler for signal sig; returns old handler */
extern void                     (*signal(int sig, void (*func)(int)))(int);
extern __sighandler_t           __sysv_signal(int sig, __sighandler_t func);
#if defined(_SVID_SOURCE)
extern sighandler_t             ssignal(int sig, sighandler_t action);
extern int                      gsignal(int sig);
#endif
#if defined(_GNU_SOURCE)
extern __sighandler_t           sysv_signal(int sig, __sighandler_t func);
#endif
#if defined(_XOPEN_SOURCE)
extern __sighandler_t           bsd_signal(int sig, __sighandler_t func);
#endif

#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
extern void                     psiginfo(const siginfo_t *info, const char *msg);
void                            psignal(int sig, const char *message);
#endif

#if defined(_POSIX_SOURCE)
/*
 * send signal sig to process or group described by pid
 * - if pid is zero, send sig to all processes in the current one's process
 *   group
 * - if pid < -1, send sig to all processes in the process group -pid
 */
extern int                      kill(pid_t pid, int sig);

#if defined(POSIX_SOURCE) && (_POSIX_C_SOURCE >= 199506L) || (_XOPEN_SOURCE >= 500)
//int pthread_kill(pthread_t thr, int sig);
//int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
#endif

#endif /* _POSIX_SOURCE */

#if (_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/*
 * send signal sig to all processes in the group pgrp
 * - if pid is zero, send sig to all processes in the current one's group
 */
extern int                      killpg(pid_t pgrp, int sig);
#endif /* _BSD_SOURCE || USEXOPENEXT */

extern int                      raise(int sig);
#if defined(_SVID_SOURCE)
extern                          __sighandler_t ssignal(int sig, __sighandler_t func);
extern int                      gsignal(int sig);
#endif /* USESVID */
//extern void                   psignal(int sig);
#if defined(_FAVOR_BSD)
/* set mask to blocked signals, wait for signal, restore the mask */
extern int                      sigpause(int mask);
#elif (_XOPEN_SOURCE)
/* remove sig from signal mask and suspend the process */
#if defined(__GNUC__)
extern int                      sigpause(int sig) __asm__ ("__xpg_sigpause\n");
#else
//#define               sigpause(sig) __sigpause(sig, 1);
#endif
#endif

#if defined(_BSD_SOURCE)

/* none of these functions should be used any longer */
#define sigmask(sig)            (1L << ((sig) - 1))
/* block signals in mask, return old mask */
extern int                      sigblock(int mask);
/* set mask of blocked signals, return old mask */
extern int                      sigsetmask(int mask);
/* return current signal mask */
extern int                      siggetmask(void);

#if !defined(_POSIX_SOURCE)
int                             sigvec(int sig, const struct sigvec *vec,
                                       struct sigvec *oldvec);
#endif

// extern int sigreturn(struct sigcontext *scp);

#endif /* defined(_BSD_SOURCE) */

#if defined(_POSIX_SOURCE)

/* get and/or change set of blocked signals */
extern int              sigprocmask(int how, const sigset_t *restrict set,
                                    sigset_t *restrict oldset);
/* change blocked signals to set, wait for a signal, restore the set */
extern int      sigsuspend(const sigset_t *set);
/* set or examine signal behavior */
extern int      sigaction(int sig, const struct sigaction *restrict act,
                                  struct sigaction *restrict oldact);
/* fetch pending blocked signals */
extern int      sigpending(sigset_t *set);
/* wait for a signal in set */
extern int      sigwait(const sigset_t *set, int *restrict sig);
#if (_POSIX_C_SOURCE >= 199309L)
extern int      sigwaitinfo(const sigset_t *restrict set,
                                    siginfo_t *restrict info);
extern int      sigtimedwait(const sigset_t *restrict set,
                                     siginfo_t *restrict info,
                                     const struct timespec *restrict timeout);
extern int      sigqueue(pid_t pid, int sig, const union sigval val);
#endif /* USEPOSIX199309 */

#endif /* USEPOSIX */

#if defined(_BSD_SOURCE)
extern const char *const        _sys_siglist[_NSIG];
extern const char *const        sys_siglist[_NSIG];
#endif

/*
 * if intr is nonzero, make signal sig interrupt system calls (causing them
 * to fail with EINTR); if intr is zero, make system calls be restarted
 * after signal sig
 */
extern int              siginterrupt(int sig, int intr);

#if (_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
extern int              sigstack(struct sigstack *stk, struct sigstack *oldstk);
extern int              sigaltstack(const stack_t *restrict stk,
                                    stack_t *restrict oldstk);
#else
extern int              sigaltstack(const struct sigaltstack *stk,
                                    struct sigaltstack *oldstk);
#endif

#if (_XOPEN_SOURCE) && 0
#include <ucontext.h>
#endif

#if defined(_XOPEN_SOURCE_EXTENDED)
extern int              sighold(int sig);
extern int              sigrelse(int sig);
extern int              sigignore(int sig);
extern __sighandler_t   sigset(int sig, __sighandler_t func);
#endif /* USEXOPENEXT */

#endif /* __SIGNAL_H__ */

