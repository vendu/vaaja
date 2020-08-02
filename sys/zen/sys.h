#ifndef __SYS_ZEN_SYS_H__
#define __SYS_ZEN_SYS_H__

#include <sys/zen/zen.h>
#include <sys/zen/sched/task.h>

#define ZEN_PANIC_REBOOT    (1 << 0)

struct zensys {
    struct zencpu       cputab[ZEN_MAX_CPUS];
    struct zenproc     *proctab[ZEN_MAX_TASKS];
    struct zentask     *systasktab[ZEN_SYS_THREADS];
    struct zentask     *tasktab[ZEN_MAX_TASKS];
    m_page_t            pagedir[MACH_PAGE_DIR_ITEMS];
    m_uword_t           ndesc;
    m_word_t           *desctab;
#if defined(__v0__)
    m_word_t            pad1;
#endif
    m_uword_t           nnodehash;
    struct zenvfsnode  *nodehash;
    m_uword_t           nmembuf;
#if defined(__v0__)
    m_word_t            pad2;
#endif
    struct zenmembuf   *membuftab;
};

static C_INLINE C_NORETURN void
kreboot(int flg)
{
    if (!(flg & ZEN_PANIC_REBOOT)) {
        /*
         * - unmount filesystems
         * - send processes SIGTERM
         * - wait for, say, one minute
         * - send still-existing processes SIGKILL
         * - close user sessions
         */
    }
    kpanic("kreboot() not implemented yet", SIGSYS);
}

#endif /* __SYS_ZEN_SYS_H__ */

