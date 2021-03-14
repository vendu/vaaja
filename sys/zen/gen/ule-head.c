/* slightly revamped FreeBSD ULE-scheduler */
#include <limits.h>
#include <stdint.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <mach/asm.h>
#include <mt/tktlk.h>
#include <zen/fastudiv.h>
#include <sys/zen/util.h>
#include <sys/zen/mem.h>
#include <sys/zen/sched/ule.h>
#include <sys/zen/sched/tao.h>
#include <sys/zen/sched/task.h>

/*
 * lookup table to convert nice values to priority offsets
 * as well as slices in ticks
 *
 * - nice is between -20 and 19 inclusive
 * - taskniceptr = &tasknicetab[SCHED_ULE_NICE_HALF]
 * - prio += taskniceptr[nice];
 * - slice = tasksliceptr[nice];
 */

