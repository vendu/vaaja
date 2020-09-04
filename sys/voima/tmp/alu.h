#ifndef __VOIMA_ALU_H__
#define __VOIMA_ALU_H__

/* imm16-member fields */
#define ALU_PAD_MASK            0xffff      // all 1-bits for 32-bit alignment
#define ALU_IMM32_BIT           (1 << 15)   // imm32-field present
#define ALU_CNT_BIT             (1 << 14)   // 5-bit byte-count in imm16[8:4]
#define ALU_LEFT_BIT            (1 << 13)   // shift or rotate right
#define ALU_ROT_BIT             (1 << 12)   // shift-instructions with rotate
#define ALU_REG3_BIT            (1 << 11)   // 4-bit register ID in imm16[3:0]
#define ALU_IMM8_BIT            (1 << 10)   // 8-bit immediate in imm16[7:0]
#define ALU_CNT_BITS            5
#define ALU_CNT_SHIFT           4
#define ALU_CNT_MASK            ((1 << ALU_CNT_BITS) - 1)
#define ALU_REG_MASK            0x000f

#define VOIMA_NOT   			0x00        // logical NOT
// r2 = ~ri1;
#define VOIMA_NEG   			0x01        // arithmetic negation
// r2 = -ri1;
#define VOIMA_OR    			0x02        // logical OR
// r2 |= ri1;
#define VOIMA_AND   			0x03        // logical AND
// r2 &= ri1;
#define VOIMA_XOR   			0x04        // logical XOR
// r2 ^= ri1;
#define VOIMA_NOR       		0x05        // logical NOR
#define VOIMA_XNOR      		0x06        // logical XNOR
#define VOIMA_NAND      		0x07        // logical NAND
#define VOIMA_SRL   			0x08        // logical right-shift
// r3 = r2 << ri1;
#define VOIMA_SRL   			0x08        // logical left-shift
// r3 = r2 >> ri1;
#define VOIMA_SAR   			0x08        // arithmetic right-shift
// r3 = r2 >>> ri1;
#define VOIMA_ADD   			0x09        // addition
// r2 += ri1;
#define VOIMA_SUB   			0x0a        // subtraction
// r2 -= ri1;
#define VOIMA_MUL   			0x0b        // multiplication
// r2 *= ri1;
#define VOIMA_MHI   			0x0c        // multiplication, return high word
// r2 = (r2 * ri1) >> 32;
#define VOIMA_IRP   			0x0d        // cache inverse reciprocal (replace DIV with MUL)
// cirp(ri1);
#define VOIMA_DIV   			0x0e        // integer division
// r2 /= ri1;
#define VOIMA_REM   			0x0f        // integer division
// r2 %= ri1;
#define VOIMA_CMP       		0x10        // set flags for comparison
// ((r2 == r1) ? MSW |= ZF, (r1 < r2) ? MSW |= LT
#define VOIMA_BST       		0x11        // set bit
// r2 |= (1 << ri1);
#define VOIMA_BCL       		0x12        // clear bit
// r2 &= ~(1 << ri1);
#define VOIMA_BFL       		0x13        // flip bit
// r2 ^= (1 << ri1);
#define VOIMA_BSF       		0x14        // bit-scan forward
// r2 |= ctz(ri1);
#define VOIMA_BSR       		0x15        // bit-scan reverse
// r2 |= clz(ri1);
#define VOIMA_HAM       		0x16        // bit-population (# of 1-bits)
// r2 = ham(ri1);
#define VOIMA_PAR       		0x17        // byte-parity; 0 for even parity (#of of 1-bits)
#define VOIMA_BTS       		0x18        // bit test and set iff clear
// r3 = r2 & (1 << ri1); if (!r3) { r2 |= (1 << ri1) }
#define VOIMA_BTC       		0x19        // bit test and clear iff set
// r3 = r2 & (1 << ri1); if (r3) { r2 &= ~(1 << ri1) }
#define VOIMA_BTF       		0x1a        // bit test and flip
// r3 = r2 & (1 << ri1); r ^= (1 << ri1);
#define VOIMA_BEX       		0x1b        // bit extract
// BEX(0x12345678, 0xff00fff0) -> 0x00012567
#define VOIMA_BDP       		0x1c        // bit deposit
// BEX(0x00012567, 0xff00fff0) -> 0x12005670

#endif /* __VOIMA_ALU_H__ */

