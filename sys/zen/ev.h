#ifndef __SYS_ZEN_EV_H__
#define __SYS_ZEN_EV_H__

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <gfx/rgb.h>

#define ZEN_EV_OBJ_BITS  32
#define ZEN_EV_TIME_BITS 32
#define ZEN_EV_WORD_BITS 32

#if (ZEN_EV_TIME_BITS == 64)
typedef uint64_t zenevtime_t;
#elif (ZEN_EV_TIME_BITS == 32)
typedef uint32_t zenevtime_t;
#endif

#define ZEN_EV_KEYCODE_BITS     32 // number of bits for keys in event keycodes
#define ZEN_EV_KEY_UP_BIT       (1L << 31)
#define ZEN_EV_KEY_BITS         21 // space for up to 0x1fffff
#define ZEN_EV_KEY_NONE         0 // no key value provided
#define ZEN_EV_KEY_INVAL        (~0L) // invalid key/error return
typedef int32_t                 zenkeycode_t; // transport keycode of 32 bits
typedef int32_t                 zenkeystate_t; // modifier + button state

/* library/user-level keycode type */
#if (ZEN_EV_KEYCODE_BITS <= 16)
typedef int16_t  zenevkey_t;
#else
typedef int32_t  zenevkey_t;
#endif

#if (ZEN_EV_WORD_BITS == 32)
typedef int32_t  zenevword_t;
typedef uint32_t zenevuword_t;
#elif (ZEN_EV_WORD_BITS == 64)
typedef int64_t  zenevword_t;
typedef uint64_t zenevuword_t;
#endif

/* internal protocol events
 * - response event ID of 0 is protocol messages, event ID 1 for protocol errors
 */
#define ZEN_EV_MASK(id)                  ((id) - 2)
#define ZEN_ERROR_MASK(id)               (0xf0 | ZEN_EV_MASK(id))
#define ZEN_REQUEST_MASK(id)             (0x80 | ZEN_EV_MASK(id))
#define ZEN_NOTIFY_MASK(id)              (0x40 | ZEN_EV_MASK(id))
#define ZEN_HINT_MASK(id)                (0x20 | ZEN_EV_MASK(id))
/* protocol events */
#define ZEN_PROTO_MSG                    0 // protocol message
#define ZEN_PROTO_ERROR                  1 // protocol error
/* keyboard events */
#define ZEN_KEY_PRESS                    2 // keypress event
#define ZEN_KEY_RELEASE                  3 // keyrelease event
/* pointer events */
#define ZEN_BUTTON_PRESS                 4 // buttonpress event
#define ZEN_BUTTON_RELEASE               5 // buttonrelease event
/* window events */
#define ZEN_MAP                          6 // map notification
#define ZEN_UNMAP                        7 // unmap notification
#define ZEN_CREATE                       8 // create notification
#define ZEN_DESTROY                      9 // destroy notification
#define ZEN_ENTER                       10 // enter window event
#define ZEN_LEAVE                       11 // leave window event
#define ZEN_MOTION                      12 // pointer/cursor motion event
#define ZEN_FOCUS_IN                    13 // focus in event
#define ZEN_FOCUS_OUT                   14 // focus out event
#define ZEN_CONFIGURE                   15 // configure notfication
#define ZEN_MOVE                        16
#define ZEN_RESIZE                      17
#define ZEN_MOVE_RESIZE                 18
#define ZEN_REPARENT                    19 // reparent notification
#define ZEN_CIRCULATE                   20 // circulation notification
#define ZEN_GRAVITY                     21 // gravity notification
#define ZEN_PROPERTY                    22 // property notification
#define ZEN_COLORMAP                    23 // colormap notification
#define ZEN_KEYMAP                      24 // keymap notification
/* struct deckevnote */
#define ZEN_MESSAGE                     25 // message from another client
#define ZEN_EXPOSE                      26 // exposure notification
#define ZEN_VISIBILITY                  27 // visibility notification
#define ZEN_KILL                        28 // kill window
#define ZEN_LOG_OUT                     29
#define ZEN_SHUT_DOWN                   30
/* events for window managers */

/*
 * bitmask values used to choose input events
 */
#define ZEN_NO_EVENTS_BIT               0
#define ZEN_KEY_PRESS_BIT               ZEN_EV_MASK(ZEN_KEY_PRESS)
#define ZEN_KEY_RELEASE_BIT             ZEN_EV_MASK(ZEN_KEY_RELEASE)
#define ZEN_KEY_EV_MASK                 (ZEN_KEY_PRESS_BIT              \
                                         | ZEN_KEY_RELEASE_BIT)
