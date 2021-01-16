#include <errno.h>
#include <sys/time.h>

/* interval timer routines */

int
getitimer(itimer_which_t which, struct itimerval *val)
{
    ;
}

int
setitimer(itimer_which_t which, const struct itimerval *__restrict newval,
          struct itimerval *__restrict oldval)
{
    ;
}

