# Deck Source Code Roadmap

## Toplevel Directory

### conf.h
- compile-time options

### deck.h
- main header file included from other headers and source files

### fxp.h
- fixed point coprocessor extension

### fpu.h
- floating point coprocessor extension

### inst.h
- instruction set architecture definition

### intr.h
- interrupts, exceptions, traps, aborts, faults - a beloved child has many names

### io.h
- input/output interface

### mmu.h
- memory management unit

### mp.h
- multiprocessor extension

### regs.h
- register file definition

### sanity.h
- optional compile-time sanity checks

### types.h
- system types

### vm.h
- deck virtual machine

## bsp/
- board/hardware support packages

## fpga/
- FPGA-kit/chip support files

## rtl/
- deck verilog implementation

