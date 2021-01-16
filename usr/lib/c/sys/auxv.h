#ifndef __SYS_AUXV_H__
#define __SYS_AUXV_H__

#define AT_BASE          0      // base address of program interpreter (ld.so)
#define AT_CLKTCK        1      // times() count frequency; sysconf(_SC_CLK_TCK)
#define AT_DCACHEBSIZE   2      // data cache block size
#define AT_UID           3      // thread real user ID
#define AT_GID           4      // thread real group ID
#define AT_EUID          5      // thread effective user ID
#define AT_EGID          6      // thread effective group ID
#define AT_ENTRY         7      // executable entry address
#define AT_EXECFD        8      // program file descriptor
#define AT_EXECFN        9      // program execution path
#define AT_HWCAP         10     // processor capability bits
#define AT_HWCAP2        11     // more processor capability bits
#define AT_ICACHEBSIZE   12     // instruction cache block size
#define AT_PAGESZ        13     // system page size; sysconf(_SC_PAGESIZE)
#define AT_PHDR          14     // executable program header file address
#define AT_PHENT         15     // size of program header entry
#define AT_PHNUM         16     // number of program headers
#define AT_PLATFORM      17     // address of hardware identification string
#define AT_RANDOM        18     // address of 16-byte random value
#define AT_SECURE        20     // treat securely; setuid, setgid, capabilities
#define AT_SYSINFO       21     // entry point of system call function in VDSO
#define AT_SYSINFO_EHDR  22     // VDSO address
#define AT_UCACHEBSIZE   23     // unified cache block size

#define AT_FLAGS         511
#define AT_FPUCW         510    // FPU initialisation control word
#define AT_BASE_PLATFORM 509    // real platform identification string address

unsigned long getauxval(unsigned long type);

#endif /* __SYS_AUXV_H__ */

