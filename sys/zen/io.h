#ifndef __SYS_ZEN_IO_H__
#define __SYS_ZEN_IO_H__

#define V0_IO_SYNC      (1 << 0)        // synchronous I/O mode
#define V0_IO_DMA       (1 << 1)        // DMA direct memory transfers
#define V0_IO_POLL      (1 << 2)        // polled I/O devices/files
#define V0_IO_REALTIME  (1 << 3)        // realtime/deadline I/O scheduler

#endif /* __SYS_ZEN_IO_H__ */

