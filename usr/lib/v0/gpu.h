#ifndef __V0_GPU_H__
#define __V0_GPU_H__

#include <stdint.h>
#include <v0/v0.h>
#include <gfx/rgb.h>

typedef gfxpix444               v0pix;

#define V0_GPU_REGISTERS        16

#if (V0_MACHINE_TYPE == V0_PAPILIO_PRO)
#define V0_GPU_PIX_FORMAT   V0_GPU_444_PIX
#endif

#define V0_GPU_8_PIX            0x00
#define V0_GPU_444_PIX          0x01
#define V0_GPU_555_PIX          0x02
#define V0_GPU_565_PIX          0x03
#define V0_GPU_888_PIX          0x04
#define V0_GPU_ARGB32_PIX       0x05

#define V0_GPU_NOP_OP           0x00    // (void)0
#define V0_GPU_COPY_OP          0x01    // dest = src
#define V0_GPU_COPY_INV_OP      0x02    // dest = ~src
#define V0_GPU_CLEAR_OP         0x03    // dest = 0
#define V0_GPU_SET_OP           0x04    // dest = ~0
#define V0_GPU_INV_OP           0x05    // dest = ~dest
#define V0_GPU_AND_OP           0x06    // dest = src & dest
#define V0_GPU_AND_REV_OP       0x07    // dest = src & (~dest)
#define V0_GPU_AND_INV_OP       0x08    // dest = (~src) & dest
#define V0_GPU_OR_OP            0x09    // dest = src | dest
#define V0_GPU_OR_INV_OP        0x10    // dest = (~src) | dest
#define V0_GPU_OR_REV_OP        0x11    // dest = src | (~dest)
#define V0_GPU_XOR_OP           0x12    // dest = src ^ dest
#define V0_GPU_NOR_OP           0x13    // dest = (~src) & (~dest)
#define V0_GPU_EQ_OP            0x14    // dest = (~src) ^ dest
#define V0_GPU_NAND_OP          0x15    // dest = ~(src & dest)

struct v0gpu {
    v0flt                        regs[V0_GPU_REGISTERS];
    v0flt                        w;
    v0flt                        h;
    v0pix                       *fb;
};

#endif /* __V0_GPU_H__ */

