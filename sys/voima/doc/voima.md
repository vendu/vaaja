# preface

voima is a project to design and implement a new game console. depending of the configuration, the console has the following hardware and software parts.

# voima console

- v0 (custom verilog source) or risc-v (bonfire) microprocessor
- zen operating system
  - kernel
  - libc
    - iso c standard library with unix-like functionality
  - libm
    - iso c mathematics library
  - sdl
    - [software directmedia layer](http://libsdl.org/)
  - libdeck
    - terminal interface
    - keyboard
    - pointer; mouse, joystick, pad, tab
    - scan
    - print
    - plot
  - libaud
  - libgfx
    - obj
      - get, put, free
    - pix
    - ipc
    - rop
    - conf
    - gfx; init, setup, run
    - draw; pnt, line, tri, rect, circ, ell, poly
    - img
    - color
    - sprite
    - fx
    - xform
    - shader

# voima instruction set

## instruction formats

- the fld-row lists field-names
- the w-row lists field-widths in bits

### def-format; default instruction format

- the unit-field identifies processor subunit or external coprocessor (0x1f)
- dst is destination register
- src1 is primary source register
- src2 is secondary source register or 0 if not applicable
- inst is 7-bit instruction ID; bit #7 may be used to indicate folding of a
  shift or rotate instruction listed in src2 a'la
  add   rd, rs1, rs1, sll $3    ; r0 = r1 + (r1 << 3) i.e. r1 * 9
- parm has
  #1 - shift and rotate instructions
    - shift/rotate count
  # conditional branch instructions
  #2 - load-store instructions
     - parm[0] indicates presence of immediate operand
     - immediate operand size in bits [2:1]; immsize = 8 << parm[2:1];
     - address-mode flags in bits [4:3]
       - BASE   - base register in use (parm[3])
       - NDX    - index register in use (parm[4])
  #3
    -

def | 0..4 | 5..8 | 9..12 | 13..16 | 17..23 | 24..31 |
----|------|------|-------|--------|--------|--------|
fld | unit | dst  | src1  | src2   | inst   | parm   |
----|------|------|-------|--------|--------|--------|
w   |  5   |  4   |   4   |   4    |   7    |   8    |

### jmp-format; unconditional branches

- processor subunit (0x08)
- 2-bit instruction ID (jmi, jmr, jal, jpc)
- signed 25-bit offset

jmp | 0..4 | 5..6 | 7..31 |
----|------|------|-------|
fld | unit | inst | ofs   |
----|------|------|-------|

#### logical instructions

- flg == 1 => atomic
  - NOT     %r1, %r2            ; r1 = ~r2;
  - LOR     %r1, %r2, %r3       ; r1 = r2 | r3;
  - XOR     %r1, %r2, %r3       ; r1 = r2 ^ r3;
  - AND     %r1, %r2, %r3       ; r1 = r2 & r3;
  - NOR     %r1, %r2, %r3       ; r1 = r2 ~| r3;
  - XNOR    %r1, %r2, %r3       ; r1 = r2 ^~ r3;
  - NAND    %r1, %r2, %r3       ; r1 = r2 ~& r3;

#### shift and rotate instructions

- flg == 1 => shift right
  - SHL     %r1, %r2, %r3|$i    ; r1 = r2 << %r3|$i;
  - SHR     %r1, %r2, %r3|$i    ; r1 = r2 >> %r3|$i;
  - SAR     %r1, %r2, %r3|$i    ; r1 = r2 >>> %r3|$i;
  - ROL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i;
  - ROR     %r1, %r2, %r3|$i    ; r1 = r2 -> %r3|$i;
  - RCL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);
  - RCR     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);

#### arithmetic instructions
- INC
- UINC
- XINC
- DEC,
- UDEC,
- XDEC              - flg == 1 => atomic
- ADD, UADD, ADC, XADD, SUB, USUB, SBB, XSUB    - flg == 1 => atomic
- MUL, UMUL, MHI, UMHI                          - flg == 1 => return high word
- DIV, UDIV, RDIV, URDIV                        - flg == 1 => compute remainder
- REM, UREM, IRP, UIRP
- BST, BCL, BFL
- BTS, BTC, BTF                                 - flg == 1 => atomic
- BFS, BLS, PAR, HAM
- CTZ, CLZ, HSH, HUN

- ZEX, SEX                                      - flg == 1 => sign-extend

 rd, rs1, rs1, sll $3    ; r0 = r1 + (r1 << 3) i.e. r1 * 9
- parm has
  #1 - shift and rotate instructions
    - shift/rotate count
  # conditional branch instructions
  #2 - load-store instructions
     - reg[24] indicates presence of immediate operand
     - immediate operand size in bits [26:25]; immsize = 8 << reg[26:25];
     - instruction size in bits [28:27]; instsize = 32 + 8 << reg[28:27];
     - address-mode flags in bits [31:29]
       - BASE   - base register in use (reg[29])
       - NDX    - index register in use (reg[30])
       - OFS    - immediate [32-bit] offset in use (reg[31])
  #3
    -

def | 0..4 | 5..8 | 9..12 | 13..16 | 17..23 | 24..31 |
----|------|------|-------|--------|--------|--------|
fld | unit | src1 | dst   | src2   | inst   | parm   |
----|------|------|-------|--------|--------|--------|
w   |  5   |  4   |   4   |   4    |   7    |   8    |

