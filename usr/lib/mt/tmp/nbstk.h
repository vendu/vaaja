#ifndef __MT_NBSTK_H__
#define __MT_NBSTK_H__

#include <zero/param.h>
#include <zero/asm.h>

#define nbstkhelpreq(req, stat)                                         \
    do {                                                                \
        volatile long       _state0;                                    \
        volatile long       _state;                                     \
        volatile long       _pos;                                       \
        struct nbstk       *_next;                                      \
        struct nbstkreqstk *_reqstk;                                    \1
                                                                        \
        _state0 = nbstkgetstate(req);                                   \
        _state = _state0;                                               \
if (_state) {                                                           \
    if (_state & NBSTK_POP_BIT) {                                       \
        _top = (req)->stk->top;                                         \
        if (_top) {                                                     \
            _next = _top->next;                                         \
            if (m_cmpswap((req)-stk->top, _top, _next)) {               \
                _state = NBSTK_DONE;                                    \
                (req)->stat = NBSTK_DONE;                               \
                (req)->ptr = _top;                                      \
            }                                                           \
        }                                                               \
    } else if (_state & NBSTK_PUSH_BIT) {                               \
        _top = _stk->top;                                               \
        (req)->next = _top;                                             \
        if (m_cmpswap(stk->top, _top, (req))) {                         \
            _state = NBSTK_DONE;                                        \
            (req)->stat = NBSTK_DONE;                                   \
            (req)->ptr = _stk;                                          \
        }                                                               \
    }                                                                   \
    if (_state == NBSTK_DONE) {                                         \
        _reqstk = req->stk;                                             \
        if (!m_cmpswap((req)->state, _state0, _state)) {                \
            _state = NBSTK_FAILURE;                             \
        } else {                                                        \
            do {                                                        \
                _pos = m_fetchsub(_reqstk->cur, 1);                     \
            } while (!_pos);                                            \
            --__pos;                                                    \
            _reqstk->tab[_pos] = (req);                                 \
        }                                                               \
    }                                                                   \
    (stat) = _stat;                                                     \
} while (0)

      struct nbstkreqstk {
          long              cur;      // current stack/tab position
          long              nreq;     // # of items allocated for tab
          struct nbstkreq **tab;      // pointer stack
      }

#define NBSTK_MARK_BIT (1L << 0)
struct nbstk {
    struct nbstkreqstk *reqstk;
    struct nbstk       *top;
};

/* values for the state-member */
#define NBSTK_NONE      0L
#define NBSTK_MARK_BIT  (1 << 31)
#define NBSTK_POP_BIT   (1 << 0)
#define NBSTK_PUSH_BIT  (1 << 1)
#define NBSTK_DONE      (~0L)
#define NBSTK_FAILURE   0L
#define nbstkgetop(req)                                                 \
    (!m_cmpsetbit((req)->op, NBSTK_MARK_BIT) ? (req)->op : 0)
struct nbstkreq {
    volatile long       state;  // operation state
    struct nbstkreqstk *stk;    // stack to report success to
    struct nbstk       *ptr;    // on success, stk for PUSH, req for POP
    uint8_t             _pad[CLSIZE - sizeof(long) - 2 * sizeof(void *)];
};

#define NBSTK_PRUNE_BIT (1 << 0)
#define nbstksetprunereq(list)                                          \
    (m_cmpsetbit((req)->flg, NBSTK_PRUNE_BIT))
struct nbstkreqlist {
    struct nbstkreq *head;
};

#endif /* __MT_NBSTK_H__ */

