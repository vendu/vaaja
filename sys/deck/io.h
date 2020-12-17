#ifndef DECK_IO_H
#define DECK_IO_H

#define DECK_IO_PERM_MAP        (1 << 0)    // system may map I/O-object
#define DECK_IO_PERM_WRITE      (1 << 1)    // system may write
#define DECK_IO_PERM_READ       (1 << 2)    // system may read
#define DECK_IO_PERM_UMAP       (1 << 3)    // user may I/O-object
#define DECK_IO_PERM_UWRITE     (1 << 4)    // user may write
#define DECK_IO_PERM_UREAD      (1 << 5)    // user may read

#define DECK_MAX_IO_PORTS       65536
#define DECK_PORT_BITS          16

#define DECK_TMR_PORT           0x0000  // interrupt timer
#   define DECK_RESET_TMR       0x00    // zero time-stamp
#   define DECK_CONF_TMR        0x01    // set timer interrupt frequency
#   define DECK_READ_TMR        0x02    // read time-stamp value
#   define DECK_STOP_TMR        0x03    // disable timer interrupts
#   define DECK_SET_TMR         0x04    // set time-stamp
#define DECK_KBD_PORT           0x0001  // keyboard
#   define DECK_RESET_KBD       0x00    // restore boot-time/default values
#   define DECK_CONF_KBD        0x01    // configure typematic rate etc.
#   define DECK_READ_KBD        0x02    // read keycode (32-bit signed)
#define DECK_HID_PORT           0x0002  // human interface devices, e.g. mouse
#   define DECK_RESET_HID       0x00
#   define DECK_CONF_HID        0x01
#   define DECK_READ_HID        0x02
#   define DECK_STOP_HID        0x03    // disable HID interrupts (energy-save)
#define DECK_SND_PORT           0x0003  // audio interface
#   define DECK_RESET_SND       0x00    // restore system default values
#   define DECK_CONF_SND        0x01    // # of channels, sample rate/size, buf
#   define DECK_PLAY_SND        0x02    // play sample or stream
#   define DECK_READ_SNG        0x03    // read/record audio
#define DECK_GFX_PORT           0x0004  // graphics interface
#   define DECK_RESET_GFX       0x00    // restore system default values
#   define DECK_CONF_GFX        0x01    // resolution, pixfmt, fps, ...
#   define DECK_READ_GFX        0x02    // capture video
#   define DECK_STOP_GFX        0x03    // stop screen output (energy-saving)
#define DECK_DRIVE_PORT         0x0005  // disk, optical, and solid-state drives
#   define DECK_RESET_DRIVE     0x00    // restore system default values
#   define DECK_START_DRIVE     0x01    // spin up (for relevant devices)
#   define DECK_OPEN_DRIVE      0x02
#   define DECK_STOP_DRIVE      0x03
#   define DECK_CLOSE_DRIVE     0x04
#   define DECK_SEEK_DRIVE      0x05
#   define DECK_READ_DRIVE      0x06
#   define DECK_WRITE_DRIVE     0x07
#define DECK_CARD_PORT          0x0006  // flash and 'punch' cards :)
#   define DECK_PROBE_CARD      0x00
#   define DECK_READ_CARD       0x01
#   define DECK_PUNCH_CARD      0x02
#   define DECK_ERASE_CARD      0x03
#define DECK_TAPE_PORT          0x0007  // tape drives
#   define DECK_RESET_TAPE      0x00    // write buffers out, rewind
#   define DECK_SEEK_TAPE       0x01    // seek to position or BEG, END, ...
#   define DECK_READ_TAPE       0x02    // read blocks
#   define DECK_WRITE_TAPE      0x03    // write blocks
#   define DECK_ERASE_TAPE      0x04    // write full of 0-bits
#define DECK_RTC_PORT           0x0008  // real-time clock
#   define DECK_RESET_RTC       0x00    // reset to Jan 1 1970 (0)
#   define DECK_READ_RTC        0x01
#   define DECK_SET_RTC         0x02
#define DECK_BUS_PORT           0x0009  // buses such as USB
#   define DECK_PROBE_BUS       0x00
#   define DECK_INIT_BUS        0x01
#   define DECK_START_BUS       0x02
#   define DECK_STOP_BUS        0x03
#   define DECK_CLOSE_BUS       0x04
#   define DECK_SEEK_BUS        0x05
#   define DECK_READ_BUS        0x06
#   define DECK_WRITE_BUS       0x07
#define DECK_CPU_BASE_PORT      0x0c00  // processor interface ports (max 1024)
#   define DECK_RESET_CPU       0x00    // reset processor to initial state
#   define DECK_START_CPU       0x01    // start processor
#   define DECK_STOP_CPU        0x02    // stop processor (energy-saving)
#   define DECK_SEND_CPU        0x03    // send interrupt/protocol messages
#define DECK_CPU_PORT(i)        (DECK_CPU_BASE_PORT + (i))

/* commands for the IOC-instruction */
#define IOC_CHK_PERM            0x00    // check permission bit for I/O port
#define IOC_SET_PERM            0x01    // allow I/O on port
#define IOC_CLR_PERM            0x02    // deny I/O on port

#define DECK_IO_INVAL           0x00    // invalid/uninitialized object or op
#define DECK_IO_PROBE           0x01    // probe I/O-device
#define DECK_IO_INIT            0x02    // initialize I/O-node
#define DECK_IO_MOUNT           0x03    // mount I/O-node
#define DECK_IO_UNMOUNT         0x04    // unmount I/O-node
#define DECK_IO_OPEN            0x05    // open I/O-node
#define DECK_IO_CLOSE           0x06    // close I/O-node
#define DECK_IO_SEEK            0x07    // seek I/O-node
#define DECK_IO_READ            0x08    // read from I/O-node
#define DECK_IO_WRITE           0x09    // write to I/O-node
#define DECK_IO_MAP             0x0a    // map regions of I/O-nodes
#define DECK_IO_UNMAP           0x0b    // map regions of I/O-nodes
#define DECK_IO_SYNC            0x0c    // synchronize I/O-node
#define DECK_IO_RDBUF           0x0d    // read data from node to buffer
#define DECK_IO_WRBUF           0x0e    // write data from buffer to node
#define DECK_IO_LINK            0x0f
#define DECK_IO_UNLINK          0x10
#define DECK_IO_MKDIR           0x11
#define DECK_IO_RMDIR           0x12
#define DECK_IO_READDIR         0x13
#define DECK_IO_POLL            0x14
#define DECK_IO_FLUSH           0x15
#define DECK_IO_RELEASE         0x16
#define DECK_IO_FSYNC           0x17
#define DECK_IO_FLOCK           0x18
#define DECK_IO_FUNLOCK         0x19
#define DECK_IO_FALLOC          0x1a
#define DECK_IO_AREAD           0x1b
#define DECK_IO_AWRITE          0x1c
#define DECK_IO_AFSYNC          0x1d
#define DECK_IO_SETLEASE        0x1e
#define DECK_IO_IOCTL           0x1f

struct deckiodev {
    const char                 *path;
    uint32_t                    state;
    uint32_t                    major;
    uint32_t                    minor;
};

struct deckiobuf {
    struct iodev               *dev;
    void                       *buf;
    uint32_t                    bufsize;
    uint32_t                    bufpos;
    uint32_t                    fsize;
    uint32_t                    id;
};

#endif /* DECK_IO_H */

