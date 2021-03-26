Solaris Doors IPC?

# Solaris Operation System Research Notes

## Multithreaded Kernel
- interrupts and kernel services are scheduled as regular kernel threads; these
  threads would include

### KERNEL-Thread

- takes care of small on-demand duties such as page allocation as well as
  periodical ('idle') tasks if we should opt to support them; the kernel is
  traditionally split into two parts; the 'low core' where the kernel/drivers
  deal with devices, as well as the 'high core' where the system-call interface
  and related tools are implemented.

## KERNEL ARCHITECTURE

## System Call Interface

### Task-Related Calls

exec();
fork();
vfork();
tfork(*fn, *arg, int flg);
// TFORK_NOSIGCHLD, TFORK_WAITPID
rfork();
// RFORK_PROC, RFORK_NOWAIT, RFORK_FD, RFORK_CFD, RFORK_MEM, RFORK_SIGSHARE
mmap();
munmap();
mremap();