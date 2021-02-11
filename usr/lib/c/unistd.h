#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <features.h>

/* standard descriptor names */
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#include <features.h>
#if !defined(__size_t_defined)
#include <share/size.h>
#endif
#if !defined(__off_t_defined)
#include <share/off_t.h>
#endif
//#include <stddef.h>
#include <stdio.h>
//#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <mach/param.h>
#if defined(_USE_POSIX2) && 0 /* TODO: getopt.h */
#include <getopt.h>
#endif

#include <bits/unistd.h>

extern char *optarg;
extern int   optind;
extern int   opterr;
extern int   optopt;

#if 0
#define _XOPEN_UNIX     1
#define _XOPEN_CRYPT    1
#define _XOPEN_ENH_I18N 1
#define _XOPEN_LEGACY   1
#endif

#if !defined(__kernel__)

extern char **__environ;
#if defined(_GNU_SOURCE)
extern char **environ;
#endif

/* check given access permissions */
int            access(const char *path, int type);
#if defined(_GNU_SOURCE)
/* check given access permissions using effective UID and GID */
int            euidaccess(const char *path, int type);
#define        eaccess       euidaccess
#endif
#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
/* check access at path relative to fd */
int            faccessat(int fd, const char *path, int type, int flg);
#endif
/* file operations */
/* lseek(), llseek() */
off_t          lseek(int fd, off_t ofs, int whence);
/* read(), write(), close() */
ssize_t        read(int fd, void *buf, size_t count);
ssize_t        write(int fd, void *buf, size_t count);
int            close(int fd);
#if defined(_USE_UNIX98)
/* like read() and write(), but with offset arguments */
ssize_t        pread(int fd, void *buf, size_t len, off_t ofs);
ssize_t        pwrite(int fd, const void *buf, size_t len, off_t ofs);
#endif
/* pipe/LIFO I/O channel */
int            pipe(int fd[2]);
/* trigger SIGALRM in [at least] nsec seconds */
unsigned int   alarm(unsigned int nsec);
/* sleep for [at least] nsec seconds */
unsigned int   sleep(unsigned int nsec);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/* trigger SIGALRM in [at least] value microseconds, then every interval */
useconds_t     ualarm(useconds_t value, useconds_t interval);
/* sleep for [at least] nusec microseconds */
int            usleep(useconds_t nusec);
#endif
/* sleep until signal-terminated or signal handler invoked */
int            pause(void);
/* change owner and group of file, possibly via a symlink */
int            chown(const char *path, uid_t uid, gid_t gid);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/* change owner and group of file refered to by fd */
int            fchown(int fd, uid_t uid, gid_t gid);
/* change onwer credentials of a symlink */
int            lchown(const char *path, uid_t uid, gid_t gid);
#endif
#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
/* change owner credentials of file at path relative to fd */
int            fchownat(int fd, const char *path, uid_t uid, gid_t gid);
#endif
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/* change current working directory that refered to by fd */
int            fchdir(int fd);
#endif
/* query current working directory */
char         * getcwd(char *buf, size_t len);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/* DEPRECATED; buf should be at least PATH_MAX bytes; allocate if NULL */
char         * getwd(char *buf);
#endif
#if defined(_GNU_SOURCE)
/*
 * get current working directory; malloc() buffer of proper length;
 * return PWD
*/
char         * get_current_dir_name(void);
#endif
int            dup(int fd);
int            dup2(int fd, int newfd);
int            execve(const char *path, char *const argv[],
                             char *const *envp);
#if defined(_GNU_SOURCE)
int            fexecve(int fd, char *const argv, char *const envp[]);
#endif
int            execv(const char *path, char *const argv[]);
int            execle(const char *path, const char *arg, ...);
int            execvp(const char *file, char *const argv[]);
int            execlp(const char *file, const char *arg, ...);
#if defined(_XOPEN_SOURCE)
int            nice(int incr);
#endif
void           _exit(int status);
long           pathconf(const char *path, int name);
long           fpathconf(int fd, int name);
long           sysconf(int name);
#if defined(_USE_POSIX2)
size_t         confstr(int name, char *buf, size_t len);
#endif
pid_t          getpid(void);
pid_t          getppid(void);
#if !defined(FAVORBSD)
pid_t          getpgrp(void);
#else
#define        getpgrp getpgid
#endif
#if defined(_XOPEN_SOURCE_EXTENDED)
pid_t          getpgid(pid_t pid);
#endif
#if defined(_SVID_SOURCE) || defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
#if !defined(_FAVOR_BSD)
int            setpgrp(void);
#else
#define        setpgrp setpgid
#endif
#endif
pid_t          setsid(void);
#if defined(_XOPEN_SOURCE_EXTENDED)
pid_t          getsid(pid_t pid);
#endif
uid_t          getuid(void);
uid_t          geteuid(void);
gid_t          getgid(void);
gid_t          getegid(void);
int            getgroups(int len, gid_t list[]);
#if defined(_GNU_SOURCE)
int            group_member(gid_t gid);
#endif
int            setuid(uid_t uid);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
int            setreuid(uid_t ruid, uid_t euid);
#endif
#if defined(_BSD_SOURCE) || defined(_USE_XOPEN2K)
int            seteuid(uid_t uid);
#endif
int            setgid(gid_t gid);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
int            setregid(gid_t rgid, gid_t egid);
#endif
#if defined(_BSD_SOURCE) || defined(_USE_XOPEN2K)
int            setegid(gid_t gid);
#endif
#if defined(_GNU_SOURCE)
int            getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int            getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
int            setresuid(uid_t ruid, uid_t euid, uid_t suid);
int            setresgid(gid_t gid, gid_t egid, gid_t sgid);
#endif
//pid_t          fork(void);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
/*
 * - don't clone address space; execute in that of the parent
 * - suspend calling process until exit() or execve()
 */
