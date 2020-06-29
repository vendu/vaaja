#ifndef _KERN_SCHED_H_
#define _KERN_SCHED_H_

#include <kern/conf.h>
#include <zero/cdefs.h>
#include <zero/fastudiv.h>
#include <kern/proc/proc.h>
#include <kern/proc/task.h>
#include <kern/unit/ia32/task.h>

#if defined(_KERNEL_)
extern void schedinit(void);
#endif
extern void schedyield(void);

#if defined(ZEROSCHED)

extern void                     schedinitset(void);
extern FASTCALL NORETURN void   schedswitchtask(struct task *curtask);
extern void                     schedsetready(struct task *task);
extern void                     schedsetstopped(struct task *task);
extern void                     schedsetzombie(struct proc *proc);
extern void                     schedsetwait(struct task *task);
extern void                     schedsetsleep(struct task *task);

/* timeshare classes */
#define ULE_RESPONSIVE          3           // 'quick' timeshare tasks
#define ULE_NORMAL              4           // 'normal' timeshare tasks
#define ULE_BATCH               5           // batch tasks
#define ULE_NUSERCLASS          3           // number of user/timeshare classes
#define ULE_CLASSES             6           // # of user scheduler classes
#define ULE_IDLE                ULE_CLASSES // idle tasks
#define ULE_QUEUES              (ULE_CLASSES * ULE_CLASS_QUEUES)
#define ULE_ULE_TOTAL_QUEUES    (ULE_QUEUES + ULE_NIDLE) // ULE_IDLE too
#define ULE_TAB_QUEUES          512
#define ULE_NOCLASS             (-0x7f)

#if 0 /* FIXME: these will be handled in ULE_TRAP */
/* fixed priorities */
#define ULE_HID                 0   // human interface devices (kbd, mouse)
#define ULE_AUDIO               1   // audio synchronisation
#define ULE_VIDEO               2   // video synchronisation
#define ULE_INIT                3   // init; creation of new processes
#define ULE_FIXED_PRIO_MIN      0
#endif

#define schedclassminprio(c)                                            \
    ((c) * ULE_CLASS_PRIOS)
#define schedclassmaxprio(c)                                            \
    (schedclassminprio(c) + ULE_CLASS_PRIOS - 1)
#define schedclassminqueue(c)                                           \
    ((c) * ULE_CLASS_QUEUES)
/* priority organisation */
#define ULE_NIDLE               ULE_CLASS_QUEUES
#define ULE_SYS_PRIO_MIN        schedclassminprio(ULE_SYSTEM)
/* interrupt priority limits */
#define ULE_TRAP_PRIO_MIN       schedclassminprio(ULE_TRAP)
#define ULE_TRAP_PRIO_MAX       schedclassmaxprio(ULE_TRAP)
/* realtime priority limits */
#define ULE_RT_PRIO_MIN         schedclassminprio(ULE_REALTIME)
#define ULE_RT_PRIO_MAX         schedclassmaxprio(ULE_REALTIME)
/* timeshare priority limits */
#define ULE_USER_PRIO_MIN       schedclassminprio(ULE_RESPONSIVE)
/* positive nice values will not be mapped to ULE_IDLE */
#define ULE_USER_PRIO_MAX       (schedclassmaxprio(ULE_BATCH) - ULE_NICE_HALF)
#define ULE_USERRANGE           (ULE_USER_PRIO_MAX - ULE_USER_PRIO_MIN + 1)
/* batch priority limits */
#define ULE_BATCH_PRIO_MIN      schedclassminprio(ULE_BATCH)
#define ULE_BATCH_PRIO_MAX      schedclassmaxprio(ULE_BATCH)
#define ULE_BATCHRANGE          (ULE_BATCH_PRIO_MAX - ULE_BATCH_PRIO_MIN + 1)
/* idle priority limits */
#define ULE_IDLE_PRIO_MIN       (ULE_IDLE * ULE_CLASS_PRIO_S)
#define ULE_IDLE_PRIO_MAX       (ULE_IDLE_PRIO_MIN + ULE_CLASS_QUEUES - 1)
#define ULE_IDLERANGE           (ULE_IDLE_PRIO_MAX - ULE_IDLE_PRIO_MIN + 1)
/* nice limits */
#define ULE_NICE_MIN            (-(ULE_CLASS_QUEUES << 1))
#define ULE_NICE_MAX            ((ULE_CLASS_QUEUES << 1) - 1)
#define ULE_NICE_RANGE          (ULE_NICE_MAX - ULE_NICE_MIN + 1)
#define ULE_NICE_HALF           (ULE_NICE_RANGE >> 1)
/* highest and lowest priorities are reserved for nice */
/* we allow negative nice values to map to classes SCHEDREALTIME..SCHEDSYSTEM */
#define ULE_PRIO_MIN            ULE_USER_PRIO_MIN
#define ULE_PRIO_MAX            ULE_USER_PRIO_MAX
#define ULE_PRIO_RANGE          (ULE_PRIO_MAX - ULE_PRIO_MIN)
/* interactive priority limits */
#define ULE_INT_PRIO_MIN        ULE_USER_PRIO_MIN
//#define ULE_INT_PRIO_MAX     (ULE_BATCH_PRIO_MIN + ULE_BATCH_PRIO_MAX - 1)
#define ULE_INT_PRIO_MAX        ULE_USER_PRIO_MAX
#define ULE_INT_RANGE           (ULE_INT_PRIO_MAX - ULE_INT_PRIO_MIN + 1)

/* interactivity scoring */
/* interactivity scores are in the range [0, 128] */
#define ULE_SCORE_MAX           128
/* minimum score to mark thread as interactive */
#define ULE_SCORE_INT_LIM       32
/* half of maximum interactivity score */
#define ULE_SCORE_HALF          (ULE_SCOREMAX >> 1)
/* number of seconds to keep cpu stats around */
#define ULE_HISTORY_SECS        8
//#define ULE_HISTORY_SIZE    (ULE_HISTORY_MAX * (HZ << ULE_TICK_SHIFT))
/* number of ticks to keep cpu stats around */
#define ULE_HISTORY_TICKS       (ULE_HISTORY_SECS * kgethz())
/* maximum number of ticks before scaling back */
#define ULE_HISTORY_SIZE        (ULE_HISTORY_TICKS + kgethz())
//#define ULE_REC_TIME_DELTA       ((HZ << ULE_TICK_SHIFT) / HZ)
#define ULE_TICK__SHIFT         10
/* maximum number of sleep time + run time stored */
#define ULE_REC_TIME_MAX        ((kgethz() << 2) << ULE_TICK_SHIFT)
#define ULE_REC_TIME_FORKMAX    ((kgethz() << 1) << ULE_TICK_SHIFT)

#if defined(_KERNEL_)
#include <kern/bits/sched.h>
#endif

#endif /* defined(ZEROSCHED) */

#endif /* _KERN_SCHED_H_ */

