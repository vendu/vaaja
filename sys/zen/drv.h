#ifndef __SYS_ZEN_DRV_H__
#define __SYS_ZEN_DRV_H__

struct zeniofuncs {
    int         (*io_creat)(const char *path, mode_t mode);
    int         (*io_open)(const char *path, int flg, ...);
    int         (*io_close)(int fd);
    off_t       (*io_lseek)(int fd, off_t ofs, int whence);
    int         (*io_link)(const char *src, const char *dest);
    int         (*io_symlink)(const char *src, const char *dest);
    int         (*io_unlink)(const char *path);
    ssize_t     (*io_read)(int fd, void *buf, size_t nb);
    ssize_t     (*io_write)(int fd, void *buf, size_t nb);
    void        (*io_mmap)(void *adr, size_t nb, int prot,
                           int flg, int fd, off_t ofs);
    int         (*io_mkdir)(const char *path, mode_t mode);
    int         (*io_rmdir)(const char *path);

};

#endif /* __SYS_ZEN_DRV_H__ */

