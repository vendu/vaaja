#ifndef __SYS_ZEN_DEV_CHR_CON_H__
#define __SYS_ZEN_DEV_CHR_CON_H__

#include <sys/zen/util.h>

static C_INLINE void
conputs(const char *str)
{
    kpanic("conputs() not implemented yet", SIGSYS);
}

static C_INLINE void
conputchar(int ch)
{
    kpanic("conputs() not implemented yet", SIGSYS);
}

#endif /* __SYS_ZEN_DEV_CHR_CON_H__ */

