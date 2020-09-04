# Voima Instruction Set Architecture

## Design Goals

### Instruction Types

#### Basic Instructions

##### Logical Instructions

- no flag bits

- NOT       - invert all bits
- OR        - logical OR
- AND       - logical AND
- XOR       - logical XOR
- NOR       - logical NOR
- XNOR      - logical XNOR
- NAND      - logical NAND

##### Shift and Rotate Instructions

- 3 flags; direction, set-carry, arithmetic shift

- SLL       - logical left shift
- SRL       - logical right shift
- SAR       - arithmetic (fill with sign) right shift
- ROL       - rotate left
- ROR       - rotate right
- RCL       - rotate left with carry
- RCR       - rotate right with carry

##### Arithmetic Instructions

- 3 flags; unsigned, set-MSW, return high result word

- NEG       - arithmetic negation
- INC       - increment by one
- DEC       - decrement by one
- ADD       - addition
- SUB       - subtraction
- MUL       - multiplication
- MHI       - multiplication, return high [32-bit] result word
- IRP       - inverse reciprocal to replace division with multiplication
- DIV       - division
- REM       - remainder

##### Bit Manipulation

- flags; bus-lock, return original memory value

- BST       - set bit
- BCL       - clear bit
- BFL       - flip/toggle bit
- BSF       - bit scan forward
- BSR       - bit scan reverse

- BTS       - bit test-and-set
- BTC       - bit test-and-clear
- BTF       - bit test-and-flip

##### Bit Operations

- PAR       - compute parity
- HAM       - count 1-bits (bit-population/Hamming Weight)
- CTZ       - count trailing (low) zero-bits
- CLZ       - count leading (high) zero-bits
- HSH       - hash a value
- HUN       - reverse-hash a value

##### System Control Operations

- NOP       - no operation
- FTR       - check machine feature support
- HLT       - halt/wait for interrupt
- RST       - reset
- INT       - raise interrupt
- CPF       - prefetch cacheline
- RBR       - read barrier
- WBR       - write barrier
- BAR       - full memory-barrier
- WFE       - wait for event
- SEV       - signal event
- IPG       - invalidate page lookup-entry
- SBL       - set low-bound for range
- SBH       - set high-bound for range

##### Control Flow Operations

- CMP
- JMP
- JMR
- BRA
- CSR
- RET
- SME
- SMX
- RTI
- THR
- THX

