#include <stdint.h>
#include <stdlib.h>
#include <zero/lfdeq.h>
#include <mach/asm.h>

static int16_t           lfdeqtab[LFDEQ_MAX_ITEMS];
static long              lfdeqndx = 1;
static struct lfdeqitem *lfdeqlist;
struct lfdeqitem        *lfdeqbase;
struct lfdeq             lfdeq; // initially { NULL, NULL, LFDEQ_STABLE }

static void
lfdeqinit(struct lfdeqitem *head, size_t n)
{
    long ndx;

    for (ndx = 0 ; ndx < n ; ndx++) {
        lfdeqtab[ndx] = ndx;
    }

    return;
}

int16_t
lfdeqallocitem(void)
{
    long              ndx;
    int16_t           ret;
    struct lfdeqitem *head;

    while (!lfdeqlist) {
        if (m_cmpsetbit((m_atomic_t *)&lfdeqlist, LFDEQ_ADR_LK_BIT_POS)) {
            head = malloc(LFDEQ_MAX_ITEMS * sizeof(struct lfdeqitem));
            lfdeqinit(head, LFDEQ_MAX_ITEMS);
            if (!m_cmpswapptr((m_atomic_t **)&lfdeqlist, NULL, head)) {
                free(head);

                break;
            }
            lfdeqbase = head;
        }
    }
    ndx = m_fetchadd((m_atomic_t *)lfdeqndx, 1);
    head = &lfdeqbase[ndx];
    if (head) {
        *((uint32_t *)&head->info) = UINT32_C(0);
    }

    return ret;
}

