#include <signal.h>

sigset_t                _sigintr C_HIDDEN;

sighandler_t
ssignal(int sig, sighandler_t action)
{
    sighandler_t    old = signal(sig, action);

    return old;
}

int
gsignal(int sig)
{
    int             retval;
    sighandler_t    func = signal(sig, SIG_DFL);

    if (!func || func == SIG_DFL) {

        return 0;
    } else if (func == SIG_IGN) {

        return 1;
    }
    retval = func(sig);

    return retval;
}

