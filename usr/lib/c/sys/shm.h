#ifndef __SYS_SHM_H__
#define __SYS_SHM_H__

#include <features.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <mach/param.h>

/* permissions for shmget() */
#define SHM_R      0x00000100
#define SHM_W      0x00000080
/* flags for shmat() */
#define SHM_RDONLY 0x00001000
#define SHM_RND    0x00002000
#define SHM_REMAP  0x00004000
/* commands for shmctl() */
#define SHM_LOCK   11
#define SHM_UNLOCK 12
#define SHM_STAT   13
#define SHM_INFO   14
/* shm_mode upper byte flags */
#define SHM_DEST      0x00000200
#define SHM_LOCKED    0x00000400
#define SHM_HUGETLB   0x00000800
#define SHM_NORESERVE 0x00001000

#define SHMLBA     PAGESIZE

typedef unsigned long shmatt_t;

//typedef long key_t;

struct shmid_ds {
    struct ipc_perm shm_perm;	// permission structure
    size_t          shm_segsz;	// segment size in bytes
    time_t          shm_atime;	// time of last shmat()
    unsigned long   res1;
    time_t          shm_dtime;	// time of last shmdt()
    unsigned long   res2;
    time_t          shm_ctime;	// time of last shmctl() change
    unsigned long   res3;
    pid_t           shm_cpid;	// creator process ID
    pid_t           shm_lpid;	// last shmop process ID
    shmatt_t        shm_nattch;	// # of current attaches
    unsigned long   res4[2];
};

struct shminfo {
    unsigned long shmmax;
    unsigned long shmmin;
    unsigned long shmmni;
    unsigned long shmseg;
    unsigned long shmall;
    unsigned long res[4];
};

struct shm_info {
    int           used_ids;
    unsigned long shm_tot;
    unsigned long shm_rss;
    unsigned long shm_swp;
    unsigned long swap_attempts;
    unsigned long swap_successes;
};

#if !defined(__KERNEL__)

extern int shmctl(int shm, int cmd, struct shmid_ds *buf);
extern int shmget(key_t key, size_t size, int shmflg);
extern void *shmat(int shm, const void *adr, int shmflg);
extern int shmdt(const void *adr);

#endif /* !defined(__KERNEL__) */

#endif /* __SYS_SHM_H__ */

