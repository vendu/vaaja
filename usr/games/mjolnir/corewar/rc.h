#ifndef __COREWAR_RC_H__
#define __COREWAR_RC_H__

#include <stdio.h>

struct rcprog {
    FILE               *fp;
    long                start;
};

void                    rcinitop(void);
void                    rcaddop(const char *name, long id);
long                    rcloadfile(const char *name, long base, long *sizeret);

#endif /* __COREWAR_RC_H__ */

