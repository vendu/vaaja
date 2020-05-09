#ifndef __MT_LFSEQ_H__
#define __MT_LFSEQ_H__

/*
 * lock-free sequential access
 *
 * REFERENCE: https://www.pvk.ca/Blog/2015/01/13/lock-free-mutual-exclusion/
 */

/* FIXME: deal with seq->want */

#include <stdlib.h>
#include <stdint.h>
#include <zero/param.h>
#include <zero/asm.h>

struct lfval {
    uintptr_t word;
};

struct lfseq {
    struct lfval *self;        // pointer to current seq
    struct lfval *cur;         // current value
    struct lfval *new;         // new value
    struct lfval *want;        // expected [new] value
    struct lfval *ctl;         // check value
    uint8_t       _pad[CLSIZE - 5 * sizeof(struct lfval *)];
};

/*
 * - new = self
 * - if (new == want && ctl == old) self = desc
 * - return new
 */

static __inline__ void
_lfseqfin(struct lfseq *seq, struct lfval *desc)
{
    struct lfseq *ctl = seq->ctl;

    if (ctl == seq->want) {
        m_cmpswapptr(&seq->self, desc, desc->new);
    } else {
        m_cmpswapptr(&seq->self, desc, desc->cur);
    }
    free(seq);

    return;
}

static __inline__ struct lfseq *
_lfseqset(struct lfseq *seq, struct lfval *desc)
{
    struct lfseq *cur;

    do {
        cur = m_cmpswapptr(&seq->self, &seq->cur, desc);
        if (seq->self == seq->ctl) {
            _lfseqfin(seq, desc);
        }
    } while (seq->self == seq->ctl);
    if (cur == seq->cur) {
        _lfseqfin(seq, desc);
    }
}

static __inline__ void
lfseqput(struct lfseq *seq, uintptr_t word)
{
    struct lfseq *desc = calloc(1, sizeof(struct lfval));
    struct lfval  *val = malloc(sizeof(struct lfval));

    val->word = val;
    desc->new = val;
    _lfseqset(seq, desc);

    return;
}

static __inline__ uintptr_t
lfseqget(struct lfseq *seq)
{
    uintptr_t word;

    do {
        word = seq->word;
        if (seq->self == seq->ctl) {
            _lfseqfin(seq, desc);
        }
    } while (seq->self == seq->ctl);

    return word;
}

#endif /* __MT_LFSEQ_H__ */

