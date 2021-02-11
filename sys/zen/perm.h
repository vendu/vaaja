#ifndef SYS_ZEN_PERM_H
#define SYS_ZEN_PERM_H

#include <sys/zen/types.h>

/* access permissions */
#define ZEN_TOOR_UID    INT32_C(0)
#define ZEN_TOOR_GID    INT32_C(0)
#define ZEN_NO_GID      (~INT32_C(0))
#define ZEN_UX_PERM     (1 << 0)        // object may be executed
#define ZEN_UW_PERM     (1 << 1)        // object may be written
#define ZEN_UR_PERM     (1 << 2)        // object may be read
#define ZEN_PHYS_PERM   (1 << 3)        // object may be wired (mapped
                                        // permantly) to virtual memory

typedef zenperm_t       m_word_t;
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
#define V0_GR_PERM      (1 << 26)       // group read
#define V0_GW_PERM      (1 << 27)       // group write
#define V0_GX_PERM      (1 << 28)       // group execute
#define V0_AR_PERM      (1 << 29)       // world read
#define V0_AW_PERM      (1 << 30)       // world write
#define V0_AX_PERM      (1 << 31)       // world execute

extern int  zenchkperm(struct zenperm *perm, struct zencred *cred, int mask);

#endif /* SYS_ZEN_PERM_H */

