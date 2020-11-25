#ifndef __MJOLNIR_RC_H__
#define __MJOLNIR_RC_H__

#include <stdio.h>
#include <mjolnir/cw.h>

struct rcprog {
    FILE               *fp;
    long                start;
};

void                    rcinitop(void);
void                    rcaddop(const char *name, long id);
long                    rcloadfile(const char *name, long base, long *sizeret);

#endif /* __MJOLNIR_RC_H__ */

