#ifndef SYS_TAIKA_EV_H
#define SYS_TAIKA_EV_H

//#include <gfx/rgb.h>
#include <stdint.h>

//#define TAIKA_EV_OBJ_BITS         64
//#define TAIKA_EV_TIME_BITS        64
//#define TAIKA_EV_WORD_BITS        64

#if defined(TAIKA_TIME_BITS) && (TAIKA_TIME_BITS == 64)
typedef uint64_t                    taikatime_t;
#else
typedef uint32_t                    taikatime_t;
#endif

#define TAIKA_EV_KEYCODE_BITS       32 // number of bits for keys in event keycodes
#define TAIKA_EV_KEY_UP_BIT         (1L << 31)
#define TAIKA_EV_BTN_STATE          (1L << 30) // button state present
#define TAIKA_EV_KEY_BITS           21 // space for up to 0x1fffff in Unicode
#define TAIKA_EV_KEY_NONE           0 // no key value provided
#define TAIKA_EV_KEY_INVAL          (~0L) // invalid key/error return
typedef int32_t                     taikakeycode_t; // transport keycode of 32 bits
typedef int32_t                     taikakeystate_t; // modifier + button state

/* library/user-level keycode type */
#if defined(TAIKA_EV_KEYCODE_BITS) && (TAIKA_EV_KEYCODE_BITS == 32)
typedef int32_t                     taikaevkey_t;
#else
typedef int16_t                     taikaevkey_t;
#endif

#if defined (TAIKA_EV_WORD_BITS) && (TAIKA_EV_WORD_BITS == 64)
typedef int64_t                     taikaevword_t;
typedef uint64_t                    taikaevuword_t;
#else
typedef int32_t                     taikaevword_t;
typedef uint32_t                    taikaevuword_t;
#endif

/* internal protocol events
 * - response event ID of 0 is protocol messages, event ID 1 for protocol errors
 */
#define TAIKA_EV_MASK(id)           (1 << ((id) - 2))
#define TAIKA_ERROR_MASK(id)        (0xf0 | TAIKA_EV_MASK(id))
#define TAIKA_REQUEST_MASK(id)      (0x80 | TAIKA_EV_MASK(id))
#define TAIKA_NOTIFY_MASK(id)       (0x40 | TAIKA_EV_MASK(id))
#define TAIKA_HINT_MASK(id)         (0x20 | TAIKA_EV_MASK(id))
/* protocol events */
#define TAIKA_PROTO_MSG             0 // protocol message
#define TAIKA_PROTO_ERROR           1 // protocol error
/* keyboard events */
#define TAIKA_KEY_PRESS             2 // keypress event
#define TAIKA_KEY_RELEASE           3 // keyrelease event
/* pointer events */
#define TAIKA_BUTTON_PRESS          4 // buttonpress event
#define TAIKA_BUTTON_RELEASE        5 // buttonrelease event
/* window events */
#define TAIKA_MAP                   6 // map notification
#define TAIKA_UNMAP                 7 // unmap notification
#define TAIKA_CREATE                8 // create notification
#define TAIKA_DESTROY               9 // destroy notification
#define TAIKA_ENTER                 10 // enter window event
#define TAIKA_LEAVE                 11 // leave window event
#define TAIKA_MOTION                12 // pointer/cursor motion event
#define TAIKA_FOCUS_IN              13 // focus in event
#define TAIKA_FOCUS_OUT             14 // focus out event
#define TAIKA_CONFIGURE             15 // configure notfication
#define TAIKA_REQUEST               16 // move, resize, moveresize
#define TAIKA_REPARENT              17 // reparent notification
#define TAIKA_CIRCULATE             19 // circulation notification
#define TAIKA_GRAVITY               20 // gravity notification
#define TAIKA_PROPERTY              21 // property notification
#define TAIKA_COLORMAP              22 // colormap notification
#define TAIKA_KEYMAP                23 // keymap notification
/* struct deckevnote */
#define TAIKA_MESSAGE               24 // message from another client
#define TAIKA_EXPOSE                25 // exposure notification
#define TAIKA_VISIBILITY            26 // visibility notification
#define TAIKA_KILL                  27 // kill window
#define TAIKA_DEEP_SIX              28 // DEEP_SIX_REBOOT
#define TAIKA_HIBERNATE             29
/* events for window managers */

/*
 * bitmask values used to choose input events
 */
//#define TAIKA_NO_EVENTS_BIT         0
#define TAIKA_KEY_PRESS_BIT         TAIKA_EV_MASK(TAIKA_KEY_PRESS)
#define TAIKA_KEY_RELEASE_BIT       TAIKA_EV_MASK(TAIKA_KEY_RELEASE)
#define TAIKA_KEY_EV_MASK           TAIKA_EV_MASK(TAIKA_KEY_PRESS_BIT   \
                                              | TAIKA_KEY_RELEASE_BIT)
