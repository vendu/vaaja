# Zen Kernel

Zen aims to be a hybrid kernel with a traditional monolithic design.

# Lowest Level

sys/page.[h|c]                  - page allocator/daemon
sys/intr.[h|c]                  - interrupt request handling; interrupt vector

# Processor Level

sys/tmr.[h|c]                   - system timers (RTC, PIC, HPET, ...)
sys/trap.[h|c]                  - system interrupt management
sys/vm.[h|c]                    - virtual memory manager (MMU)

# Kernel Level

- conf.h                        - system compile-time configuration
- task.[h|c]                    - task (process/thread) abstraction
- sched.[h|c]                   - system process scheduler
- dev                           - API for bus/device management
- mem                           - memory management
  - shm                         - shared memory segments for IPC/RPC
  - msg                         - message [queue] interface
- syn                           - synchronization facilities
                                - MTX, SPIN, TKT, THRBAR
- io                            - I/O tools and drivers
  - strm                        - streaming
  - nb                          - non-blocking
  - reg                         - regular blocking

## User Level

- sys                           - system abstractions such as 'files'
- tty                           - pseudo-teletype HID
- kbd                           - keyboard interface
- mouse                         - mouse interface
- hid                           - common Human Interface Device
- thr                           - multithreading API
- malloc                        - [dynamic] memory allocator
- io                            - I/O facilities

# Zen Thread and I/O Schedulers

- SCHED_UNI; uniform scheduler
- SCHED_ULE; a scheduler borrowed and slightly revamped from the FreeBSD camp
  - thanks a million guys!
