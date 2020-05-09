#ifndef __ZEN_API_H__
#define __ZEN_API_H__

/* USER-MODE API */

/* task-interface; processes and threads */
int		taskexec(const char *pathname, char *const argv[],
			 char *const envp[]);
int		taskstart(struct thr *thr, const struct thratr *atr,
		  	  void *(*func)(void *), void *arg);
int		tasksleep(struct thr *thr, unsigned int usecs);
int		taskwait(struct thr *thr, intptr_t wchan);
int		taskpause(struct thr *thr, sigset_t sigmask);
int		tasksig(struct thr *thr, int sig);
int		taskstop(struct thr *thr);
int		taskcont(struct thr *thr);
void	taskyield(void);

/* memory-interface */
void   *memalloc(size_t size);
void   *memalign(size_t size, size_t align);
void	memfree(void *ptr);
void   *memmap(void *adr, size_t size, int perm, int flags,
	   	       int fd, off_t ofs);
void	memunmap(void *adr, size_t size);
int		shmget(intptr_t key, size_t size, int flags);
int		shmat(intptr_t key, const void *adr, int flags);
int		shmdt(const void *adr);
int		shmopen(const char *name, int flags, int mode);
int		shmclose(const char *name);

/* I/O-interface */
int		ioopen(const char *path, int flags, ... /* int perm */);
int		ioclose(int fd);
off_t		ioseek(int fd, off_t ofs, int whence);
int		ioread(int fd, void *buf, size_t size);
int		iowrite(int fd, void *buf, size_t size);
int		ioreadv(int fd, const struct iovec *iov, int iovcnt);
int		iowritev(int fd, const struct iovec *iov, int iovcnt);
int		iopoll(int fd, void *fdset);
int		iochdir(const char *path);
int		iochmod(const char *path, int perm);
int		iorename(const char *path, const char *dest);
int		iosymlink(const char *path, const char *link);
int		iounlink(const char *path);
int		ioctl(int fd, int req, ... /* void *arg */);
int		iomkdir(const char *path, int perm);
struct dirent  *ioreaddir(DIR *dir);
int		ioscandir(const char *path, struct dirent ***list,
			  int (*filter)(const struct dirent *entry),
			  int (*cmp)(const struct dirent **entry1,
			      	     const struct dirent **entry2));
int		ioclosedir(DIR *dir);

IPC; Inter-Process Communications
---------------------------------

int		mqopen(intptr_t key, int flags);
int		mqsend(int mqid, const void *msg, size_t size, int flags);
int		mqrecv(int mqid, const void *msg, size_t size, int type,
		       int flags);
int		mqctl(int mqid, int cmd, struct mqatr *atr);

#endif /* __ZEN_API_H__ */

