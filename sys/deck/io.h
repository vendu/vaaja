#ifndef DECK_IO_H
#define DECK_IO_H

#define DECK_MAX_IO_PORTS       (1 << DECK_PORT_BITS)
#define DECK_PORT_BITS          6

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
#define DECK_CPU_BASE_PORT      0x000a  // processor interface ports
#   define DECK_RESET_CPU       0x00    // reset processor to initial state
#   define DECK_START_CPU       0x01    // start processor
#   define DECK_STOP_CPU        0x02    // stop processor (energy-saving)
#   define DECK_SEND_CPU        0x03    // send interrupt/protocol messages
#define DECK_CPU_PORT(i)        (DECK_CPU_BASE_PORT + (i))

/* commands for the IOC-instruction */
#define IOC_CHK_PERM            0x00    // check permission bit for I/O port
#define IOC_SET_PERM            0x01    // allow I/O on port
#define IOC_CLR_PERM            0x02    // deny I/O on port

#endif /* DECK_IO_H */

