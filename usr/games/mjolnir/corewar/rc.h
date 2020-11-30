#ifndef __COREWAR_RC_H__
#define __COREWAR_RC_H__

#include <stdio.h>

struct rcprog {
    long  start;
    FILE *fp;
};

void rcinitop(void);
void rcaddop(const char *name, long id);
long rcxlate(FILE *fp, long pid, long base, long *baseret, long *sizeret);
void rcdisasm(struct cwinstr *op, FILE *fp);

#endif /* __COREWAR_RC_H__ */

