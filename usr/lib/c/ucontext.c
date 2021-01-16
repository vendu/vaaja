#include <stdarg.h>
#include <stdint.h>
#include <ucontext.h>

#define CPUIDINFO 1
#define CPUIDFXSR 0x01000000
#define cpuid(op, ptr)                                                  \
    __asm__ __volatile__ ("movl %4, %%eax\n"                            \
                          "cpuid\n"                                     \
                          "movl %%eax, %0\n"                            \
                          "movl %%ebx, %1\n"                            \
                          "movl %%ecx, %2\n"                            \
                          "movl %%edx, %3\n"                            \
                          : "=m" ((ptr)->eax),                          \
                            "=m" ((ptr)->ebx),                          \
                            "=m" ((ptr)->ecx),                          \
                            "=m" ((ptr)->edx)                           \
                          : "i" (op)                                    \
                          : "eax", "ebx", "ecx", "edx")

#if ((defined(__i386__) || defined(__i486__)                            \
      || defined(__i586__) || defined(__i686__))                        \
     && !defined(__x86_64__) && !defined(__amd64__))

#define m_setstack(stk) __asm__ __volatile__ ("movl %0, %%esp\n"        \
                                              :                         \
                                              : "m" ((stk)->ss_sp))

struct {
    int32_t eax;
    int32_t ebx;
    int32_t ecx;
    int32_t edx;
} __cpuid;

static volatile long cpuidinit;

int
getcontext(ucontext_t *uc)
{
    uint8_t *fctx = uc->fctx;
    
    if (!cpuidinit) {
        cpuid(CPUIDINFO, &__cpuid);
        cpuidinit = 1;
    }
    if (__cpuid.eax & CPUIDFXSR) {
        __asm__ __volatile__ ("fxsave (%0)\n" : : "r" (fctx));
    } else {
        __asm__ __volatile__ ("fnsave (%0)\n" : : "r" (fctx));
    }
    sigprocmask(SIG_BLOCK, NULL, &uc->sigmask);
    uc->gregs = &uc->genregs;
    uc->fpstate = fctx;
    uc->uc_link = NULL;
    uc->__func = NULL;
    uc->__argc = 0;
    m_getcontext(&uc->uc_mcontext);

    return 0;
}

void
makecontext(ucontext_t *uc, void (*func)(), int argc, ..)
{
    va_list   va;
    long      ofs = argc * sizeof(long);
    long     *sp = (long *)((int8_t *)uc->uc_stack.ss_sp
                            + uc->uc_stack.ss_size);
    void    (*func)(int argc, ...);

    sp -= argc;
    uc->__argc = argc;
    va_start(va, argc);
    while (argc--) {
        *sp++ = va_arg(va, long);
    }
    uc->uc_stack.ss_sp = sp;
    uc->__func = func;

    return;
}

int
setcontext(const ucontext_t *uc)
{
    uint8_t *fctx = uc->fctx;
    
    if (!cpuidinit) {
        cpuid(CPUIDINFO, &__cpuid);
        cpuidinit = 1;
    }
    if (__cpuid.eax & CPUIDFXSR) {
        __asm__ __volatile__ ("fxrstor (%0)\n" : : "r" (fctx));
    } else {
        __asm__ __volatile__ ("fnrstor (%0)\n" : : "r" (fctx));
    }
    sigprocmask(SIG_SETMASK, &uc->sigmask, NULL);
    if (uc->__func) {
        m_setstack(&uc->stack);
        uc->__func();
        if (uc->uc_link) {
            m_setcontext(uc->uc_link);
        } else {

            exit(0);
        }
    } else {
        m_setcontext(uc);
    }
}

#endif