#define TAIKA_BUTTON_PRESS_BIT      TAIKA_EV_MASK(TAIKA_BUTTON_PRESS
#define TAIKA_BUTTON_RELEASE_BIT    TAIKA_EV_MASK(TAIKA_BUTTON_RELEASE)
#define TAIKA_BUTTON_EV_MASK        (TAIKA_BUTTON_PRESS_BIT             \
                                     | TAIKA_BUTTON_RELEASE_BIT)
#define TAIKA_BUTTON_MOTION_MASK          TAIKA_NOTIFY_MASK(TAIKA_BUTTON_PRESS)
#define TAIKA_POINTER_MOTION_BIT          TAIKA_EV_MASK(TAIKA_MOTION)
#define TAIKA_POINTER_MOTION_MASK         TAIKA_HINT_MASK(TAIKA_MOTION)
#define TAIKA_MAP_WINDOW_BIT              TAIKA_EV_MASK(TAIKA_MAP)
#define TAIKA_ENTER_WINDOW_BIT            TAIKA_EV_MASK(TAIKA_ENTER)
#define TAIKA_LEAVE_WINDOW_BIT            TAIKA_EV_MASK(TAIKA_LEAVE)
#define TAIKA_EXPOSURE_BIT                TAIKA_EV_MASK(TAIKA_EXPOSE)
#define TAIKA_VISIBILITY_NOTIFY_BIT       TAIKA_EV_MASK(TAIKA_VISIBILITY)
#define TAIKA_STRUCTURE_NOTIFY_BIT
#define TAIKA_RESIZE_REDIRECT_BIT
#define TAIKA_SUBSTRUCTURE_NOTIFY_BIT
#define TAIKA_SUBSTRUCTURE_REDIRECT_BIT
#define TAIKA_FOCUS_NOTIFY_BIT
#define TAIKA_PROPERTY_NOTIFY_BIT
#define TAIKA_COLORMAP_NOTIFY_BIT
#define TAIKA_OWNER_GRAB_BUTTON_BIT
#define TAIKA_KEYMAP_NOTIFY_BIT

#define taikagetevtype(ev)          ((ev)->hdr.type)
#define taikasetevtype(ev, t)       ((ev)->hdr.type = (t))
#define taikagetevtime(ev)          ((ev)->hdr.tm)
#define taikasetevtime(ev, t)       ((ev)->hdr.tm = (t))

/* API */
#define taikaevmask(id)             (1U << (id))
/* TODO: implement ring-buffers for event queues */
/* - wired to physical memory permanently */
/* - event queue is mapped to both kernel and user space to avoid data copies */
/* - register to listen to ev with flg parameters */
/*   - taikaregev returns pointer to dual-mapped event queue (set of pages) */
void * taikaregev(long mask, long flg);
/* read next event from queue */
long   taikapeekev(struct taikaev *ev, long mask);
/*
 * NOTE: the flg-bit values below are guaranteed not to overlap with event
 * masks
 */
/*
'* flg-value bits for taikagetev() and taikaputev()
 * --------------------------------------
 * - TAIKAGETEV, TAIKAPUTEV: flush queue unless flg has the EVNOFLUSH-bit set
 * - TAIKAGETEV:        remove from queue unless flg has the EVNOREMOVE-bit set
 */
/* read events; taikagetev() */
#define TAIKA_CHECK_EV              0x80000000 // check queue; don't flush
#define TAIKA_PEEK_EV               0x40000000 // do not remove event from queue
/* queue events; taikaputev() */
#define TAIKA_QUEUE_EV              0x20000000 // put event at back of queue
#define TAIKA_DEQUEUE_EV            0x10000000 // get evnt from front of queue
/* flg-argument bits for taikasyncev() */
#define TAIKA_SYNC_EV               0x08000000 // asynchronous if not set
#define TAIKA_DISCARD_INPUT         0x04000000 // discard pending user input
#define TAIKA_DISCARD_OUTPUT        0x02000000

/* event interface */

/* keyboard events */

/*
 * - the kernel passes raw keyboard scan-codes to userland as 64-bit integers
 * - the longest scan-codes I'm aware on PC keyboards are 6-byte
 * - these scan-codes are read from the keyboard byte by byte
 * - we leave the translation to userland according to locale and such settings
 * - userland processing of scan-codes keeps our interrupt handler minimal
 */

