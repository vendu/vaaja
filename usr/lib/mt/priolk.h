#ifndef MT_PRIOLK_H
#define MT_PRIOLK_H

#include <mt/conf.h>

#if defined(ZEN_PRIORITY_LOCK)

/* priority locks */

#include <stdint.h>
#include <mach/param.h>

#define MT_PRIOLK_ALLOCFAILED       NULL
#define MT_PRIOLKALLOC(sz)          malloc(sz)

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

#endif /* defined(ZEN_PRIORITY_LOCK) */

#endif /* MT_PRIOLK_H */

