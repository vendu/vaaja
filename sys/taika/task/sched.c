/*
 * Hybrid Lottery Scheduler as described
 * REFERENCE http://www.usenix.org/events/usenix99/full_papers/petrou/petrou.pdf
 */

#define SCHED_HLS_TICK_MSECS    4
#define SCHED_HLS_QUANTUM_MSECS 64
#define SCHED_HLS_WINDOW_SECS   8
#define SCHED_HLS_TICKS_PER_SEC (1000 / SCHED_HLS_TICK_SECS)
#define SCHED_HLS_HISTORY_TICKS (SCHED_HLS_TICKS_PER_SEC * SCHED_HLS_WINDOW_SECS)

