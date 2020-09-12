#ifndef __SYS_ZEN_IO_H__
#define __SYS_ZEN_IO_H__

#define ZEN_IO_SYNC      (1 << 0)        // synchronous I/O mode
#define ZEN_IO_DMA       (1 << 1)        // DMA direct memory transfers
#define ZEN_IO_POLL      (1 << 2)        // polled I/O devices/files
#define ZEN_IO_REALTIME  (1 << 3)        // realtime/deadline I/O scheduler

/* values for the flg-member to denote presence of parameters */
#define SYS_IO_DEV_BIT   (1U << 0)
#define SYS_IO_BUS_BIT   (1U << 1)
#define SYS_IO_PERM_BIT  (1U << 2)
#define SYS_IO_BASE_BIT  (1U << 3)
#define SYS_IO_BUF_BIT   (1U << 4)
#define SYS_IO_SIZE_BIT  (1U << 5)
#define SYS_IO_DESC_SIZE (8 * V0_WORD_SIZE)
struct v0iodesc {
    void       *base;   // I/O-map such as framebuffer base address
    void       *buf;    // I/O read buffer base address
    int32_t     flg;    // flags listing parameters present
    int32_t     dev;    // device ID
    int32_t     bus;    // bus ID
    int32_t     perm;   // I/O-permission bits
    size_t      size;   // I/O-map size in bytes
    int32_t     _pad;   // pad to boundary of 8 v0regs
};

#endif /* __SYS_ZEN_IO_H__ */

