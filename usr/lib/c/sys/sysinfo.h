#ifndef __SYS_SYSINFO_H__
#define __SYS_SYSINFO_H__

#include <features.h>

#if !defined(__KERNEL__)

#if (_GNU_SOURCE)
extern int  get_nprocs_conf(void);
extern int  get_nprocs(void);
extern long get_phys_pages(void);
extern long get_avphys_pages(void);
#endif

#endif /* !defined(__KERNEL__) */

/* structure from linux */
struct sysinfo {
    long          uptime;    /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* swap space still available */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned long mem_unit;  /* Memory unit size in bytes */
    unsigned long procs;     /* Number of current processes */
    unsigned long _pad[2];   /* pad to 16 * sizeof(long) */
};

#endif /* __SYS_SYSINFO_H__ */

