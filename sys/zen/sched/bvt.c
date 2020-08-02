#include <sys/zen/conf.h>

/* borrowed virtual time scheduler stub */

#if (ZEN_TASK_SCHED == ZEN_BVT_TASK_SCHED)

#include <mach/types.h>
//#include <sys/zen/sched/tao.h>

struct m_thr            g_taothrtab[ZEN_MAX_TASKS];
static m_uword_t        g_taovtime;

#endif /* (ZEN_TASK_SCHED == ZEN_BVT_TASK_SCHED) */

