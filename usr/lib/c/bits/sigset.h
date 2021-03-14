#ifndef API_ZEN_SIGSET_H
#define API_ZEN_SIGSET_H

#include <mach/param.h>
#if (MACH_WORD_SIZE == 32)
#include <bits/sigset32.h>
#else
#include <bits/sigset64.h>
#endif

#define __sigisvalid(sig)                                               \
    (((sig) >= 0) && ((sig) < SIGRTMAX))
#define __sigisnormal(sig)                                              \
    (((sig) > 0) && ((sig) < SIGRTMIN))
#define __sigisrt(sig)                                                  \
    (((sig) >= SIGRTMIN) && ((sig) <= SIGRTMAX))

#endif /* API_ZEN_SIGSET_H */

