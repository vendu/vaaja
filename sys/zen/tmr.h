#ifndef SYS_ZEN_TMR
#define SYS_ZEN_TMR

#include <sys/zen/conf.h>
#include <time.h>
#include <sys/time.h>
#include <mach/types.h>

#if defined(ZEN_TMR_HZ)
#define ZEN_SCHED_HZ            ZEN_TMR_HZ
#endif
#define kgethz()                ZEN_TMR_HZ

#define ZEN_INTR_TIMER          0
#define ZEN_HIRES_TIMER         1
#define ZEN_RTC_TIMER           2
#define ZEN_HIRES_TIMER_DRIFT   1   // FIXME

#define ZEN_HIRES_TIMER_FREQ    ZEN_HIRES_TIMER_MIN_HZ
#define ZEN_HIRES_TIMER_MIN_HZ  1000000000

struct zentmr {
    m_word_t                    type;
    m_word_t                    freq;
    m_word_t                    drift;
    m_word_t                    hz;
    union {
        struct timespec         ts;
        struct timeval          tv;
    } val;
};

#endif /* SYS_ZEN_TMR */

