# Voima Instruction Set Architecture

## ALU; Arithmetic-Logical Unit

### Instruction Format

- instruction size is 32 bits;
  - instructions are always 32-bit aligned, no need to pad the imm32-operand
  - the imm16 field is used to store instruction-flags and other instruction-
    related data

------------------------------------
| op | dst | src | imm16 | (imm32) |
------------------------------------

### ALU Instruction Set

#### Logical Functions; OP = 0

- 2 register operands

operation       | dst   | src   | imm16[8:11]
----------------|-------|-------|-----------
NOT             | 1-15  | N/A   | 0x00
OR              | 1-15  | 0-15  | 0x01
AND             | 1-15  | 0-15  | 0x02
XOR             | 1-15  | 0-15  | 0x03
NOR             | 1-15  | 0-15  | 0x04
XNOR            | 1-15  | 0-15  | 0x05
NAND            | 1-15  | 0-15  | 0x06

#### Shift and Rotate Operations; OP = 1

- destination register operand
- source register or immediate 5-or 6-bit count operand (64-bit)
- the C-bit (imm16[6]) denotes presence of shift count in imm16[0:5]

operation       | dst   | src   | imm16[0:5]    | imm16[6]      | imm16[8:11]
----------------|-------|-------|---------------|---------------|------------
SLL             | 1-15  | 1-15  | count (C)     | C             | 0x00 
SRL             | 1-15  | 1-15  | count (C)     | C             | 0x01
SAR             | 1-15  | 1-15  | count (C)     | C             | 0x02
ROL             | 1-15  | 1-15  | count (C)     | C             | 0x03
ROR             | 1-15  | 1-15  | count (C)     | C             | 0x04
RCL             | 1-15  | 1-15  | count (C)     | C             | 0x05
RCR             | 1-15  | 1-15  | count (C)     | C             | 0x06

#### Comparison Operation; OP = 2

- sets MSW-bits ZF, LT, CF, OF

operation       | dst   | src   | imm16
----------------|-------|-------|------
CMP             | 1-15  | 0-15  | N/A

#### Arithmetic Operations; OP = 3

- imm16[0] is the M-bit denoting MSW-flags (CF, OF) are set
- imm16[1] is the U-bit indicating an unsigned operation
- imm16[8:11] is the instruction ID

operation       | dst   | src   | imm16[0:1]    | imm16[8:11]
----------------|-------|-------|---------------|------------
INC             | 1-15  | 0-15  | MU            | 0x00
DEC             | 1-15  | 0-15  | MU            | 0x01
ADD             | 1-15  | 0-15  | MU            | 0x02
SUB             | 1-15  | 0-15  | MU            | 0x03
MUL             | 1-15  | 0-15  | MU            | 0x04
MHI             | 1-15  | 0-15  | MU            | 0x05
IRP             | 1-15  | 0-15  | MU            | 0x06
DIV             | 1-15  | 0-15  | MU            | 0x07
REM             | 1-15  | 0-15  | MU            | 0x08

#### Bit Operations; OP = 4

BSF             | 1-15  | 0-15  | 0x03          | N/A           | 0x03
BSR             | 1-15  | 0-15  | 0x04          | N/A           | 0x04
HAM
PAR
BEX
BDP

## MEM; Memory Unit

#### Address Calculations; OP = 5

- imm16[0:3] is an optional 3rd (offset) register operand
- imm16[4] is the R-bit denoting use of base address register (src)
- imm16[5] is the O-bit denoting use of offset register (imm16[0:3])
- imm16[6] is the A-bit denoting immediate address/offset (imm32)
- imm16[7] is the S-bit denoting offset register shift count in imm16[12:15]

operation       | dst   | src   | imm16[0:3]    | imm16[4:7]    | imm16[8:11]
----------------|-------|-------|---------------|---------------|-------------
LEA             | 1-15  | 0-15  | ofsreg        | ROAS          | 0x00

#### Bit Manipulation; OP = 6

- imm16[0:5] is an immediate bit position
- imm16[6] is the B-bit denoting bit position in imm16[0:5]
- imm16[7] is the A-bit denoting a 32-bit immediate address/offset (imm32)
- imm16[8:11] is the instruction ID
- imm16[12:15] is register ID to return the original memory-word (0 to ignore)

operation       | dst   | src   | imm16[0:5]    | imm16[6:7]    | imm16[8:11]
----------------|-------|-------|---------------|---------------|------------
BST             | 1-15  | 1-15  | position (B)  | BA            | 0x00
BCL             | 1-15  | 1-15  | position (B)  | BA            | 0x01
BFL             | 1-15  | 1-15  | position (B)  | BA            | 0x02
BTS             | 1-15  | 1-15  | position (B)  | BA            | 0x03
BTC             | 1-15  | 1-15  | position (B)  | BA            | 0x04
BTF             | 1-15  | 1-15  | position (B)  | BA            | 0x05

#### Load Instructions

##### Load Halfword; OP = 7

operation       | dst   | src   | imm16
----------------|-------|-------|------
0x07 - LDH      | 1-15  | N/A   | [0:15] is halfword to load

- imm16[0:3] is an optional 3rd (offset) register operand
- imm16[4] is the R-bit denoting use of base address register (src)
- imm16[5] is the O-bit denoting use of offset register (imm16[0:3])
- imm16[6] is the A-bit denoting immediate address/offset (imm32)
- imm16[7] is the S-bit denoting offset register shift count in imm16[12:15]
- imm16[8] is the I-bit denoting immediate operand following opcode
- imm16[9] is the B-bit denoting byte to load in imm16[0:7]
- imm16[10] is the L-bit denoting [aligned] 64-bit longword following opcode
- imm16[11] is the S-bit denoting the source is a system register
- imm16[12] is the D-bit denoting the destination is a system register
- imm16[13] is the L1-bit denoting use of the L1-cache
- imm16[14] is the L2-bit denoting use of the L1-cache

##### Other Load Instructions; OP = 8

operation       | dst   | src   | imm16[0:3]    | imm16[4:12]
----------------|-------|-------|---------------|------------
LDB             | 1-15  | N/A   | [0:7] is byte | B
LDI             | 1-15  | N/A   | N/A           | I
LDL             | 1-15  | N/A   | N/A           | L
LDR             | 1-15  | 1-15  | ofsreg        |
LDX             | 0-15  | 1-15  | ofsreg        |
LNT             | 1-15  | 1-15  | ofsreg        |

#### Store Instructions; OP = 2

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

