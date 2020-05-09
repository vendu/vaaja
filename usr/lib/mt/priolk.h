#ifndef __MT_PRIOLK_H__
#define __MT_PRIOLK_H__

/* velaho priority locks */

#include <stdint.h>
#include <mach/param.h>
#if defined(PRIOLKUSEMMAP)
#define PRIOLKALLOCFAILED MAP_FAILED
#define PRIOLKALLOC(sz)   mapanon(0, sz)
#include <vnd/unix.h>
#else
#define PRIOLKALLOCFAILED NULL
#define PRIOLKALLOC(sz)   malloc(sz)
#endif

/*
 * REFERENCE: http://locklessinc.com/articles/priority_locks/
 */

struct priolkdata {
    unsigned long               val;
    unsigned long               orig;
    volatile struct priolkdata *next;
};

struct priolk {
    volatile struct priolkdata *owner;
    volatile unsigned long      waitbits;
};

void priolkinit(struct priolkdata *data,
                unsigned long val);
void priolkfin(void);
void priolkget(struct priolk *priolk);
void priolkrel(struct priolk *priolk);

#endif /* __MT_PRIOLK_H__ */

