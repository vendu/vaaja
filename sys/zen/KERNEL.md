# Zen Kernel

## 0. Lowest Level

sys/page.[h|c]                  - Zen page allocator/daemon

## 1. Processor Level

sys/tmr.[h|c]                   - system timers (RTC, PIC, HPET, ...)
sys/trap.[h|c]                  - system interrupt management
sys/vm.[h|c]                    - virtual memory manager (MMU)

## 2. Kernel Level

- conf.h                        - system compile-time configuration
- task.[h|c]                    - task (process/thread) abstraction
- sched.[h|c]                   - system process scheduler
- mem                           - memory management
- io                            - I/O tools and drivers

## 3. User Level

- sys                           - system abstractions such as 'files'
- tty                           - pseudo-teletype HID
- kbd                           - keyboard interface
- mouse                         - mouse interface
- hid                           - common Human Interface Device
- thr                           - multithreading API
- malloc                        - [dynamic] memory allocator
- io                            - I/O facilities

