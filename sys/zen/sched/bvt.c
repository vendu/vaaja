#if defined (__v0__)
#include <zen/bsp/v0.h>
#include <v0/types.h>
#endif
#include <zen/sched/bvt.h>

m_thr_t                 g_taothrtab[MACH_MAX_THREADS]
static m_uword_t        g_taovtime;

