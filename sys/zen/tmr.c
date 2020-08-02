#include <sys/zen/test.h>
#include <sys/zen/tmr.h>
#include <sys/zen/errno.h>

struct zentmr *
tmrget(void)
{
    struct zentmr      *tmr = kmalloc(sizeof(struct zentmr));

    if (!tmr) {
        kseterrno(ENOMEM);
    }

    return tmr;
}

void
tmrconf(struct zentmr *tmr, m_word_t type, m_word_t hz)
{
    tmr->type = type;
    switch (type) {
        case ZEN_RTC_TIMER:
            tmr->freq = 60;
            tmr->drift = 0;
            tmr->hz = 60;

            break;
        case ZEN_HIRES_TIMER:
            tmr->freq = ZEN_HIRES_TIMER_FREQ;
            tmr->drift = ZEN_HIRES_TIMER_DRIFT;
            tmr->hz = ZEN_HIRES_TIMER_MIN_HZ;

            break;
    }
    tmr->val.ts.tv_sec = 0;
    tmr->val.ts.tv_nsec = 0;

    return;
}

struct zentmr *
tmrprobe(m_word_t type, m_word_t hz)
{
    struct zentmr       *tmr = tmrget();

    if (tmr) {
        tmrconf(tmr, type, hz);
    }

    return tmr;
}

/*
 * optionally allocate (tmr == NUL) a timer and initialize it
 */
struct zentmr *
tmrinit(m_word_t type, m_word_t hz)
{
    struct zentmr  *tmr;
    
    tmr = tmrprobe(type, hz);

    return tmr;
}

