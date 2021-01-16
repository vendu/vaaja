# Vaaja Programmer's Calculator

- support 8-bit, 16-bit, 32-bit and 64-bit integer presentations of numbers
- support IEEE 754 32-bit single and 64-bit double precision floating-point
- use stack as the user interface, targeting operations on the top one or two
  variables/values

# Supported Integer Operations

- +
- -
- *
- /
- %
- NOT
- OR
- AND
- XOR
- NOR
- XNOR
- NAND
- <<
- >>
- >>>
- ROL
- ROR
- RCL
- RCR

# Supported Floating-Point Operations

- +
- -
- *
- /
- SQRT
- SIN
- COS
- TAN
- ATAN
- EXP
- LOG
- LN
- FCMP
- FLDI
- FSTI

# Implementation Details

## Code Modules

- stk; stack implementation for RPN (Reverse Polish Notation) user interface
- iop; integer operations
- fop; floating-point operations (IEEE 754)
- fxp; fixed-point operations (Q3.29, Q24.24)
- fmt; numeric conversions between formats
- vtd; virtual tape drive

