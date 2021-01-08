#ifndef MT_PRIOLK_H
#define MT_PRIOLK_H

/* velaho priority locks */

#include <stdint.h>
#include <mach/param.h>
#if defined(PRIOLKUSEMMAP)
#define MTPRIOLKALLOCFAILED         MAP_FAILED
#define MTPRIOLKALLOC(sz)           mapanon(0, sz)
#include <vnd/unix.h>
#else
#define MTPRIOLKALLOCFAILED NULL
#define MTPRIOLKALLOC(sz)           malloc(sz)
#endif

/*
 * REFERENCE: http://locklessinc.com/articles/priority_locks/
 */

struct mtpriolkdata {
    unsigned long                   val;
    unsigned long                   orig;
    volatile struct mtpriolkdata   *next;
};

struct mtpriolk {
    volatile struct mtpriolkdata   *owner;
    volatile unsigned long          waitbits;
};

void                                mtinitpriolk(struct priolkdata *data,
                                                 unsigned long val);
void                                mtfinpriolk(void);
void                                priolkget(struct priolk *priolk);
void                                priolkrel(struct priolk *priolk);

#endif /* MT_PRIOLK_H */