/* modifier flags */
#define TAIKA_EV_MOD(x)             (1L << (32 - (x)))
#define TAIKA_EV_MOD_SHIFT_BIT      TAIKA_EV_MOD(TAIKA_EV_MOD_SHIFT)
#define TAIKA_EV_MOD_CAPSLK_BIT     TAIKA_EV_MOD(TAIKA_EV_MOD_CAPSLK)
#define TAIKA_EV_MOD_CTRL_BIT       TAIKA_EV_MOD(TAIKA_EV_MOD_CTRL)
#define TAIKA_EV_MOD_META_BIT       TAIKA_EV_MOD(TAIKA_EV_MOD_META)
#define TAIKA_EV_MOD_COMPOSE_BIT    TAIKA_EV_MOD(TAIKA_EV_MOD_COMPOSE)
#define TAIKA_EV_MOD_ALT_BIT        TAIKA_EV_MOD(TAIKA_EV_MOD_ALT)
#define TAIKA_EV_MOD_ALTGR_BIT      TAIKA_EV_MOD(TAIKA_EV_MOD_ALTGR)
#define TAIKA_EV_MOD_SCRLK_BIT      TAIKA_EV_MOD(TAIKA_EV_MOD_SCRLK)
#define TAIKA_EV_MOD_NUMLK_BIT      TAIKA_EV_MOD(TAIKA_EV_MOD_NUMLK)
/* button masks/IDs */
#define TAIKA_EV_BUTTON(i)          (1 << ((i) - 1)
#define TAIKA_EV_BUTTON_1           TAIKA_EV_BUTTON(1)
#define TAIKA_EV_BUTTON_2           TAIKA_EV_BUTTON(2)
#define TAIKA_EV_BUTTON_3           TAIKA_EV_BUTTON(3)
#define TAIKA_EV_BUTTON_4           TAIKA_EV_BUTTON(4)
#define TAIKA_EV_BUTTON_5           TAIKA_EV_BUTTON(5)
#define TAIKA_EV_BUTTON_6           TAIKA_EV_BUTTON(6)
#define TAIKA_EV_BUTTON_7           TAIKA_EV_BUTTON(7)
#define TAIKA_EV_BUTTON_8           TAIKA_EV_BUTTON(8)
#define TAIKA_EV_BUTTON_9           TAIKA_EV_BUTTON(9)
#define TAIKA_EV_BUTTON_10          TAIKA_EV_BUTTON(10)
#define TAIKA_EV_BUTTON_11          TAIKA_EV_BUTTON(11)
#define TAIKA_EV_BUTTON_12          TAIKA_EV_BUTTON(12)
#define TAIKA_EV_BUTTON_13          TAIKA_EV_BUTTON(13)
#define TAIKA_EV_BUTTON_14          TAIKA_EV_BUTTON(14)
#define TAIKA_EV_BUTTON_15          TAIKA_EV_BUTTON(15)
#define TAIKA_EV_BUTTON_16          TAIKA_EV_BUTTON(16)
#define TAIKA_KEY_STATE_BUTTONS     16
#define TAIKA_BUTTON_MASK           0xffff
/* state-bits for modifier keys */
#define TAIKA_EV_MOD_NONE           0
#define TAIKA_EV_MOD_SHIFT          (1L << 25)
#define TAIKA_EV_MOD_CAPSLK         (1L << 24)
#define TAIKA_EV_MOD_CTRL           (1L << 23)
#define TAIKA_EV_MOD_META           (1L << 22)
#define TAIKA_EV_MOD_COMPOSE        (1L << 21)
#define TAIKA_EV_MOD_ALT            (1L << 20)
#define TAIKA_EV_MOD_ALTGR          (1L << 19)
#define TAIKA_EV_MOD_SCRLK          (1L << 18)
#define TAIKA_EV_MOD_NUMLK          (1L << 17)
#define TAIKA_EV_MOD_MASK           0x01ff0000

/* keyboard event macros */
#define TAIKA_KEY_MASK              0x01ffffffL              // Unicode 1..0x10fffff
#define taikakeysym(key)            ((key) & TAIKA_KEY_MASK)
#define taikakeyhasstate(key)       ((key) & TAIKA_KBD_STATE_BIT)  // have state
#define taikabuttondown(id, b)      ((id) & TAIKA_EV_BUTTON(b)) // button pressed
#define taikagetkeybuttons(ev, mask) ((ev)->state & (mask))    // buttons pressed
#define taikagetkeymod(ev, mod)     ((ev)->state & TAIKA_EV_MOD(mod)) // modifiers?
#define taikagetkeymods(ev, mask)   ((ev)->state & TAIKA_EV_MOD_MASK) // modifiers

/* pointer such as mouse device events */

