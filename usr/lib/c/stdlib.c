#include <stdlib.h>
#include <stdio.h>
#include <mach/param.h>
#include <mt/mtx.h>

#define EXIT_MAX_HANDLERS               64
#define AT_QUICK_EXIT_MAX_HANDLERS      32

typedef void (*__exitfunc)(void);

struct __exitfuncs {
    __exitfunc                         *exit[ATEXIT_MAX_HANDLERS];
    void                               *args[ATEXIT_MAX_HANDLERS];
    __exitfunc                         *quick_exit[AT_QUICK_EXIT_MAX_HANDLERS];
    volatile m_atomic_t                 nexit;
    volatile m_atomic_t                 nquickexit;
};

static struct _exitfuncs                stdlibexitfuncs C_ALIGNED(MACH_CL_SIZE);
mtfmtx                                  stdlibinitmtx;
long                                    stdlibinitflg;

C_NORETURN void
abort(void)
{
    raise(SIGABRT);
}

int
atexit(void (*func)(void))
{
    long                                ndx;

    do {
        if (stdlibinitflg) {
            ndx = m_fetchadd(&stdlibexitfuncs.nexit, -1);
            if (ndx >= 0) {
                stdlibexitfuncs.exit[ndx] = func;

                return 0;
            }

            return 1;
        } else if (m_atommttryfmtx(&stdlibinit)) {
            stdlibexitfunc.nexit = EXIT_MAX_HANDLERS - 1;
            stdlibexitfunc.nquickexit = AT_QUICK_EXIT_MAX_HANDLERS - 1;
            stdlibinitflg = 1;
        }
    }
    C_UNREACHABLE;
}

int
on_exit(void (*func)(int, void *), void *arg)
{
    long                                ndx;

    do {
        if (stdlibinitflg) {
            ndx = m_fetchadd(&stdlibexitfuncs.nexit, -1);
            if (ndx >= 0) {
                stdlibexitfuncs.exit[ndx] = func;
                stdlibexitfuncs.args[ndx] = arg;

                return 0;
            }

            return 1;
        } else if (m_atommttryfmtx(&stdlibinit)) {
            stdlibexitfunc.nexit = EXIT_MAX_HANDLERS - 1;
            stdlibexitfunc.nquickexit = AT_QUICK_EXIT_MAX_HANDLERS - 1;
            stdlibinitflg = 1;
        }
    }
    C_UNREACHABLE;
}

void
exit(int status)
{
    long                                ndx = EXIT_MAX_HANDLERS;
    __exitfunc                         *func;
    void                               *arg;

    while (ndx--) {
        func = stdlibexitfuncs.exit[ndx];
        arg = stdlibexitfuncs.args[ndx];
        if (stdlibexitfuncs.args[ndx]) {
            func(status, arg);
        } else {
            func();
        }
    }
    fcloseall();

    return status;
}
