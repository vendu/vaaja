#include <sys/zen/conf.h>
#include <sys/zen/sys.h>
#include <sys/zen/sched/task.h>

struct zenproc  k_proctab[ZEN_MAX_TASKS];
//struct zentask  k_tasktab[ZEN_MAX_TASKS];
struct zentask  k_systasktab[ZEN_SYS_THREADS];
struct zensys   k_zensys;

struct zentask *
k_getcurtask(void)
{
#if defined(ZEN_SIMULATION)
    pthread_t                   id = pthread_self();
    struct zentask             *task = &k_tasktab[id];
#else
    struct zentask             *task = &k_tasktab[ZEN_KERNEL_THREAD];
#endif

    return task;
}

C_NORETURN C_INLINE void
k_jmptask(struct m_thr *thr)
{
    ;
}

