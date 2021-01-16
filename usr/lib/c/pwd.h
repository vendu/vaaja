#ifndef __PWD_H__
#define __PWD_H__

#include <features.h>
#if (USEXVID) || (_GNU_SOURCE)
#include <stdio.h>
#endif
#include <sys/types.h>

struct pwd {
	char  *pw_name;     // user name
	char  *pw_passwd;   // password
	uid_t  pw_uid;      // user ID
	gid_t  pw_gid;      // group ID
	char  *pw_gecos;    // real name
	char  *pw_dir;      // home directory
	char  *pw_shell;    // shell program
};

#if !defined(__KERNEL__)

#if (USESVID) || (USEXOPENEXT)
extern void           setpwent(void);
extern void           endpwent(void);
extern struct passwd *getpwent(void);
#endif
#if (USESVID)
extern struct passwd *fgetpwent(FILE *stream);
extern int             putpwent(const struct passwd *__restrict passwd,
								FILE *__restrict fp);
#endif
extern struct passwd *getpwuid(uid_t uid);
extern struct passwd *getpwnam(const char *name);

#define NSS_BUFLEN_PASSWD 1024
#if (_POSIX_SOURCE)
#if (USESVID)
extern int getpwent_r(struct passwd *__restrict resbuf,
                      char *__restrict buf, size_t buflen,
                      struct passwd **res);
#endif
extern int getpwuid_r(uid_t uid,
                      struct passwd *__restrict resbuf,
                      char *__restrict buf, size_t buflen,
                      struct passwd **__restrict res);
extern int getpwnam_r(const char *__restrict name,
                      struct passwd *__restrict resbuf,
                      char *__restrict buf, size_t buflen,
                      struct passwd **__restrict res);
#if (USESVID)
extern int fgetpwent_r(FILE *__restrict stream,
                       struct passwd *__restrict resbuf,
                       char *__restrict buf, size_t buflen,
                       struct passwd *__restrict res);
#endif

#if (_GNU_SOURCE)
extern int getpw(uid_t uid, char *buf);
#endif

#endif /* !__KERNEL__ */

#endif /* __PWD_H__ */

