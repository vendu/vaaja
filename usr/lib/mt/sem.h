#ifndef __MT_SEM_H__
#define __MT_SEM_H__

/* velho semaphores */

#include <limits.h>
#include <mt/mtx.h>

#define MTSEM_MAXVAL ULONG_MAX
typedef struct sem {
#if (MTFMTX)
    mtfmtx      lk;
#endif
    unsigned long val;
} mtsem;

#endif /* __MT_SEM_H__ */

