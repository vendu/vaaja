#Ifndef __ZEN_ULE_H__
#define __ZEN_ULE_H__

#include <zen/conf.h>

#if defined(ZEN_ULE_THR_SCHED)

#if defined(__KERNEL__)
extern void uleinit(void);
#endif
extern void schedyield(void);

#if defined(ZEROULE)

extern void                   uleinitset(void);
extern FASTCALL NORETURN void uleswitchtask(struct zentask *curtask);
extern void                   ulesetready(struct zentask *task);
extern void                   ulesetstopped(struct zentask *task);
extern void                   ulesetzombie(struct zenproc *proc);
extern void                   ulesetwait(struct zentask *task);
extern void                   ulesetsleep(struct zentask *task);

/*
 * in default configuration, we have
 *
 * ULE_CLASS_PRIOS    64
 * ULE_CLASS_QUEUES   ULE_CLASS_PRIOS / 4
/* task scheduler classes */
#define ULE_CLASS_PRIOS       64      // # of priorities per class
#define ULE_CLASS_QUEUES      (ULE_CLASS_PRIO >> ULE_QUEUE_SHIFT)
#define ULE_QUEUE_SHIFT       2
#define ULE_QUEUE_PRIOS       (1 << ULE_QUEUE_SHIFT)
/* 'system' classes */
#define ULE_DEADLINE          (-ULE_REALTIME - 1) // deadline tasks
#define ULE_REALTIME_RR       (-ULE_REALTIME) //
#define ULE_TRAP              0       // interrupt tasks
#define ULE_REALTIME          1       // realtime threads
#define ULE_SYSTEM            2
#define ULE_SYS_CLASSES       3       // # of system scheduler classes
/* timeshare classes */
#define ULE_RESPONSIVE        3       // 'quick' timeshare tasks
#define ULE_NORMAL            4       // 'normal' timeshare tasks
#define ULE_BATCH             5       // batch tasks
#define ULE_USER_CLASSES      3       // number of user ('timeshare') classes
#define ULE_CLASSES           6       // # of user scheduler classes
#define ULE_IDLE              ULE_CLASSES // idle tasks
#define ULE_QUEUES            (ULE_CLASSES * ULE_CLASS_QUEUES)
#define ULE_TABLE_QUEUE       512
#define ULE_NOO_CLASS         (-0x7f)

#if 0 /* FIXME: these will be handled in ULE_TRAP */
/* fixed priorities */
#define ULE_HID               0           // human interface devices (kbd, mouse)
#define ULE_AUDIO             1           // audio synchronisation
#define ULE_VIDEO             2           // video synchronisation
#define ULE_INIT              3           // init; creation of new processes
#define ULE_FIXED_PRIO_MIN    0
#endif

#define uleclassminprio(c)                                              \
    ((c) * ULE_CLASS_PRIOS)
#define uleclassmaxprio(c)                                              \
    (uleclassminprio(c) + ULE_CLASS_PRIO - 1)
/* priority organisation */
#define ULE_IDLE               ULE_CLASS_QUEUE
#define ULE_SYS_PRIO_MIN       uleclassminprio(ULE_SYSTEM)
/* interrupt priority limits */
#define ULE_TRAP_PRIO_MIN      uleclassminprio(ULE_TRAP)
#define ULE_TRAP_PRIO_MAX      uleclassmaxprio(ULE_TRAP)
/* realtime priority limits */
#define ULE_RT_PRIO_MIN        uleclassminprio(ULE_REALTIME)
#define ULE_RT_PRIO_MAX        uleclassmaxprio(ULE_REALTIME)
/* timeshare priority limits */
#define ULE_USER_PRIO_MIN      uleclassminprio(ULE_RESPONSIVE)
/* positive nice values will not be mapped to ULE_IDLE */
#define ULE_USER_PRIO_MAX      (uleclassmaxprio(ULE_BATCH) - ULE_ICEHALF)
#define ULE_USER_RANGE         (ULE_USER_PRIO_MAX - ULE_USER_PRIO_MIN + 1)
/* batch priority limits */
#define ULE_BATCH_PRIO_MIN     uleclassminprio(ULE_BATCH)
#define ULE_BATCH_PRIO_MAX     uleclassmaxprio(ULE_BATCH)
#define ULE_BATCH_RANGE        (ULE_BATCH_PRIO_MAX - ULE_BATCH_PRIO_MIN + 1)
/* idle priority limits */
#define ULE_IDLE_PRIO_MIN      (ULE_IDLE * ULE_CLASS_PRIO)
#define ULE_IDLE_PRIO_MAX      (ULE_IDLE_PRIO_MIN + ULE_CLASS_QUEUE - 1)
#define ULE_IDLE_RANGE         (ULE_IDLE_PRIO_MAX - ULE_IDLE_PRIO_MIN + 1)
/* nice limits */
#define ULE_ICE_MIN            (-(ULE_CLASS_QUEUEs << 1))
#define ULE_ICE_MAX            ((ULE_CLASS_QUEUES << 1) - 1)
#define ULE_ICE_RANGE          (ULE_ICE_MAX - ULE_ICE_MIN + 1)
#define ULE_ICE_HALF           (ULE_ICE_RANGE >> 1)
/* highest and lowest priorities are reserved for nice */
/* we allow negative nice values to map to classes ULE_REALTIME..ULE_SYSTEM */
#define ULE__PRIO_MIN          ULE_USER_PRIO_MIN
#define ULE__PRIO_MAX          ULE_USER_PRIO_MAX
#define ULE_PRIO_RANGE         (ULE__PRIO_MAX - ULE__PRIO_MIN)
/* interactive priority limits */
#define ULE_INT_PRIO_MIN       ULE_USER_PRIO_MIN
//#define ULE_INT_PRIO_MAX     (ULE_BATCH_PRIO_MIN + ULE_BATCH_PRIO_MAX - 1)
#define ULE_INT_PRIO_MAX       ULE_USERPRIOMAX
#define ULE_INT_RANGE          (ULE_INT_PRIO_MAX - ULE_INT_PRIO_MIN + 1)

/* interactivity scoring */
/* interactivity scores are in the range [0, 128] */
#define ULE_SCORE_MAX           128
/* minimum score to mark thread as interactive */
#define ULE_SCORE_INT_LIM       32
/* half of maximum interactivity score */
#define ULE_SCORE_HALF          (ULE_SCORE_MAX >> 1)
/* number of seconds to keep cpu stats around */
#define ULE_HISTORY_SECS        8
//#define ULE_HISTORY_SIZE    (ULE_HISTORYMAX * (HZ << ULE_TICK_SHIFT))
/* number of ticks to keep cpu stats around */
#define ULE_HISTORY_TICKS       (ULE_HISTORY_SECS * kgethz())
/* maximum number of ticks before scaling back */
#define ULE_HISTORY_SIZE        (ULE_HISTORY_TICKS + kgethz())
//#define ULE_RECTIMEINCR       ((HZ << ULE_TICK_SHIFT) / HZ)
#define ULE_TICK_SHIFT          10
/* maximum number of sleep time + run time stored */
#define ULE_REC_TIME_MAX        ((kgethz() << 2) << ULE_TICK_SHIFT)
#define ULE_REC_TIME_FORK_MAX   ((kgethz() << 1) << ULE_TICK_SHIFT)

#endif /* defined(ZEROULE_) */

#endif /* __ZEN_ULE_H__ */

