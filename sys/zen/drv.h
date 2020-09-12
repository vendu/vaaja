#ifndef __SYS_ZEN_DRV_H__
#define __SYS_ZEN_DRV_H__

struct zeniofuncs {
    int        *creat(const char *path, mode_t mode);
    int        *open(const char *path, int flg, ...);
    int        *close(int fd);
    off_t      *lseek(int fd, off_t ofs, int whence);
    int        *link(const char *src, const char *dest);
    int        *symlink(const char *src, const char *dest);
    int        *unlink(const char *path);
    ssize_t    *read(int fd, void *buf, size_t nb);
    ssize_t    *write(int fd, void *buf, size_t nb);
    void       *mmap(void *adr, size_t nb, int prot,
                     int flg, int fd, off_t ofs);
    int        *mkdir(const char *path, mode_t mode);
    int        *rmdir(const char *path);

};

#endif /* __SYS_ZEN_DRV_H__ */

