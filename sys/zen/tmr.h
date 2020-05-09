#ifndef __ZEN_TIMER_H__
#define __ZEN_TIMER_H__

#define ZEN_INTR_TIMER   0
#define ZEN_HIRES_TIMER  1

#define ZEN_HIRES_MIN_HZ 1000000000

struct zentmr {
    m_word_t            type;
    m_word_t            freq;
    m_word_t            drift;
    m_word_t            hz;
    union {
        struct timespec ts;
        struct timeval  tv;
    } val;
};

#endif /* __ZEN_TIMER_H__ */

