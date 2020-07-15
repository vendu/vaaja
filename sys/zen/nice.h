#ifndef __KERN_NICE_H__
#define __KERN_NICE_H__

#include <stdint.h>
#include <zen/sched/ule.h>

struct schednice {
    int8_t  nice;
    int8_t  slice;
};

/*
 * lookup table to convert nice values to priority offsets
 * as well as slices in ticks
 *
 * - nice is between -20 and 19 inclusive
 * - taskniceptr = &tasknicetab[ULE_NICE_HALF]
 * - prio += taskniceptr[nice];
 * - slice = tasksliceptr[nice];
 */
static struct schednice k_schednicetab[ULE_NICE_RANGE]
= {
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 },
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 },
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 },
    { -64,   1 }, { -60,   1 }, { -57,   2 }, { -54,   2 },
    { -51,   3 }, { -47,   3 }, { -44,   4 }, { -41,   4 },
    { -38,   5 }, { -35,   5 }, { -31,   6 }, { -28,   6 },
    { -25,   7 }, { -22,   7 }, { -19,   8 }, { -15,   8 },
    { -12,   9 }, {  -9,   9 }, {  -6,  10 }, {  -3,  10 },
    {   0,  11 }, {   3,  11 }, {   6,  12 }, {   9,  12 },
    {  13,  13 }, {  16,  13 }, {  19,  14 }, {  23,  14 },
    {  26,  15 }, {  29,  15 }, {  33,  16 }, {  36,  16 },
    {  39,  17 }, {  43,  17 }, {  46,  18 }, {  49,  18 },
    {  53,  19 }, {  56,  19 }, {  59,  20 }, {  63,  20 },
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 },
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 },
    {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }
   
};

#endif /* __KERN_NICE_H__ */

