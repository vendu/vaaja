#ifndef V0_IO_H
#define V0_IO_H

#define IO_TTY_BUF_SIZE         16384       // pseudo-teletype buffer size
#define IO_BLK_BUF_SIZE         65536       // buffer-size for block devices
#define IO_PKT_BUF_SIZE         65536       // size of IPv4 packet

#define IO_DEV_RAW              0x00
#define IO_DEV_BLK              0x01
#define IO_DEV_SHM              0x02
#define IO_DEV_PKT              0x03

/* device capabilities */
#define IO_DEV_OPEN             (1 << 0)    // create and remove files
#define IO_DEV_SEEK             (1 << 1)    // free seek as opposed to rew/ffwd
#define IO_DEV_SYMLINK          (1 << 2)    // can host symbolic links
#define IO_DEV_MKDIR            (1 << 3)    // create and remove directories
#define IO_DEV_PIPE             (1 << 4)    // create and remove FIFOs
#define IO_DEV_SEM              (1 << 5)    // can host semaphores
#define IO_DEV_MQ               (1 << 6)    // create and remove message queues

#define IO_NODE_EXEC            (1 << 0)
#define IO_NODE_READ            (1 << 1)
#define IO_NODE_WRITE           (1 << 2)
#define IO_NODE_APPEND          (1 << 3)
#define IO_NODE_SYNC            (1 << 4)
#define IO_NODE_SEQ             (1 << 5)
#define IO_NODE_MAP             (1 << 6)

struct iodev {
    unsigned char              *name;       // e.g. "tty00"
    void                       *buf;        // buffer (DMA)
    size_t                      bufsize;    // buffer size in bytes
    int32_t                     status;     // device status/error
    int32_t                     dev;        // 16-bit major, 16-bit minor
    int32_t                     cap;        // capabilities
    int32_t                     flg;        // see above
    int_fast8_t                 dma;        // DMA-channel ID
};

#endif /* V0_IO_H */