#define taikapnthasbutton(ev, b)    ((ev)->state & (1 << (b)))
/* pointer device, e.g. mouse event */
struct taikacoord {
    taikaevword_t         x;      // screen X coordinate
    taikaevword_t         y;      // screen Y coordinate
    taikaevword_t         z;      // screen Z coordinate
};

/* cmd-field for SYS */
/* EVSHUTDOWN is delivered no matter what events have been selected */
#define TAIKA_SYS_NO_OP             0
#define TAIKA_SYS_REBOOT            1 // reboot
#define TAIKA_SYS_SHUTDOWN          2 // shutdown
#define TAIKA_SYS_SLEEP             3 // power-saving sleep
#define TAIKA_SYS_WAKEUP            4 // ignite parts of system
#define TAIKA_SYS_HIBERNATE         5 // shutdown, store state for recovery
#define TAIKA_SYS_DEEP_SIX          6 // forceful shutdown
#define TAIKA_SYS_EVENT_TYPES       7

/* cmd-field for SYSHWNOTIFY */
/* hardware/driver events */
#define TAIKA_HW_NO_OP              0
#define TAIKA_HW_LOAD               1 // kernel module loaded
#define TAIKA_HW_UNLOAD             2 // kernel module unloaded
#define TAIKA_HW_PLUG               3 // new device plugged
#define TAIKA_HW_UNPLUG             4 // device unplugged
#define TAIKA_HW_EVENT_TYPES        5

/* cmd-field for SYSFSNOTIFY */
/* event names */
#define TAIKA_FS_NO_OP              0
#define TAIKA_FS_MOUNT              1 // filesystem mount event
#define TAIKA_FS_UNMOUNT            2 // filesystem unmount event
#define TAIKA_FS_CREAT              3 // file or symlink creation event
#define TAIKA_FS_UNLINK             4 // unlink event
#define TAIKA_FS_MKDIR              5 // add directory
#define TAIKA_FS_RMDIR              6 // remove directory
#define TAIKA_FS_EVENT_TYPES        7

struct taikacmd {
    taikaevword_t                   func;   /* set iff TAIKA_MSG_BIT is set in cmd */
    taikaevword_t                   obj;    /* target object ID */
};

/* event structure for TAIKA_SYS_EV, TAIKA_HW_EV, and TAIKA_FS_EV */
struct taikamsg {
    taikaevword_t                   size; // # of bytes in msg
    uint8_t                         data[C_VLA]; // message text/data
};

struct taikareqev {
    ;
};

/*
 * TODO: move this comment elsewhere...
 * Unicode specifies 0x10ffff as maximum value, leaving us with 11 high bits to
 * be used as flags if need be; let's start using them from the highest ones to
 * be safer, though
 */

struct taikaevhdr {
    taikaevtime_t         tm;   // timestamp; 32- or 64-bit
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
    } data[C_VLA];
    //    taikaevuword_t        type; // event type such as KEYUP, FSCREAT
};

#define TAIKA_EV_MSG_MAX          (sizeof(taikaevword_t) + 8)
#define TAIKA_MAX_EV_SIZE         (offsetof(struct taikaev, msg) + TAIKA_EV_MSG_MAX)
/* event structure */
struct taikaev {
    struct taikaevhdr             hdr;
    union {
        /* actual event message */
        /*
         * TAIKA_KEY_PRESS, TAIKA_KEY_RELEASE,
         * TAIKA_BUTTON_PRESS, TAIKA_BUTTON_RELEASE,
         * TAIKA_POINTER_MOTION, TAIKA_BUTTON_MOTION
         */
        struct taikacoord         xyz;
        struct taikacmd           cmd;
    } data;
    struct taikamsg               msg[C_VLA];
};

#define taikachkev(evq, ev, flg)  taikagetev((evq), (ev), (flg) | TAIKA_CHECK_EV)
#define taikaeekev(evq, ev, flg)  taikagetev((evq), (ev), (flg) | TAIKA_PEEK_EV)
void    taikagetev(struct taikaev *evq, struct taikaev *ev, long flg);
long    taikaputev(struct taikaev *evq, struct taikaev *ev, long flg);
void    taikasyncev(struct taikaev *evq, long flg);
long    taikaskiphid(struct taikaev *evq, long len);

#undef RING_ITEM
#undef RING_INVAL
#undef RING_MALLOC

#if defined(__taika__)
#define RING_ITEM  struct taikaev
#define RING_INVAL (RING_ITEM){ 0 }
#define RING_MALLOC(sz) kmalloc(sz)
#else /* !defined(__taika__) */
#define RING_ITEM               struct taikaev
#define RING_INVAL              (RING_ITEM){ 0 }
#define RING_MALLOC(sz)         malloc(sz)
#endif /* defined(__taika__) */

#include <algo/ring.h>

#endif /* SYS_TAIKA_EV_H */

