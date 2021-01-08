# Zen Kernel

The goal of Zen is to create a fast, modern kernel for systems from smartphones
to high-performance audio-visual workstations and possibly beyond.

The very first version's goal is to implement an efficient 32-bit game console
for inexpensive FPGA-kits.

## Source Code Modules and Layout

### bsp/v0      - V0 board support package

The board support packages contain platform-specific declarations. Of particular
interest would be the following, i

#### System Parameters

| macro                 | brief
|-----------------------|------
| ZEN_CL_SIZE           | memory subsystem cacheline size
| ZEN_STRIDE_SIZE       | memory cache stride (access) size
| ZEN_PAGE_SIZE         | virtual memory page size
| ZEN_JUMBO_SIZE        | virtual memory huge page size (if applicable, else 0)
| ZEN_BANK_SIZE         | physical memory bank size

### conf        - system compile-time configuration

The conf.h header file controls system features, lists some [default] system
limits, and states some modules to use for different tasks (e.g. thread
scheduler).

### dev         - I/O-device support and drivers

The toplevel <zen/dev.h> header file lists some default devices such as null.

### errno       - error reporting

<zen/errno.h> lists system values for errno [for error-reporting].

### ev          - event interface

The ev-module implements event/message queues to control the system and desktop.

### fs0         - Zen File System based on Berkeley FFS

The fs0-filesystem is based on the Berkeley Fast Filesystem (FFS) aka UFS.

### io          - I/O operations

The I/O-subsystem interfaces with device drivers to perform input and output for
devices such as disks and network interfaces.

### macros      - small routines for miscellaneous things

<zen/macros.h> has auxiliary macros for common operations.

### mem         - memory manager for kmalloc

The mem-module implements allocation routines for virtual and physical memory.

### net         - network support

Zero supports Internet-style networking using TCP/IP and related protocols.

### net/ip4     - TCP/IP v4 support

Internet protocols version 4 support module.

### perm        - permission control

Basic Unix-like permissions (user and group).

### sched       - thread scheduler modules

Different thread schedulers for different uses.

### signal      - Unix/POSIX-like signal support interface

Primitive interprocess communications using signals.

### sys         - system operations

Interface for the system/machine, including some hardware-dependent things.

### task        - processes and threads

Zen task abstraction.

### tao/bvt     - borrowed virtual time task/thread scheduler

A thread scheduler.

### tao/ule     - re-engineered/adopted FreeBSD ULE-scheduler

Revamped FreeBSD scheduler targeted at desktop use.

### tmr         - timer device support; INTR, HIRES, RTC

Timers; interrupt, high-resolution, real-time clock, etc.

### trap        - interrupts and exceptions

Interrupts, exceptions, faults, aborts, ...

### util        - miscellaneous tools

The util-module implements some common routines such as kprintf() and kpanic().

### var         - system parameters interface

System configuration parameters such as page-size exposed as a mapped page of
variables and values.

### vfs         - virtual filesystem interface

Zen virtual filesysten for I/O.

### vm          - virtual memory support

Zen virtual memory support incl. the page daemon.

### zen         - main kernel module

Kernel initialisation and loop etc.

