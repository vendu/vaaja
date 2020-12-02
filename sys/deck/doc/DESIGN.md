# Deck Soft Processor Design

The plan is to implement the hardware (read: FPGA) parts of the project in the
Verilog Hardware Description Language (HDL).

## The Deck [Verilog] Source Tree

    (Deck RTL)[https://github.com/vendu/vaaja/blob/master/sys/deck/rtl]

## Verilog Files/Modules

### conf.vh
- definitions common to all modules

### proc/
- processor module directory

#### proc/deck.v
- standard processor module

#### proc/ext.v
- optional extensions to main.v functionality

### mem/
- memory interface directory

#### mem/mem.vh
- common definitions such as sizes of different types of installed memory

#### mem/sdram.v
- SDRAM driver

#### mem/mmu.v
- memory management unit [for virtual memory support]

#### mem/nvram.v
- non-volatile ('flash') memory

