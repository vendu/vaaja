#ifndef __V0_SYS_H__
#define __V0_SYS_H__

#include <v0/v0.h>

/* prec-member */
#define V0_TIME_NANO            1000000000L
#define V0_TIME_MICRO           1000000L
#define V0_TIME_MILLI           1000L
/* flg-member */
#define V0_TIME_SYNC            (1L << 0)
#define V0_TIME_UTC             (1L << 1)
#define V0_DEV_FLAG_BITS        16L
#define V0_BUS_BITS             8L
#define V0_DEV_BITS             8L
#define v0busid()               ((dev) >> V0_DEV_BITS)
#define v0devid(dev)            ((dev) & 0x00ffffffL)
struct v0timedev {
    m_word_t    prec;                   // precision/frequency in Hz
    m_word_t    drift;                  // clock drift if applicable
    m_word_t    dev;                    // bus + device IDs
    m_word_t    flg;                    // device flag-bits
};

/* IOP-commands */

#define V0_IO_PORT_CLR_PERM     0x00    // disable I/O-permission
#define V0_IO_PORT_SET_PERM     0x01    // enable I/O-permission
#define V0_IO_PORT_READ         0x02    // read data from port
#define V0_IO_PORT_WRITE        0x03    // write data to port
#define V0_IO_PORT_POLL         0x04    // check port for input
#define V0_IO_PORT_FLUSH        0x05    // commit pending write for port
#define V0_IO_PORT_WAIT         0x06    // wait for input on port
#define V0_IO_PORT_SYNC         0x07    // set port to synchronous I/O mode

/* IOC-commands */
#define V0_IO_PROBE_BUS         0
#define V0_IO_CONF_BUS          1
#define V0_IO_PROBE_DEV         2
#define V0_IO_CONF_DEV          3
#define V0_IO_LOCK_DEV          4
#define V0_IO_MAP_BUF           5
#define V0_IO_CONF_BUF          6
#define V0_IO_OPEN_DEV          7
#define V0_IO_CLOSE_DEV         8
#define V0_IO_SEEK_DEV          9
#define V0_IO_READ_DEV          10
#define V0_IO_WRITE_DEV         11
#define V0_IO_SYNC_BUF          12
#define V0_IO_FLUSH_BUF         13
#define V0_IO_POLL_DEV          14
#define V0_IO_WAIT_DEV          15

#define V0_IO_RAW               (1L << 31)      // raw/character-based I/O
#define V0_IO_BLK               (1L << 30)      // buffered block-based I/O
#define V0_IO_NO_BUF            (1L << 29)      // do not cache blocks
#define V0_IO_DIRECT            (1L << 28)      // direct memory access
#define V0_IO_SYNC              (1L << 27)      // synchronous I/O
#define V0_IO_POLL              (1L << 26)      // pollable object
#define V0_IO_BUSY              (1L << 25)      // I/O in progress
#define V0_IO_SIGIO             (1L << 24)      // send SIGIO on objects ready
#define V0_IO_SIGURG            (1L << 23)      // send SIGURG for ZEN_IO_SYNC

/* I/O control register IDs */
#define V0_PIC_REG              0x00    // programmable interrupt controller
#define V0_TMR_REG      		0x01    // timer configuration
#define V0_RTC_REG      		0x02    // real-time clock
#define V0_KBD_REG      		0x03    // keyboard configuration
#define V0_MOUSE_REG    		0x04    // mouse configuration
#define V0_DISPLAY_REG  		0x05    // display configuration
#define V0_BUS_REG      		0x06    // bus configuration
#define V0_DEV_REG      		0x07    // device configuration
/* I/O device classes */
#define V0_NO_DEV       		(-1)    // invalid/unset device
#define V0_TIME_DEV     		0x00    // [interrupt] timers
#define V0_HID_DEV      		0x01    // human interface device
#define V0_AUDIO_DEV    		0x02    // audio device
#define V0_VIDEO_DEV    		0x03    // video device
#define V0_MEDIA_DEV    		0x04    // media [such as disk] controller
#define V0_NET_DEV      		0x05    // network interfaces

#define V0_NET_ADR_LEN          32

/* adrtype-member */
#define V0_NET_IPV4_ADR 		0x00
#define V0_NET_IPV6_ADR 		0x01
struct v0netdev {
    uint8_t     adr[V0_NET_ADR_LEN];    // interface network address such as IP
    m_word_t    adrsize;                // size of address structure in bytes
    m_word_t    adrtype;                // address family (IPv4, IPv6, ...)
    m_word_t    type;                   // interface type
    m_word_t    flg;                    // permissions + interface-specific bits
    m_word_t    irq;                    // interrupt-request channel
    m_word_t    dma;                    // direct memory access channel
};

#endif /* __V0_SYS_H__ */

