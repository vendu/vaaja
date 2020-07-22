#ifndef __SYS_ZEN_SYS_H__
#define __SYS_ZEN_SYS_H__

#include <sys/zen/sched/task.h>

struct zensys {
    struct zencpu       cputab[ZEN_MAX_CPUS];
    struct zenproc     *proctab[ZEN_MAX_TASKS];
    struct zentask     *systasktab[ZEN_SYS_THREADS];
    struct zentask     *tasktab[ZEN_MAX_TASKS];
    m_page_t            pagedir[MACH_PAGE_DIR_ITEMS];
    m_uword_t           ndesc;
    m_word_t           *desctab;
    m_uword_t           nnodehash;
    struct zenvfsnode  *nodehash;
    m_uword_t           nmembuf;
    struct zenmembuf   *membuftab;
};

#endif /* __SYS_ZEN_SYS_H__ */