#define ZEN_BUTTON_PRESS_BIT            ZEN_EV_MASK(ZEN_BUTTON_PRESS)
#define ZEN_BUTTON_RELEASE_BIT          ZEN_EV_MASK(ZEN_BUTTON_RELEASE)
#define ZEN_BUTTON_EV_MASK              (ZEN_BUTTON_PRESS_BIT           \
                                         | ZEN_BUTTON_RELEASE_BIT)
#define ZEN_BUTTON_MOTION_MASK          ZEN_NOTIFY_MASK(ZEN_BUTTON_PRESS)
#define ZEN_POINTER_MOTION_BIT          ZEN_EV_MASK(ZEN_MOTION)
#define ZEN_POINTER_MOTION_MASK         ZEN_HINT_MASK(ZEN_MOTION)
#define ZEN_MAP_WINDOW_BIT              ZEN_EV_MASK(ZEN_MAP)
#define ZEN_ENTER_WINDOW_BIT            ZEN_EV_MASK(ZEN_ENTER)
#define ZEN_LEAVE_WINDOW_BIT            ZEN_EV_MASK(ZEN_LEAVE)
#define ZEN_EXPOSURE_BIT                ZEN_EV_MASK(ZEN_EXPOSE)
#define ZEN_VISIBILITY_NOTIFY_BIT       ZEN_EV_MASK(ZEN_VISIBILITY)
#define ZEN_STRUCTURE_NOTIFY_BIT
#define ZEN_RESIZE_REDIRECT_BIT
#define ZEN_SUBSTRUCTURE_NOTIFY_BIT
#define ZEN_SUBSTRUCTURE_REDIRECT_BIT
#define ZEN_FOCUS_NOTIFY_BIT
#define ZEN_PROPERTY_NOTIFY_BIT
#define ZEN_COLORMAP_NOTIFY_BIT
#define ZEN_OWNER_GRAB_BUTTON_BIT
#define ZEN_KEYMAP_NOTIFY_BIT

#define zengetevtype(ev)    ((ev)->hdr.type)
#define zensetevtype(ev, t) ((ev)->hdr.type = (t))
#define zengetevtime(ev)    ((ev)->hdr.tm)
#define zensetevtime(ev, t) ((ev)->hdr.tm = (t))

/* API */
#define zenevmask(id) (1U << (id))
/* TODO: implement ring-buffers for event queues */
/* - wired to physical memory permanently */
/* - event queue is mapped to both kernel and user space to avoid data copies */
/* - register to listen to ev with flg parameters */
/*   - zenregev returns pointer to dual-mapped event queue (set of pages) */
void * zenregev(long mask, long flg);
/* read next event from queue */
long   zenpeekev(struct deckev *ev, long mask);
/*
 * NOTE: the flg-bit values below are guaranteed not to overlap with event
 * masks
 */
/*
'* flg-value bits for zengetev() and zenputev()
 * --------------------------------------
 * - ZENGETEV, ZENPUTEV: flush queue unless flg has the EVNOFLUSH-bit set
 * - ZENGETEV:        remove from queue unless flg has the EVNOREMOVE-bit set
 */
/* read events; zengetev() */
#define ZEN_EV_CHECK            0x80000000 // check queue; don't flush
#define ZEN_EV_PEEK             0x40000000 // do not remove event from queue
/* queue events; zenputev() */
#define ZEN_EV_QUEUE            0x20000000
#define ZEN_EV_DEQUEUE          0x10000000
/* flg-argument bits for zensyncevq() */
#define ZEN_EV_SYNC             0x08000000 // asynchronous if not set
#define ZEN_EV_DISCARD          0x04000000 // discard pending user input

#define zenpeekev(deck, ev, flg) zengetev((deck), (ev), (flg) | EVNOREMOVE)
void    zengetev(struct deck *deck, struct deckev *ev, long flg);
long    zenputev(struct deck *deck, struct deckev *ev, long flg);
void    zensyncev(struct deck *deck, long flg);

#if defined(__zen__)

#if !defined(RING_ITEM)
#define RING_ITEM  struct deckev
#endif
#if !defined(RING_INVAL)
#define RING_INVAL { 0 }
#endif
#if !defined(MALLOC)
#define MALLOC(sz) kmalloc(sz)
#endif

#else /* !defined(__zen__) */

#define RING_ITEM  struct deckev
#define RING_INVAL NULL
#define MALLOC(sz) malloc(sz)

