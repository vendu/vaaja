#include <sys/zen/conf.h>
#include <sys/zen/zen.h>
#include <sys/zen/sys.h>
#include <sys/zen/sched/task.h>

struct zenproc  k_proctab[ZEN_MAX_TASKS];
struct zentask  k_tasktab[ZEN_MAX_TASKS];
struct zensys   k_zensys;

struct zentask *
k_getcurtask(void)
{
    struct zentask *task = &k_tasktab[ZEN_KERNEL_THREAD];

    return task;
}

C_NORETURN C_INLINE void
k_jmptask(struct m_thr *thr)
{
    ;
}

