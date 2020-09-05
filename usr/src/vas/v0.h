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

#define V0_DEF_CORE   (8 * 1024 * 1024) // 8-megabyte default core size

#define __VALHALLA_VAS_V0_H__

