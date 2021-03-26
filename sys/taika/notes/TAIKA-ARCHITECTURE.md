# Taika Kernel Architecture

Taika aims to be a relatively tradiation, modular but monolithic kernel - I
chose monolithic because it seems relatively easy to implement, yet should allow
for all advantages of micro- and such kernel approaches - at least those
relevant to home/desktop users whom I'm targeting first. Well, the very first
target would obviously be programmers to create the system. :)

## Taika Process Schedulers

To apply the KISS philosophy, I'm getting started from a traditional model with
a few different tasks; interrupt priorities are higher than any other; the next
in line are realtime priorities, followed by system priorities and finally, by
interactive tasks. As a special case of interactive tasks, the window/desktop
system can boost priorities of the tasks with window focus or urgent pending I/O
(such as audio and video buffer synchronization).

## Kernel/Interrupt Threads; priorities 0-7

PID       service       brief       priority        attributes
:---------:-------------:-----------:---------------:---------------------
0x00      KERNEL        system      0x00             BOOT, KMOD, SYSCTL, SYSCONF
0x01      INIT          master      0x01             LOWLAT
0x02      HID           human input 0x03             LOWLAT, POLL
0x03      BUF           buffer-I/O  0x03             LOWLAT; VM, VFS, IPC
0x04      AUD           audio       0x04             LOWLAT, BURST
0x05      VID           video       0x05             LOWLAT, BURST
0x06      NET           network I/O 0x06             MIDLAT, URGIO, BUF
0x07      DSK           disks/discs 0x07             MECHSEEK, HIGHLAT, BUF

## Realtime Threads; priorities 8-15

priority  service       brief       attributes
:---------:-------------:-----------:-------------
0x08      realtime      soft/hard   : RTFIFO, RTRR
0x09
0x0a
0x0b
0x0c
0x0d
0x0e
0x0f

## Taika Inter-Process Communications

We shall implement traditional Unix sockets and pipes (perhaps applying the
notion of sounds and graphics where relevant). Multimedia pipes, anyone?

Both System V and POSIX IPC mechanisms are supported as well.

For graphical desktops and other such services typically running locally, we
shall device something close in nature to Solarid Doors or XOrg event queues;
messages on these queues can invoke the corresponding servers directly, giving
us low-latency and ultra-fast turnaround because the client won't need to wait
for the server thread to be scheduled to complete the request. These messages
can probably be handled in a single-producer OR single-consumer fashion without
having to lock the message data structures (typically queues).

## Taika Virtual Memory

The virtual memory system presents a simple linear range of memory, an address
space, to processes. This space is split into several segments that represent

- mappings of the exacutable
- heap space (general-purpose, process-allocated memory)
- shared libraries
- program stack

These segments are divided into equal-sized pieces of memory called pages, with
a typical page size being something like 4 or 8 kilobytes.

## Taika Directory Hierarchy

- conventionally, directory names in the diagram end with a '/'

                     /
etc/    sbin/   bin/    dev/    usr/    opt/
passwd  ls                   lib/  adm/

## Taika File Types

### Regular Files
- data within the file system

### [Block or Character] Special Files
- translated by a device driver into I/O of some type

### Pipes
- pipes don't hold data, but pass it between two processes

### Hard Links
- exist to let the same data have different names in a filesystem

### Symbolic Links
- point to other path names on any file system

### [Domain] Sockets in the file system enable local communcation between
processes

## Special File Systems

### /proc represents address spaces of processes as series of files

