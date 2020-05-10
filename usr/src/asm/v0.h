#ifndef __VALHALLA_VAS_V0_H__
#define __VALHALLA_VAS_V0_H__

#include <v0/vm/ins.h>

/* V0 instruction/opcode structure */
struct v0asmop {
    struct v0ins    ins;        // 16-bit instruction parcel
    struct v0insimm imm;        // optional 16-bit immediate operand
    union v0insarg  arg;        // optional 32-bit data parcel (aligned)
};

/*
 * v0.c
 * ----
 * asmgetop(op, str)            read instruction name + translate into op->ins
 * asmgetreg(op, str, id)       read register #0 or #1 + translate into op->ins
 * asmgetimm(op, str)           read immediate argument + translate into op->imm
 * asmgetadr(op, str, id)       read address + translate into op->imm, op->arg
 * asmxlateop(op, str)          translate operation
 */

void *
asmgetimm(struct v0op *op, const char *str)
{
    unsigned char *inp;
    size_t         ofs;
}

/* SUPPORT FOR V0 ARCHITECTURE */

/* segment flags */
#define V0_SEG_READ   (1U << 0) // read permission
#define V0_SEG_EXEC   (1U << 1) // execute permission
#define V0_SEG_WRITE  (1U << 2) // write permission
#define V0_SEG_USR    (1U << 3) // user segment (as opposed to system)
#define V0_SEG_ZERO   (1U << 4) // initiliaze to zero
#define V0_SEG_INIT   (1U << 5) // initialize from input
/* flags for process segments */
#define V0_SEG_TRAP   (V0_SEG_READ | V0_SEG_EXEC)
#define V0_SEG_RODATA V0_SEG_READ
#define V0_SEG_TEXT   (V0_SEG_READ | V0_SEG_EXEC) | V0_SEG_USR)
#define V0_SEG_DATA   (V0_SEG_READ | V0_SEG_WRITE | V0_SEG_USR | V0_SEG_INIT)
#define V0_SEG_BSS    (V0_SEG_DATA | V0_SEG_ZERO | V0_SEG_ALLOC)
#define V0_SEG_COMM   V0_SEG_RODATA
#define V0_SEG_DEBUG  V0_SEG_RODATA
#define V0_SEG_ALLOC  (1U << 6) // allocate output section
/* segment IDs */
#define V0_TRAP_SEG   0
#define V0_RODATA_SEG 1
#define V0_TEXT_SEG   2
#define V0_DATA_SEG   3
#define V0_BSS_SEG    4
#define V0_COMM_SEG   5
#define V0_DEBUG_SEG  6
#define V0_SEGS       8

#define V0_DEF_CORE   (8 * 1024 * 1024) // 8-megabyte default core size

#define __VALHALLA_VAS_V0_H__

