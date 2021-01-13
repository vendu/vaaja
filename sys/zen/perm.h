#ifndef SYS_ZEN_PERM_H
#define SYS_ZEN_PERM_H

#include <sys/zen/types.h>

/* access permissions */
#define ZEN_TOOR_UID    INT32_C(0)
#define ZEN_TOOR_GID    INT32_C(0)
#define ZEN_NO_GID      (~INT32_C(0))
#define ZEN_EXEC_PERM   1
#define ZEN_WRITE_PERM  2
#define ZEN_READ_PERM   4
struct zenperm {
    zenuid_t            uid;
    zenuid_t            gid;
    long                flags;
};

struct zencred {
    zenuid_t            uid;
    zenuid_t            gid;
    long                flags;
};

/* I/O permission-bits */
#define V0_UR_PERM      (1L << 8)       // user read permission bit
#define V0_UW_PERM      (1L << 7)       // user write
#define V0_UX_PERM      (1L << 6)       // user execute
#define V0_GR_PERM      (1L << 5)       // group read
#define V0_GW_PERM      (1L << 4)       // group write
#define V0_GX_PERM      (1L << 3)       // group execute
#define V0_AR_PERM      (1L << 2)       // world read
#define V0_AW_PERM      (1L << 1)       // world write
#define V0_AX_PERM      (1L << 0)       // world execute

extern int  zenchkperm(struct zenperm *perm, struct zencred *cred, int mask);

#endif /* SYS_ZEN_PERM_H */

