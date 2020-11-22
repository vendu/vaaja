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
long                    rcxlatef(FILE *fp,
                                 long pid,
                                 long base,
                                 long *baseret,
                                 long *sizeret);
void                    rcdisasm(struct cwinstr *op,
                                 FILE *fp);

#endif /* __MJOLNIR_RC_H__ */

