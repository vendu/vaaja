#ifndef MT_SEM_H
#define MT_SEM_H

/* zen semaphores */

#define MTSEM_INITVAL           ULONG_MAX
typedef struct sem {
#if defined(MTFMTX)
    mtfmtx                      lk;
#endif
    unsigned long               val;
} mtsem;

#endif /* MT_SEM_H */

