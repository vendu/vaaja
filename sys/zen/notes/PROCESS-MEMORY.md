# Zen Process Layout

# Process Memory Segments

These segments are not to be mixed with processor memory segmentation used on
some systems, even though the implemmentations clearly overlap. The text below
present the memory map of a basic user segemnt; we will also discuss what's
different for system-mode segments; programs such as kernels are likely to use
custom segments. We also won't explain all debugging and other extra segments
you may see in custom linker scripts as that would be beyond the scope of this
document.

# The Segment Dsecriptor and Flags

In addition to the segment page address, segment descriptors use the following
bits to control memory management

|    Segment    |    Brief
|---------------|-----------------------------
| EXEC          | segment data may be executed
|---------------|

# NULL Segment

The NULL segment can be considered a sort of dummy; its address as well as
all segment flags have the value of 0 (all zero-bits). In this scenario, the
NULL-segment is used to capture NULL-pointer dereferences, making the kernel
send the SIGSEGV-signal to the user process.

# TEXT Segment

The TEXT segment contains executable process code. It is filled at compile-
time.

# RODATA Segment

The RODATA segment contains read-only data. This segment is initialized at
compile-time.

# DATA Segment

The data segment contains initialized data written at compile-time.

# BSS Segment

The name of the BSS has long roots; orioginally, it stood for 'Block Started
by Symbol'. Its contents are allocated and initialized to zero at run time.

# COMM Segment

The COMM Segment

# DEBUG Segment

The debug section is used by the linker to store its extra data.

# HEAP Segment

The heap segment is one from which to allocate memory from with the likes of
mmap(); mostly of concers to the system dynamic memory allocator.

# Zen Process Memory Map

## 0x00000000           all-zero page, invalid to derefer

## MACH_PAGE_SIZE       k_trapvec;

## 2 * MACH_PAGE_SIZE   p_zenvars;

# Zero Memory Interface

## Kernel Memory Allocators

- zone
- slab
- page