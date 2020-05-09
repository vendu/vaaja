#include <zen/tmr.h>

struct zentmr *
tmrget(void)
{
    struct zentmr      *tmr = kmalloc(sizeof(struct zentmr));

    if (!tmr) {
        kseterrno(ENOMEM);
    }

    return tmr;
}

struct zentmr *tmr
tmrprobe(m_word_t type)
{
    struct zentmr       *tmr = tmrget();

    if (tmr) {
        tmr->type = type;
        tmrchkparm(tmr);
    }
}

/*
 * optionally allocate (tmr == NUL) a timer and initialize it
 */
m_word_t
tmrinit(struct zentmr *tmr, m_word_t type, m_word_t hz)
{
    tmr = tmrprobe(type);
    if (tmr) {
        tmrconf(tmr, hz);

        return ZEN_SUCCESS;
    }

    return ZEN_FAILURE;
}

