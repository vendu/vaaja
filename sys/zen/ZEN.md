# vaaja source code roadmap
- [vaaja on github](https://github.com/vendu/vaaja/)

# data
- deq           - dual-end queue with minimal use of atomic synchronization
- htab          - hash table with cache-friendly table-chains for colllisions
- rbt           - red-black tree; memory/resource allocator book-keeping

# zen
- [zen on github](https://github.com/vendu/vaaja/blob/master/sys/zen)

## the zen kernel
- zen           - main kernel file + global data
- usr           - user-mode API
- sys           - system API (syscalls etc.)
  int64_t syscall(m_word_t num, m_word_t arg1, m_word_t arg2, m_word_t arg3);
- conf          - system configuration + information
  m_word_t sysconf(m_word_t id);
  POSIX
  -----
  SYS_ARG_MAX, SYS_TASK_MAX, SYS_HOST_NAME_MAX, SYS_LOGIN_NAME_MAX,
  SYS_NGROUPS_MAX, SYS_OPEN_MAX, SYS_PAGE_SIZE, SYS_STREAM_MAX = FOPEN_MAX,
  SYS_SYMLOOP_MAX, SYS_TTY_NAME_MAX, SYS_TZNAME_MAX
  non-POSIX
  ---------
  SYS_CL_SIZE, SYS_L1_SIZE, SYS_L1_INST_SIZE, SYS_L1_DATA_SIZE, SYS_L2_SIZE,
  SYS_L3_SIZE, SYS_TLB_ENTRIES, SYS_TRAP_MAX (system), SYS_INTERRUPT_MAX
  (dev/usr), SYS_PKT_BUF_MIN, SYS_PKT_BUF_MAX, SYS_PROC_SHM_MAX,
  SYS_PROC_MQ_MAX, SYS_PROC_EVQ_MAX, SYS_PHYS_PAGES, SYS_PROCESSORS_MAX,
  SYS_PROCESSORS_ONLINE,
- parm          - system parameters, 'sysctl'
- task          - process and thread support
  m_word_t proc(void);
  m_word_t task(struct taskatr *atr, m_word_t flg);

## tao; high-level scheduler
[tao implementation](https://github.com/vendu/vaaja/blob/master/sys/zen/sched/)

### ule         - adopted freebsd scheduler

### bvt         - borrowed virtual time scheduler

## vfs          - virtual filesystem layer
- node          - vfs node interface
  - reg         - regular files
  - chr         - character special (device) files
  - blk         - block special files
  - dir         - directories
  - link        - symbolic links
  - sock        - socket interface
  - mq          - message/event queues

## fs           - filesystem

### fs0         - 0-filesystem (based on Berkeley UFS/FFS)

### fat32       - FAT32-filesystem support

### iso9660     - ISO9660 support

## dev          - hardware- and pseudo-device support

### chr         - character/raw (non-block) devices
- null          - /dev/null
- zero          - /dev/zero
- tty           - /dev/tty - pseudo-teletype multiplexer
- kbd           - keyboard
- ptr           - pointer devices such as mouses and drawpads
- cam           - camera devices
- pkt           - packet [network] interface
- tap           - packet-filtering (debugging, firewall, ...)

### blk         - block-based devices
- tape          - tape devices
- disk          - disk devices
- opt           - optical disc devices
- ssd           - solid-state drives (no mechanical seek)
- net           - network devices

### ev          - event interface
- hid           - human interface devices (keyboard, pointer devices)
- fs            - filesystem events

## hw - ('high level') hardware interface
- unit          - processor and coprocessor devices
- trap          - interrupts, exceptions, aborts, faults, ...
- sig           - signal
- tmr           - timer devices
- page          - page daemon
- bus           - bus support
- io            - i/o mechanisms

## mem
- vm            - virtual memory
- shm           - shared memory (fast ipc)
- blk           - small-size allocations
- run           - multiple-of-page allocations
- slab          - allocations for blk and run abstractions
- big           - big allocations (mapped memory)
- buf           - buffer cache for block-devices
- map           - memory maps
  - dev         - i/o-mapped devices
  - node        - file- and other maps

TODO: cache auto-color, choices of data structures for subsystems, ...

