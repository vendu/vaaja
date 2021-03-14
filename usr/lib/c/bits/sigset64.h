#ifndef API_ZEN_SIGSET64_H
#define API_ZEN_SIGSET64_H

typedef uint64_t sigset_t;

/* POSIX */
#define sigemptyset(sp)                                                 \
    (*(sp) = 0)
#define sigfillset(sp)                                                  \
    (*(sp) = ~0L)
#define sigaddset(sp, sig)                                              \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1L)                                       \
      : ((sp) |= (1UL << (sig)),                                        \
         0)))
#define sigdelset(sp, sig)                                              \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1L)                                       \
      : ((sp) &= ~(1UL << (sig)),                                       \
         0)))
#define sigismember(sp, sig)                                            \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1)                                        \
      : ((*(sp) & (1UL << (sig)))                                       \
         ? 1                                                            \
         : 0)))

#endif /* API_ZEN_SIGSET64_H */