#include <zero/ring.h>

#endif /* defined(__zen__) */

/* event interface */

/* keyboard events */

/*
 * - the kernel passes raw keyboard scan-codes to userland as 64-bit integers
 * - the longest scan-codes I'm aware on PC keyboards are 6-byte
 * - these scan-codes are read from the keyboard byte by byte
 * - we leave the translation to userland according to locale and such settings
 * - userland processing of scan-codes keeps our interrupt handler minimal
 */

/* keycode special-bits */
#define ZEN_EV_KBD_STATE        0x80000000 // modifier and button state present
/* modifier flags */
#define ZEN_EV_MOD(x)           (1L << (32 - (x)))
#define ZEN_EV_MOD_SHIFT_BIT    ZEN_EV_MOD(ZEN_EV_MOD_SHIFT)
#define ZEN_EV_MOD_CAPSLK_BIT   ZEN_EV_MOD(ZEN_EV_MOD_CAPSLK)
#define ZEN_EV_MOD_CTRL_BIT     ZEN_EV_MOD(ZEN_EV_MOD_CTRL)
#define ZEN_EV_MOD_META_BIT     ZEN_EV_MOD(ZEN_EV_MOD_META)
#define ZEN_EV_MOD_COMPOSE_BIT  ZEN_EV_MOD(ZEN_EV_MOD_COMPOSE)
#define ZEN_EV_MOD_ALT_BIT      ZEN_EV_MOD(ZEN_EV_MOD_ALT)
#define ZEN_EV_MOD_ALTGR_BIT    ZEN_EV_MOD(ZEN_EV_MOD_ALTGR)
#define ZEN_EV_MOD_SCRLK_BIT    ZEN_EV_MOD(ZEN_EV_MOD_SCRLK)
#define ZEN_EV_MOD_NUMLK_BIT    ZEN_EV_MOD(ZEN_EV_MOD_NUMLK)
/* button masks/IDs */
#define ZEN_EV_BUTTON(i)        (1 << ((i) - 1)
#define ZEN_EV_BUTTON_1         ZEN_EV_BUTTON(1)
#define ZEN_EV_BUTTON_2         ZEN_EV_BUTTON(2)
#define ZEN_EV_BUTTON_3         ZEN_EV_BUTTON(3)
#define ZEN_EV_BUTTON_4         ZEN_EV_BUTTON(4)
#define ZEN_EV_BUTTON_5         ZEN_EV_BUTTON(5)
#define ZEN_EV_BUTTON_6         ZEN_EV_BUTTON(6)
#define ZEN_EV_BUTTON_7         ZEN_EV_BUTTON(7)
#define ZEN_EV_BUTTON_8         ZEN_EV_BUTTON(8)
#define ZEN_EV_BUTTON_9         ZEN_EV_BUTTON(9)
#define ZEN_EV_BUTTON_10        ZEN_EV_BUTTON(10)
#define ZEN_EV_BUTTON_11        ZEN_EV_BUTTON(11)
#define ZEN_EV_BUTTON_12        ZEN_EV_BUTTON(12)
#define ZEN_EV_BUTTON_13        ZEN_EV_BUTTON(13)
#define ZEN_EV_BUTTON_14        ZEN_EV_BUTTON(14)
#define ZEN_EV_BUTTON_15        ZEN_EV_BUTTON(15)
#define ZEN_EV_BUTTON_16        ZEN_EV_BUTTON(16)
#define ZEN_KEY_STATE_BUTTONS   16
#define EVBUTTONMASK            ((1L << ZEN_KEY_STATE_BUTTONS) - 1)
/* state-bits for modifier keys */
#define ZEN_EV_MOD_NONE         0
#define ZEN_EV_MOD_SHIFT        1
#define ZEN_EV_MOD_CAPSLK       2
#define ZEN_EV_MOD_CTRL         3
#define ZEN_EV_MOD_META         4
#define ZEN_EV_MOD_COMPOSE      5
#define ZEN_EV_MOD_ALT          6
#define ZEN_EV_MOD_ALTGR        7
#define ZEN_EV_MOD_SCRLK        8
#define ZEN_EV_MOD_NUMLK        9
#define ZEN_KEY_MOD_BITS        10
#define ZEN_KEY_STATE_MODS      16
#define ZEN_EV_MOD_MASK         ((1L << ZEN_KEY_STATE_BUTTONS) - 1)

