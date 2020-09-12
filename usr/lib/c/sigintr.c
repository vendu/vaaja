#include <signal.h>
#include <errno.h>

int
siginterrupt(int sig, int intr)
{
#if defined(SA_RESTART)
    extern sigset_t     _sigintr C_HIDDEN;  /* defined in signal.c */
    struct sigaction    action;

    if (sig < 0 || sig > _NSIG) {
        errno = EINVAL;

        return -1;
    }
    if (intr) {
        sigaddset(&_sigintr, sig);
        action.sa_flags &= ~SA_RESTART;
    } else {
        sigdelset(&_sigintr, sig);
        action.sa_flags |= SA_RESTART;
    }
    if (sigaction(sig, &action, NULL) < 0) {

        return -1;
    }

    return 0;
#else
    errno = ENOSYS;

    return -1;
#endif
}