### jmp-format; unconditional branches

- signed 27-bit offset
  - pc += 4;
  - sign = ofs & 0x04000000; ofs <<= 2; sign >>= 26; ofs |= 0xe0000000 * sign;
  - pc += ofs;

jmp | 0..4 | 5..31 |
----|------|-------|
fld | unit | ofs   |
----|------|-------|

#### logical instructions

- flg == 1 => atomic
  - NOT     %r1, %r2            ; r1 = ~r2;
  - LOR     %r1, %r2, %r3       ; r1 = r2 | r3;
  - XOR     %r1, %r2, %r3       ; r1 = r2 ^ r3;
  - AND     %r1, %r2, %r3       ; r1 = r2 & r3;
  - NOR     %r1, %r2, %r3       ; r1 = r2 ~| r3;
  - XNOR    %r1, %r2, %r3       ; r1 = r2 ^~ r3;
  - NAND    %r1, %r2, %r3       ; r1 = r2 ~& r3;

#### shift and rotate instructions

- flg == 1 => shift right
  - SHL     %r1, %r2, %r3|$i    ; r1 = r2 << %r3|$i;
  - SHR     %r1, %r2, %r3|$i    ; r1 = r2 >> %r3|$i;
  - SAR     %r1, %r2, %r3|$i    ; r1 = r2 >>> %r3|$i;
  - ROL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i;
  - ROR     %r1, %r2, %r3|$i    ; r1 = r2 -> %r3|$i;
  - RCL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);
  - RCR     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);

#### arithmetic instructions
- INC
- UINC
- XINC
- DEC,
- UDEC,
- XDEC              - flg == 1 => atomic
- ADD, UADD, ADC, XADD, SUB, USUB, SBB, XSUB    - flg == 1 => atomic
- MUL, UMUL, MHI, UMHI                          - flg == 1 => return high word
- DIV, UDIV, RDIV, URDIV                        - flg == 1 => compute remainder
- REM, UREM, IRP, UIRP
- BST, BCL, BFL
- BTS, BTC, BTF                                 - flg == 1 => atomic
- BFS, BLS, PAR, HAM
- CTZ, CLZ, HSH, HUN

- ZEX, SEX                                      - flg == 1 => sign-extend

 rd, rs1, rs1, sll $3    ; r0 = r1 + (r1 << 3) i.e. r1 * 9
- parm has
  #1 - shift and rotate instructions
    - shift/rotate count
  # conditional branch instructions
  #2 - load-store instructions
     - reg[24] indicates presence of immediate operand
     - immediate operand size in bits [26:25]; immsize = 8 << reg[26:25];
     - instruction size in bits [28:27]; instsize = 32 + 8 << reg[28:27];
     - address-mode flags in bits [31:29]
       - BASE   - base register in use (reg[29])
       - NDX    - index register in use (reg[30])
       - OFS    - immediate [32-bit] offset in use (reg[31])
  #3
    -

def | 0..4 | 5..8 | 9..12 | 13..16 | 17..23 | 24..31 |
----|------|------|-------|--------|--------|--------|
fld | unit | dst  | src1  | src2   | inst   | parm   |
----|------|------|-------|--------|--------|--------|
w   |  5   |  4   |   4   |   4    |   7    |   8    |

### jmp-format; unconditional branches

- signed 27-bit offset
  - sex

jmp | 0..4 | 5..31 |
----|------|-------|
fld | unit | ofs   |
----|------|-------|

#### logical instructions

- flg == 1 => atomic
  - NOT     %r1, %r2            ; r1 = ~r2;
  - LOR     %r1, %r2, %r3       ; r1 = r2 | r3;
  - XOR     %r1, %r2, %r3       ; r1 = r2 ^ r3;
  - AND     %r1, %r2, %r3       ; r1 = r2 & r3;
  - NOR     %r1, %r2, %r3       ; r1 = r2 ~| r3;
  - XNOR    %r1, %r2, %r3       ; r1 = r2 ^~ r3;
  - NAND    %r1, %r2, %r3       ; r1 = r2 ~& r3;

#### shift and rotate instructions

- flg == 1 => shift right
  - SHL     %r1, %r2, %r3|$i    ; r1 = r2 << %r3|$i;
  - SHR     %r1, %r2, %r3|$i    ; r1 = r2 >> %r3|$i;
  - SAR     %r1, %r2, %r3|$i    ; r1 = r2 >>> %r3|$i;
  - ROL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i;
  - ROR     %r1, %r2, %r3|$i    ; r1 = r2 -> %r3|$i;
  - RCL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);
  - RCR     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);

#### arithmetic instructions
- INC
- UINC
- XINC
- DEC,
- UDEC,
- XDEC              - flg == 1 => atomic
- ADD, UADD, ADC, XADD, SUB, USUB, SBB, XSUB    - flg == 1 => atomic
- MUL, UMUL, MHI, UMHI                          - flg == 1 => return high word
- DIV, UDIV, RDIV, URDIV                        - flg == 1 => compute remainder
- REM, UREM, IRP, UIRP
- BST, BCL, BFL
- BTS, BTC, BTF                                 - flg == 1 => atomic
- BFS, BLS, PAR, HAM
- CTZ, CLZ, HSH, HUN

- ZEX, SEX                                      - flg == 1 => sign-extend

