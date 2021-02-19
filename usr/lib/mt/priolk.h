#ifndef MT_PRIOLK_H
#define MT_PRIOLK_H

#include <mt/conf.h>

#if defined(MT_PRIORITY_LOCK)

/* priority locks */

#include <stdint.h>
#include <mach/param.h>

#define ZEN_PRIOLK_ALLOCFAILED      NULL
#define ZEN_PRIOLKALLOC(sz)         malloc(sz)

/*
 * REFERENCE: http://locklessinc.com/articles/priority_locks/
 */

struct zenpriolkdata {
    unsigned long                   val;
    unsigned long                   orig;
    volatile struct zenpriolkdata  *next;
};

struct zenpriolk {
    volatile struct zenpriolkdata  *owner;
    volatile unsigned long          waitbits;
};

void                                zeninitpriolk(struct zenpriolkdata *data,
                                                  unsigned long val);
void                                zenfinpriolk(void);
void                                zengetpriolk(struct zenpriolk *priolk);
void                                zenrelpriolk(struct zenpriolk *priolk);

#endif /* defined(MT_PRIORITY_LOCK) */

#endif /* MT_PRIOLK_H */

