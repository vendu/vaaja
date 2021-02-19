#ifndef SYS_ZEN_SIGNAL_H
#define SYS_ZEN_SIGNAL_H

/* signal numbers */
#define SIGNONE                 0
#define SIGHUP                  1
#define SIGINT                  2       // C-c
#define SIGQUIT                 3       // C-\
#define SIGILL                  4
#define SIGTRAP                 5
#define SIGABRT                 6
#define SIGBUS                  7
#define SIGFPE                  8
#define SIGKILL                 9
#define SIGUSR1                 10
#define SIGSEGV                 11
#define SIGUSR2                 12
#define SIGPIPE                 13
#define SIGALRM                 14
#define SIGTERM                 15
#define SIGSTKFLT               16
#define SIGCHLD                 17
#define SIGCONT                 18
#define SIGSTOP                 19
#define SIGTSTP                 20      // C-z
#define SIGTTIN                 21
#define SIGTTOU                 22
#define SIGURG                  23
#define SIGVTALRM               26
#define SIGPROF                 27
#define SIGWWINCH               28
#define SIGIO                   29
#define SIGPOLL                 SIGIO
#define SIGPWR                  30
#define SIGSYS                  31
#define SIGUNUSED               31
#define SIGRTBIT                32
#define SIGRT(sig)              (SIGRTBIT | (sig))
#define SIGRT0                  SIGRT(0)
#define SIGRT1                  SIGRT(1)
#define SIGRT2                  SIGRT(2)
#define SIGRT3                  SIGRT(3)
#define SIGRT4                  SIGRT(4)
#define SIGRT5                  SIGRT(5)
#define SIGRT6                  SIGRT(6)
#define SIGRT7                  SIGRT(7)
#define SIGRT8                  SIGRT(8)
#define SIGRT9                  SIGRT(9)
#define SIGRT10                 SIGRT(10)
#define SIGRT11                 SIGRT(11)
#define SIGRT12                 SIGRT(12)
#define SIGRT13                 SIGRT(13)
#define SIGRT14                 SIGRT(14)
#define SIGRT15                 SIGRT(15)
#define SIGRT16                 SIGRT(16)
#define SIGRT17                 SIGRT(17)
#define SIGRT18                 SIGRT(18)
#define SIGRT19                 SIGRT(19)
#define SIGRT20                 SIGRT(20)
#define SIGRT21                 SIGRT(21)
#define SIGRT22                 SIGRT(22)
#define SIGRT23                 SIGRT(23)
#define SIGRT24                 SIGRT(24)
#define SIGRT25                 SIGRT(25)
#define SIGRT26                 SIGRT(26)
#define SIGRT27                 SIGRT(27)
#define SIGRT28                 SIGRT(28)
#define SIGRT29                 SIGRT(29)
#define SIGRT30                 SIGRT(30)
#define SIGRT31                 SIGRT(31)
#define SIGRTLIM                64

#endif /* SYS_ZEN_SIGNAL_H */

