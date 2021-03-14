#ifndef API_ZEN_SIGSET32_H
#define API_ZEN_SIGSET32_H

/* POSIX */
#define sigemptyset(sp)                                                 \
    ((sp)->norm = (sp)->rt = 0)
#define sigfillset(sp)                                                  \
    (((sp)->norm = (sp)->rt = ~UINT32_C(0)), 0)
#define sigaddset(sp, sig)                                              \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (__sigisnormal(sig)                                             \
         ? ((sp)->norm |= (1UL << (sig)))                               \
         : ((sp)->rt |= (1UL << ((sig) - SIGRTMIN))),                   \
         0)))
#define sigdelset(sp, sig)                                              \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (__sigisnormal(sig)                                             \
         ? ((sp)->norm &= ~(1UL << (sig)))                              \
         : ((sp)->rt &= ~(1UL << ((sig) - SIGRTMIN))),                  \
         0)))
#define sigismember(sp, sig)                                            \
    ((!__sigisvalid(sig)                                                \
      ? (__seterrno(EINVAL), -1L)                                       \
      : (__sigisnormal(sig)                                             \
         ? (((sp)->norm >> (sig)) & 0x01)                               \
         : (((sp)->rt >> (sig - SIGRTMIN)) & 0x01))))
#define __sigisemptyset(sp)     (!(sp)->norm && !(sp)->rt)

typedef struct {
    int norm;
    int rt;
} sigset_t;

#endif /* API_ZEN_SIGSET32_H */
