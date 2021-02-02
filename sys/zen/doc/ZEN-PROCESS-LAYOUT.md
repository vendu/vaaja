# The Zen Book, Volume 0 - System Programming

## Zen Process Layout

Just like many other parts of Zen and my other projects, the system has been
deeply inspired by Unix-like operating systems. These systems still reflect
notable elegance and simplicity of the original designs, even though I think we
have to all admit it's showing a bit too much legacy payload and in some places,
e.g. due to differing 'standards', actual bloat.

With all due respect and thanks to everyone who designed, improved, and
contributed to Unix-likes, I hope we're starting 'next generation Unix'. :)

## Process Memory Segments

These segments are not to be mixed with processor memory segmentation used on
some systems, even though the implemmentations clearly overlap. The text below
present the memory map of a basic user segemnt; we will also discuss what's
different for system-mode segments; programs such as kernels are likely to use
custom segments. We also won't explain all debugging and other extra segments
you may see in custom linker scripts as that would be beyond the scope of this
document.

## The Segment Dsecriptor and Flags

In addition to the segment page address, segment descriptors use the following
bits to control memory management; the OBJ-prefixed bits are applied to objects
such as IO-buffers as well.

X, W, R, S, P, NT

X  - execute (OBJ_EXEC_BIT)
W  - write (OBJ_WRITE_BIT)
R  - read (OBJ_READ_BIT)
S  - system-mode only (OBJ_SYS_BIT)
P  - page is present in physical memory (OBJ_PRES_BIT)
     - wired memory (parts of the kernel and its buffers) is mapped 1-to-1 to
       RAM permanently (OBJ_WIRED_BIT, same as OBJ_PRES_BIT)
   - kernel, drivers, page-directories/tables, I/O and other memory buffers,
     kernel page-fault and other interrupt handlers

; see the Permissions and Attributes column in
the table below.

|    Segment    |    Brief              | Permissions and Attributes
|---------------|-----------------------|-------------|-------------------------
|:              |:                      |:            |: Runtime
| NULL          | zero-segments         | exec, read  | access raises SIGSEGV
| TEXT          | program code          | exec, read  |
| RODATA        | read-only data        | read        |
| DATA          | program data          | read, write | initialize values
| BSS           | uninitialized data    | read, write | allocate, zero
| STACK         | stack-segment         | read, write | allocate, grow downwards
| EXTRA SEGMENTS| VARIOUS               | VARIOUS     | VARIOUS
|---------------|-----------------------|---------------------------------------

## NULL Segment

The NULL segment can be considered a sort of dummy; its address as well as
all segment flags have the value of 0 (all zero-bits). In this scenario, the
NULL-segment is used to capture NULL-pointer dereferences, making the kernel
send the SIGSEGV-signal to the user process. This is a traditional place to
put the interrupt vactor, but it's probably a good idea to have guard pages in
low memory in order not to execute code on the stack because of program counter
(instruction pointer) overflows.

## TEXT Segment

The TEXT segment contains executable process code. It is filled at compile-
time.

## RODATA Segment

The RODATA segment contains read-only data. This segment is initialized at
compile-time.

## DATA Segment

The data segment contains initialized data written at compile-time.

## BSS Segment

The name of the BSS has long roots; originally, it stood for 'Block Started
by Symbol'. Its contents are allocated and initialized to zero at run time.

## COMM Segment

TODO: is this under RODATA? I think it contains program/code comments.

The COMM Segment

## DEBUG Segment

The debug section is used by the linker to store its extra data.

## HEAP Segment

The heap segment is one from which to allocate memory from with the likes of
mmap(); mostly of concers to the system dynamic memory allocator.

## Zen Process Memory Map

### 0x00000000           all-zero page, invalid to derefer

## MACH_PAGE_SIZE       k_trapvec;

## 2 * MACH_PAGE_SIZE   p_zenvars;

# Zero Memory Interface

## Kernel Memory Allocators

- zone
- slab
- page