/* keyboard event macros */
#define ZEN_KEY_MASK            0x01ffffffL              // Unicode 1..0x10fffff
#define zenkeysym(key)          ((key) & ZEN_KEY_MASK)
#define zenkeyhasstate(key)     ((key) & ZEN_KBD_STATE_BIT)  // have state
#define zenbuttondown(id, b)    ((id) & ZEN_EV_BUTTON(b)) // button pressed
#define zengetkeybuttons(ev, mask) ((ev)->state & (mask))    // buttons pressed
#define zengetkeymod(ev, mod)   ((ev)->state & ZEN_EV_MOD(mod)) // modifiers?
#define zengetkeymods(ev, mask) ((ev)->state & ZEN_EV_MOD_MASK) // modifiers

/* pointer such as mouse device events */

#define zenpnthasbutton(ev, b) ((ev)->state & (1 << (b)))
/* pointer device, e.g. mouse event */
struct zencoord {
    zenevword_t         x;      // screen X coordinate
    zenevword_t         y;      // screen Y coordinate
    zenevword_t         z;      // screen Z coordinate
};

/* cmd-field for SYS */
/* EVSHUTDOWN is delivered no matter what events have been selected */
#define ZEN_SYS_NO_OP           0
#define ZEN_SYS_REBOOT          1 // reboot
#define ZEN_SYS_SHUTDOWN        2 // shutdown
#define ZEN_SYS_SLEEP           3 // power-saving sleep
#define ZEN_SYS_WAKEUP          4 // ignite parts of system
#define ZEN_SYS_HIBERNATE       5 // shutdown, store state for recovery
#define ZEN_SYS_DEEP_SIX        6 // forceful shutdown
#define ZEN_SYS_EVENT_TYPES     7

/* cmd-field for SYSHWNOTIFY */
/* hardware/driver events */
#define ZEN_HW_NO_OP            0
#define ZEN_HW_LOAD             1 // kernel module loaded
#define ZEN_HW_UNLOAD           2 // kernel module unloaded
#define ZEN_HW_PLUG             3 // new device plugged
#define ZEN_HW_UNPLUG           4 // device unplugged
#define ZEN_HW_EVENT_TYPES      5

/* cmd-field for SYSFSNOTIFY */
/* event names */
#define ZEN_FS_NO_OP            0
#define ZEN_FS_MOUNT            1 // filesystem mount event
#define ZEN_FS_UNMOUNT          2 // filesystem unmount event
#define ZEN_FS_CREAT            3 // file creation event
#define ZEN_FS_UNLINK           4 // file unlink event
#define ZEN_FS_MKDIR            5 // add directory
#define ZEN_FS_RMDIR            6 // remove directory
#define ZEN_FS_EVENT_TYPES      7

struct zencmd {
    zenevword_t         func;   /* msg present iff ZEN_MSG_BIT is set in cmd */
    zenevword_t         obj;    /* target object ID
};

/* event structure for ZEN_SYS_EV, ZEN_HW_EV, and ZEN_FS_EV */
struct zenmsg {
    zenevword_t size; // # of bytes in msg
    uint8_t     data[EMPTY]; // message text/data
};

struct zenreqev {
    ;
};

/*
 * TODO: move this comment elsewhere...
 * Unicode specifies 0x10ffff as maximum value, leaving us with 11 high bits to
 * be used as flags if need be; let's start using them from the highest ones to
 * be safer, though
 */

/* FIXME: tm may need to be 64-bit for everyone */
struct zenevhdr {
    zenevtime_t         tm;   // timestamp; 32- or 64-bit
    union {
        struct {
            unsigned    type : 24;
            unsigned    dest : 8;
        } u;
        struct {
            unsigned    code : 21;
            unsigned    flg  : 11;
        } key;
        union {
            uint16_t    bits;
            uint16_t    map;
        } btn;
        uint32_t        u32;
    } data[VLA];
    //    zenevuword_t        type; // event type such as KEYUP, FSCREAT
};

/* event structure */
struct zenev {
    struct zenevhdr             hdr;
    union {
        /* actual event message */
        /*
         * ZEN_KEY_PRESS, ZEN_KEY_RELEASE,
         * ZEN_BUTTON_PRESS, ZEN_BUTTON_RELEASE,
         * ZEN_POINTER_MOTION, ZEN_BUTTON_MOTION
         */
        struct zencoord         xyz;
        struct {
            struct zencmd       op;
            struct zenmsg       msg[VLA];
        } cmd;
    } msg;
};

#endif /* __SYS_ZEN_EV_H__ */

