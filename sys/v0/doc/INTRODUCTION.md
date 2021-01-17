# Introduction to Deck Soft-Processor

The goal of the Deck project is to implement a [relatively] simple, small,
understandable microprocessor to deploy on FPGA-boards and, who knows, later on
other system chips.

The project has been inspired by the ARM-family of processors (and some others),
which shows in the way we deal with conditional execution as well as folding
shift instructions to modify pretty much any instruction's result values.

## Technical Specification

This information holds for the first version I'm planning to implement using a
Gadget Factory Papilio Pro FPGA-kit.

### System Hardware

Processor:      Deck v0; 32-bit RISC-like processor with some extra goodies
Memory:         8 MB SDRAM, 8 MB SPI Flash, 512 KB BRAM
Graphics:       up to 800x450 or 768x432 (16:9) or 640x480 (4:3) pixels
                (TODO: CHECK PAPILIO SPECS)
Audio:          44.1 KHz 16-bit DAC
User Input:     PS/2-keyboard, PS/2-mouse, 2 x digital joysticks

### System Software

Kernel:         Zen - see elsewhere in this Vaaja repository
Libraries:      ISO C99 + parts of ISO C11 (threads?), SDL2
Development:    Portable virtual machine + debugger to run on [at least] Unix-
                like systems

