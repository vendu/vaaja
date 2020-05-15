/* NOTE: flg MUST be one of ZEN_EXEC_PERM, ZEN_WRITE_PERM, ZEN_READ_PERM */

/* I/O permissions */

/*
 * standard permission check from Unix;
 * - allow all access to superuser (uid == 0)
 * - check for user permission
 * - check for group permission
 * - check for world permission
 */
#define _zenchkperm(perm, uid, grp, flg)                                \
    (!(uid)                                                             \
     ? 1                                                                \
     : (((uid) == ((perm)->uid) && (((perm)->flags) & ((flg) << 6)))    \
        ? 1                                                             \
        : (((gid) == ((perm)->gid) && (((perm)->flags) & ((flg) << 3))) \
           ? 1                                                          \
           : (((perm)->flags) && ((flags) & flg)))))

int
zenchkperm(struct zenperm *perm, struct zencred *cred, int flg)
{
    zenuid_t    usr = cred->uid;
    zenuid_t    grp = cred->gid;
    zenuid_t    uid = perm->owner->uid;
    zenuid_t    gid = perm->owner->gid;
    zenperm_t   flags = perm->flags;
    int         ret;

    ret = _zenchkperm(uid, grp, flg, flg);

    return ret;
}

