#ifndef DECK_IO_H
#define DECK_IO_H

#define DECK_MAX_IO_PORTS       256

#define DECK_TMR_PORT           0x0000
#   define DECK_RESET_TMR       0x00    // zero time-stamp
#   define DECK_CONF_TMR        0x01    // set timer interrupt frequency
#   define DECK_READ_TMR        0x02    // read time-stamp value
#   define DECK_STOP_TMR        0x03    // disable timer interrupts
#   define DECK_SET_TMR         0x04    // set time-stamp
#define DECK_KBD_PORT           0x0001
#   define DECK_RESET_KBD       0x00    // restore boot-time/default values
#   define DECK_CONF_KBD        0x01    // configure typematic rate etc.
#   define DECK_READ_KBD        0x02    // read keycode (32-bit signed)
#define DECK_HID_PORT           0x0002
#   define DECK_RESET_HID       0x00
#   define DECK_CONF_HID        0x01
#   define DECK_READ_HID        0x02
#   define DECK_STOP_HID        0x03    // disable HID interrupts (energy-save)
#define DECK_SND_PORT           0x0003
#   define DECK_RESET_SND       0x00    // restore system default values
#   define DECK_CONF_SND        0x01    // # of channels, sample rate/size, buf
#   define DECK_PLAY_SND        0x02    // play sample or stream
#   define DECK_READ_SNG        0x03    // read/record audio
#define DECK_GFX_PORT           0x0004
#   define DECK_RESET_GFX       0x00    // restore system default values
#   define DECK_CONF_GFX        0x01    // resolution, pixfmt, fps, ...
#   define DECK_READ_GFX        0x02    // capture video
#   define DECK_STOP_GFX        0x03
#define DECK_DRIVE_PORT         0x0005
#   define DECK_RESET_DRIVE     0x00    // restore system default values
#   define DECK_START_DRIVE     0x01
#   define DECK_OPEN_DRIVE      0x02
#   define DECK_STOP_DRIVE      0x03
#   define DECK_CLOSE_DRIVE     0x04
#   define DECK_SEEK_DRIVE      0x05
#   define DECK_READ_DRIVE      0x06
#   define DECK_WRITE_DRIVE     0x07
#define DECK_CARD_PORT          0x0006
#   define DECK_PROBE_CARD      0x00
#   define DECK_READ_CARD       0x01
#   define DECK_PUNCH_CARD      0x02
#   define DECK_ERASE_CARD      0x03
#define DECK_TAPE_PORT          0x0007
#   define DECK_RESET_TAPE      0x00    // write buffers out, rewind
#   define DECK_SEEK_TAPE       0x01    // seek to position or BEG, END, ...
#   define DECK_READ_TAPE       0x02    // read blocks
#   define DECK_WRITE_TAPE      0x03    // write blocks
#   define DECK_ERASE_TAPE      0x04    // write full of 0-bits
#define DECK_RTC_PORT           0x0008
#   define DECK_RESET_RTC       0x00    // reset to Jan 1 1970 (0)
#   define DECK_READ_RTC        0x01
#   define DECK_SET_RTC         0x02
#define DECK_USB_PORT           0x0009
#   define DECK_PROBE_USB       0x00
#   define DECK_INIT_USB        0x01
#   define DECK_START_USB       0x02
#   define DECK_STOP_USB        0x03
#   define DECK_CLOSE_USB       0x04
#   define DECK_SEEK_USB        0x05
#   define DECK_READ_USB        0x06
#   define DECK_WRITE_USB       0x07
#define DECK_CPU_BASE_PORT      0x0010
#   define DECK_RESET_CPU       0x00
#   define DECK_START_CPU       0x01
#   define DECK_STOP_CPU        0x02
#   define DECK_SEND_CPU        0x03    // send interrupt/protocol messages
#define DECK_CPU_PORT(i)        (DECK_CPU_BASE_PORT + (i))

#endif /* DECK_IO_H */

