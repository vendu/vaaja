#ifndef __V0_MACH_H__
#define __V0_MACH_H__

#include <limits.h>
#include <v0/v0.h>

struct v0machatr {
    size_t       nbbram;            // size of BRAM-memory in bytes
    uint8_t     *bram;              // block RAM
    size_t       nbram;             // size of RAM-memory in bytes
    uint8_t     *ram;               // pointer to virtual machine "RAM"
    size_t       clshift;           // cacheline-size log2
    int8_t      *clbits;            // cacheline lock-bits
    size_t       pgshift;           // page-size log2
    v0pagedesc  *pagetab;           // page-table of v0pagedesc-entries
    size_t       niomap;            // number of I/O-descriptors
    v0iodesc   **iomap;             // v0iodesc *iomap[niomap]
};

struct v0genctx {
    v0wreg regs[V0_MAX_REGISTERS];  // scalar registers
};

struct v0fpuctx {
    v0dbl regs[V0_FPU_REGS];        // floating-point registers
};

struct v0memrom {
    long *hashpage(v0pagedesc pte);
    long *bufpage(void *tlb, v0pagedesc pte);
    long *chkpage(v0pagedesc pte, v0pagedesc flg);
    void *mappage(void *tlb, v0pagedesc pte, v0pagedesc flg);
    void *holdcl(void *adr);
    void *relcl(void *adr);
};

#define V0_BUS_MAX_DEVS 16
#define V0_BUS_ALL_DEVS (-1)
struct v0iorom {
    long *probe(long bus, long dev, void *atr, size_t n);
    void *map(long bus, long dev, void *atr, size_t nb);
    long  reset(long bus, long dev);
    long  start(long bus, long dev);
    long  stop(long bus, long dev);
};

struct v0sysrom {
    struct v0memrom mem;
};

/* values for the flg-member to denote presence of parameters */
#define SYS_IO_DEV_BIT   (1U << 0)
#define SYS_IO_BUS_BIT   (1U << 1)
#define SYS_IO_PERM_BIT  (1U << 2)
#define SYS_IO_BASE_BIT  (1U << 3)
#define SYS_IO_BUF_BIT   (1U << 4)
#define SYS_IO_SIZE_BIT  (1U << 5)
#define SYS_IO_DESC_SIZE (8 * V0_WORD_SIZE)
struct v0iodesc {
    v0reg  flg;                 // flags listing parameters present
    v0reg  dev;                 // device ID
    v0reg  bus;                 // bus ID
    v0reg  perm;                // I/O-permission bits
    v0ureg base;                // I/O-map such as framebuffer base address
    v0ureg buf;                 // I/O read buffer base address
    v0ureg size;                // I/O-map size in bytes
    v0reg  _pad;                // pad to boundary of 8 v0regs
};

struct v0iofuncs {
    int     *creat(const char *path, mode_t mode);
    int     *open(const char *path, int flg, ...);
    int     *close(int fd);
    off_t   *lseek(int fd, off_t ofs, int whence);
    int     *link(const char *src, const char *dest);
    int     *symlink(const char *src, const char *dest);
    int     *unlink(const char *path);
    ssize_t *read(int fd, void *buf, size_t nb);
    ssize_t *write(int fd, void *buf, size_t nb);
    void    *mmap(void *adr, size_t nb, int prot, int flg, int fd, off_t ofs);
    int     *mkdir(const char *path, mode_t mode);
    int     *rmdir(const char *path);

};

#endif /* __V0_SYS_H__ */

