#ifndef __SYS_ZEN_SIGNAL_H__
#define __SYS_ZEN_SIGNAL_H__

#define SIGHUP          1
#define SIGINT          2
#define SIGQUIT         3
#define SIGILL          4
#define SIGTRAP         5
#define SIGABRT         6
#define SIGBUS          7
#define SIGFPE          8
#define SIGKILL         9
#define SIGUSR1         10
#define SIGSEGV         11
#define SIGUSR2         12
#define SIGPIPE         13
#define SIGALRM         14
#define SIGTERM         15
#define SIGSTKFLT       16
#define SIGCHLD         17
#define SIGCONT         18
#define SIGSTOP         19
#define SIGTSTP         20
#define SIGTTIN         21
#define SIGTTOU         22
#define SIGURG          23
#define SIGPROF         26
#define SIGWWINCH       28
#define SIGIO           29
#define SIGPOLL         SIGIO
#define SIGPWR          30
#define SIGSYS          31
#define SIGUNUSED       31
#define SIGRTMIN        32
#define SIGRT(sig)      (SIGRTMIN | (sig))
#define SIGRT0          SIGRT(0)
#define SIGRT1          SIGRT(1)
#define SIGRT2          SIGRT(2)
#define SIGRT3          SIGRT(3)
#define SIGRT4          SIGRT(4)
#define SIGRT5          SIGRT(5)
#define SIGRT6          SIGRT(6)
#define SIGRT7          SIGRT(7)
#define SIGRT8          SIGRT(8)
#define SIGRT9          SIGRT(9)
#define SIGRT10         SIGRT(10)
#define SIGRT11         SIGRT(11)
#define SIGRT12         SIGRT(12)
#define SIGRT13         SIGRT(13)
#define SIGRT14         SIGRT(14)
#define SIGRT15         SIGRT(15)
#define _SIGRTMAX       63
#define _NRTSIG         32
#define _NSIG           (_SIGRTMAX + 1)

#endif /* __SYS_ZEN_SIGNAL_H__ */

