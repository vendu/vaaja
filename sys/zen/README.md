# Zen Kernel
---

The goal of Zen is to create a fast, modern kernel for systems from smartphones
to high-performance audio-visual workstations.

The very first version's goal is to implement an efficient 32-bit game console
on inexpensive FPGA-kits.

## Source Code Modules and Layout

| module        | brief
-:--------------|:-----
bsp/v0		| V0 board support package
conf		| system compile-time configuration
dev		| I/O-device support and drivers
errno           | error reporting
ev              | event interface
fs0		| Zen File System based on Berkeley FFS
io		| I/O operations
macros          | small routines for miscellaneous things
mem		| memory manager for kmalloc
net		| network support
net/ip4		| TCP/IP v4 support
perm            | permission control
sched		| thread scheduler modules
signal		| Unix/POSIX-like signal support interface
sys             | system operations
task		| processes and threads
tao/bvt		| borrowed virtual time task/thread scheduler
tao/ule		| re-engineered/adopted FreeBSD ULE-scheduler
tmr		| timer device support; INTR, HIRES, RTC
trap		| interrupts and exceptions
util            | string operations, printf(), etc.
var             | system parameters interface
vfs		| virtual filesystem interface
vm		| virtual memory support
zen             | main kernel module

