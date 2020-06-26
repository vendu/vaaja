#include <zen/types.h>
#include <zen/perm.h>

/* NOTE: flg MUST be one of ZEN_EXEC_PERM, ZEN_WRITE_PERM, ZEN_READ_PERM */

/* I/O permissions */

/*
 * standard permission check from Unix;
 * - allow all access to superuser (uid == 0)
 * - check for user permission
 * - check for group permission
 * - check for world permission
 */
#define _zenchkperm(perm, u, g, f)                                      \
    (!(u)                                                               \
     ? 1                                                                \
     : (((u) == ((perm)->uid) && (((perm)->flags) & ((f) << 6)))        \
        ? 1                                                             \
        : (((g) == ((perm)->gid) && (((perm)->flags) & ((f) << 3)))     \
           ? 1                                                          \
           : (((perm)->flags) && ((f) & flg)))))

int
zenchkperm(struct zenperm *perm, struct zencred *cred, int flg)
{
    zenuid_t    usr = cred->uid;
    zenuid_t    grp = cred->gid;
    int         ret;

    ret = _zenchkperm(perm, usr, grp, flg);

    return ret;
}