pid_t          vfork(void);
#endif
char         * ttyname(int fd);
int            ttyname_r(int fd, char *buf, size_t len);
#if defined(_BSD_SOURCE) || (defined(_XOPEN_SOURCE_EXTENDED) && !defined(_USE_UNIX98))
int            ttyslot(void);
#endif
int            link(const char *from, const char *to);
#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
int            linkat(int fd, const char *from, int newfd,
                             const char *to, int flg);
#endif
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED) || defined(_USE_XOPEN2K)
int            symlink(const char *from, const char *to);
ssize_t        readlink(const char *__restrict path, char *__restrict buf,
                               size_t len);
#endif
#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
int            symlinkat(const char *from, int fd, const char *to);
ssize_t        readlinkat(int fd, const char *__restrict path,
                                 char *__restrict buf, size_t len);
#endif
int            unlink(const char *path);
#if defined(_GNU_SOURCE) || defined(_ATFILE_SOURCE)
int            unlinkat(int fd, const char *path, int flg);
#endif
int            rmdir(const char *path);
pid_t          tcgetpgrp(int fd);
int            tcsetpgrp(int fd, pid_t pgrp);
char         * getlogin(void);
#if defined(_REENTRANT) || (_POSIX_C_SOURCE >= 199506L)
int            getlogin_r(char *name, size_t len);
#endif
#if defined(_BSD_SOURCE)
int            setlogin(const char *name);
#endif
#if defined(_BSD_SOURCE) || defined(_USE_UNIX98)
int            gethostname(char *name, size_t len);
#endif
#if defined(_BSD_SOURCE) || (defined(_XOPEN_SOURCE) && !defined(_USE_UNIX98))
int            sethostname(const char *name, size_t len);
int            sethostid(long id);
int            getdomainname(const char *name, size_t len);
int            setdomainname(const char *name, size_t len);
int            vhangup(void);
int            revoke(const char *file);
int            profil(unsigned short *buf, size_t size, size_t ofs,
                             unsigned int scale);
int            acct(const char *name);
char         * getusershell(void);
void           endusershell(void);
void           setusershell(void);
int            daemon(int nochdir, int noclose);
#endif
#if defined(_BSD_SOURCE) || ((_XOPEN_SOURCE) && !defined(_USE_XOPEN2K))
int            chroot(const char *path);
char         * getpass(const char *prompt);
#endif
#if defined(_BSD_SOURCE) || (_XOPEN_SOURCE)
int            fsync(int fd);
#endif
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED)
long           gethostid(void);
void           sync(void);
#endif
#if (defined(_BSD_SOURCE)                                               \
     || !(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600))
#if defined(PAGESIZE) && (PAGESIZE)
#define        getpagesize() PAGESIZE
#elif defined(_SC_PAGESIZE)
#define        getpagesize() sysconf(_SC_PAGESIZE)
#elif defined(_SC_PAGE_SIZE)
#define        getpagesize() sysconf(_SC_PAGE_SIZE)
#else
int            getpagesize(void);
#endif /* PAGESIZE */
#endif /* _BSD_SOURCE... */
#if defined(_ZERO_SOURCE)
#if defined(MACH_CL_SIZE) && (MACH_CL_SIZE)
#define getclsize() MACH_CL_SIZE
#elif defined(_SC_CACHELINESIZE)
#define getclsize() sysconf(_SC_CACHELINESIZE)
#else
#warning getclsize() not defined in <unistd.h>
#endif
#endif
int            truncate(const char *file, off_t len);
#if defined(_BSD_SOURCE) || defined(_XOPEN_SOURCE_EXTENDED) || defined(_USE_XOPEN2K)
int            ftruncate(int fd, off_t len);
#endif
#if defined(_XOPEN_SOURCE_EXTENDED)
int            brk(void *adr);
void         * sbrk(intptr_t incr);
#endif
long           syscall(long num, ...);
int            lockf(int fd, int cmd, off_t len);
#if (_POSIX_C_SOURCE >= 199309L) || defined(_USE_UNIX98)
int            fdatasync(int fd);
#endif

#if defined(_XOPEN_SOURCE)
char         * crypt(const char *key, const char *salt);
void           encrypt(char *blk, int flg);
void           swab(const void *__restrict from, void *__restrict to,
                           ssize_t n);
#endif

#if defined(_XOPEN_SOURCE)
char         * ctermid(char *str);
#endif

#if defined(_XOPEN_SOURCE_EXTENDED)
int            getdtablesize(void);
#endif

#endif /* !defined(__kernel__) */

#endif /* __UNISTD_H__ */

