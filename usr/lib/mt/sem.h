#ifndef MT_SEM_H
#define MT_SEM_H

/* zen semaphores */

#define ZEN_SEMINITVAL          ULONG_MAX
typedef struct sem {
#if defined(ZENFMTX)
    zenfmtx                     lk;
#endif
    unsigned long               val;
} zensem;

#endif /* MT_SEM_H */

