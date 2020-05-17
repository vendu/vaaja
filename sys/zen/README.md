# Zen Kernel
---

The goal of Zen is to create a fast, modern kernel for systems from smartphones
to high-performance audio-visual workstations.

## Source Code Modules and Layout

api		- Kernel Programming Interfaces
bsp/v0		- V0 board support package
conf		- system compile-time configuration
dev		- I/O-device support and drivers
io		- I/O operations
net/ip4		- Internet Protocol v4 support
mem		- memory manager for the next generation zero malloc
net		- TCP/IP v4 and v6 networking support
signal		- UNIX/POSIX-like signal support interface
tao		- thread scheduler modules
tao/bvt		- borrowed virtual time task/thread scheduler
tao/ule		- adopted FreeBSD ULE-scheduler re-engineering
task		- support for processes and threads
tmr		- timer device support; INTR, HIRES, RTC
trap		- interrupts and exceptions
vfs		- virtual filesystem interface
vm		- virtual memory support
zenfs		- Zen File System based on Berkeley FFS

