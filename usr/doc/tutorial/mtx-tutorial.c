#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <zero/prof.h>
#include <mach/param.h> /* for CLSIZE */
#include <mach/atomic.h>
#include <mach/asm.h>   /* for atomic machine operations */

#define M_FMTXINITVAL ((m_atomic_t)0)
#define M_FMTXLKVAL   ((m_atomic_t)1)

/* thread-safe/lockable data structure with a single uinptr_t data item */
struct mtxdata {
    volatile m_atomic_t lk;  /* actual lock variable */
    uintptr_t           data; /* data */
};

/* simple/naive version; returns 1 if lock succeeds, 0 if already locked */
m_atomic_t
m_trylkfmtx0(m_atomic_t *ptr)
{
    m_atomic_t res;     /* CAS-operation return value */
    res = m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL);
    return res;
}

m_atomic_t
m_trylkfmtx(volatile m_atomic_t *ptr)
{
    m_atomic_t res = 0;

    if (*ptr == M_FMTXINITVAL) {
        res = m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL);
    }

    return res;
}

/* simple/naive version */
void
m_lkfmtx0(m_atomic_t *ptr)
{
    do {
        ; /* busy-spin */
    } while (!m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL));

    return;
}

/* improved version #1 */
void
m_lkfmtx1(m_atomic_t *ptr)
{
    if (*ptr == M_FMTXINITVAL) {
        do {
            ; /* busy-spin */
        } while (!m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL));
    }

    return;
}
/* improved version #2 */
void
m_lkfmtx2(m_atomic_t *ptr)
{
    m_atomic_t res;

    do {
        res = 0;
        if (*ptr == M_FMTXINITVAL) {
            res = m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL);
        }
    } while (!res);

    return;
}

/* final version */
m_atomic_t
m_lkfmtx(volatile m_atomic_t *ptr)
{
    m_atomic_t res;

    do {
        if (*ptr == M_FMTXINITVAL) {
            res = m_cmpswap(ptr, M_FMTXINITVAL, M_FMTXLKVAL);
            if (res) {
                /* CAS succeeded */

                return res;
            }
        }
        m_waitspin(); /* "sleep" until an event occurs */
    } while (1);

    /* NOTRECHED */
    return 0;
}

void
m_unlkfmtx(volatile m_atomic_t *lp)
{
    m_membar(); /* full memory barrier */
    *lp = M_FMTXINITVAL; /* lazy-write */
    m_endspin(); /* signal wakeup-event */

    return;
}

#define MTX_NLOCK 131027
#define MTX_NTHR  16

int
main(int argc, char *argv[])
{
    struct mtxdata * volatile tab = calloc(MTX_NLOCK, sizeof(struct mtxdata));
    PROFDECLTICK(tck);

    profstarttick(tck);
    fprintf(stderr, "try: %lx\n", m_trylkfmtx(&tab[0].lk));
    m_lkfmtx(&tab[1].lk);
    fprintf(stderr, "done: %lx\n", tab[1].lk);
    m_unlkfmtx(&tab[1].lk);
    fprintf(stderr, "unlock: %lx\n", tab[1].lk);
    m_unlkfmtx(&tab[0].lk);
    fprintf(stderr, "release: %lx\n", tab[0].lk);
    profstoptick(tck);
    fprintf(stderr, "%llx microsecs\n", proftickdiff(tck));

    srand(0x666);
}

