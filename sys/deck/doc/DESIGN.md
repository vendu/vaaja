# Deck Soft Processor Design

The plan is to implement the hardware (read: FPGA) parts of the project in the
Verilog Hardware Description Language (HDL).

# Interrupts

The lowest priority interrupt are handled first.

## Interrupt Priorities

0       - Interrupt Timer; used to provide a programmable interrupt timer for
          scheduling operating system tasks.

1-7     - hardware or software triggered interrupts such as math errors. The INT
          instruction's sole purpose of existence is to provide an interface to
          system support software before the machine is fully-initialized for
          multiprocessor execution with more efficcient device interfaces.

8       - AC97 Audio Controller; this takes priority even over video and human-
          interface device synchronization, for audible clicks and noise are the
          easiest to spot. The device itself transforms samples over the bus
          using DMA-transfers, so this interrupt's purpose is mostly to keep the
          sample/stream buffers full enough at all times.

9       - VGA Graphics Adapter;

## Memory

### Memory Hierarchies

#### Register File

The register file relies in LUT RAM. It is read and written directly,
bypassing all hierarchies in the memory datapaths; caches as well as SDRAM.

Keep in mind that the Integer-Registers are internally 64-bit wide to allow for
things such as extracting the high 32-bit word of multiplication.

#### Code and Data Caches

The L1-caches fetch instructions and data from SDRAM into Block RAM. The Papilio
Pro hs 16 18Kbit Block RAM units, enough to facilitate 32 KB instruction and
data caches with parity checking enabled.

## I/O Devices

### SPI Flash Controller

The plan is to use SPI Flash memory to boot the operating system fast and
reliably.

### AC97 Audio Controller

The Papilio Pro hosts 16/18/20-bit audio output of up to 48 KHz sample rate.

### VGA Graphics Adapter

Our VGA-controller shall support resolutions like 640x480 (4:3) or 800x45
(16:9) with 4,096 distrinct colors. The VGA-framebuffer will be located in the
highest part of SDRAM (right below 8 megabytes on the Papilio Pro).

### PS/2-Protocol Keyboard and Mouse

PC-style keyboard and mouse are supported natively by the Papilio Pro.

# The Deck [Verilog] Source Tree

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

