#include <stdlib.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <mach/param.h>
#include <mt/mtx.h>

#define AT_QUICK_EXIT_MAX           ATEXIT_MAX

typedef void (*__exitfunc)(void);
typedef void (*__onexitfunc)(void *);

struct __exitfuncs {
    __exitfunc                         *exit[ATEXIT_MAX];
    __exitfunc                         *quickexit[AT_QUICK_EXIT_MAX];
    __onexitfunc                       *onexit[ATEXIT_MAX];
    void                               *args[ATEXIT_MAX];
    volatile m_atomic_t                 nexit;
    volatile m_atomic_t                 nquickexit;
    volatile m_atomic_t                 nonexit;
};

C_NORETURN void
abort(void)
{
    raise(SIGABRT);
}

int
atexit(void (*func)(void))
{
    int                                 ret = -1;
    long                                ndx;

    do {
        ndx = m_fetchadd(&stdlibexitfunc.nexit, 1);
        if (ndx < ATEXIT_MAX) {
            stdlibexitfuncs.exit[ndx] = func;
            
            ret = 0;
        }
    }

    return ret;
}

C_NORETURN int
at_quick_exit(void (*func)(void))
{
    int                                 ret = -1;
    long                                ndx;

    do {
        ndx = m_fetchadd(&stdlibexitfunc.nquickexit, 1);
        if (ndx < ATEXIT_MAX) {
            stdlibexitfuncs.quickexit[ndx] = func;
            
            ret = 0;
        }
    }

    return ret;
}

int
on_exit(void (*func)(int, void *), void *arg)
{
    long                                ret = -1;
    long                                ndx;

    do {
        ndx = m_fetchadd(&stdlibexitfuncs.nonexit, -1);
        if (ndx < ATEXIT_MAX) {
            stdlibexitfuncs.onexit[ndx] = func;
            stdlibexitfuncs.args[ndx] = arg;
            
            ret = 0;
        }
    }

    return ret;
}

void
exit(int status)
{
    long                                ndx;
    __exitfunc                         *func;
    void                               *arg;

    ndx = m_fetchadd(&stdlibexitfuncs.nexit, -1);
    while (ndx >= 0) {
        func = stdlibexitfuncs.exit[ndx];
        func();
        ndx--;
    }
    fflush(NULL);
    fcloseall();

    return status;
}

C_NORETURN void
_exit(int status)
{
    pid_t                               ppid = getppid();

    fcloseall();
    raise(SIGTERM);

    /* NOTREACHED */
}

C_NORETURN void
quick_exit(int status)
{
    long                                ndx;
    __exitfunc                         *func;
    void                               *arg;

    ndx = m_fetchadd(&stdlibexitfuncs.nquickexit, -1);
    while (ndx >= 0) {
        func = stdlibexitfuncs.quickexit[ndx];
        func();
        ndx--;
    }

    return status;
}

