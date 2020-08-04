#ifndef __SYS_ZEN_TMR_H__
#define __SYS_ZEN_TMR_H__

#include <sys/zen/conf.h>
#include <time.h>
#include <sys/time.h>
#include <mach/types.h>

#define TAO_SCHED_POLICY        TAO_LOW_LATENCY_SCHED
#define TAO_DESKTOP_SCHED       0
#define TAO_LOW_LATENCY_SCHED   1
#define TAO_SERVER_SCHED        2

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
#if (TAO_SCHED_POLICY == TAO_LOW_LATENCY_SCHED)
#define ZEN_TMR_HZ              125
#define ZEN_TMR_SLICE_MS        16
#elif (TAO_SCHED_POLICY == TAO_DESKTOP_SCHED)
#define ZEN_TMR_HZ              (125 / 2)
#define ZEN_TMR_SLICE_MS        32
#elif (TAO_SCHED_POLICY == TAO_SERVER_SCHED)
#define ZEN_TMR_HZ              (125 / 8)
#define ZEN_TMR_SLICE           64
#else
#errof TAO_SCHED_POLICY not specified in <sys/zen/sched/tao.h>
#endif
#endif /* (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED) */

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

#endif /* __SYS_ZEN_TMR_H__ */

