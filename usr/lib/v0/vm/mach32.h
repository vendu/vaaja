#ifndef __V0_MACH32_H__
#define __V0_MACH32_H__

#include <stddef.h>
#include <stdint.h>

/* virtual machine types */

typedef int32_t     v0word;         // signed register value
typedef uint32_t    v0uword;        // unsigned register value
typedef int64_t     v0wide;         // signed wide-register value
typedef uint64_t    v0uwide;   	    // unsigned wide-register value
typedef uint32_t    v0adr;
typedef int32_t     v0ofs;

/* machine types */

typedef uint64_t    v0segdesc;      // limit + page address + flags
typedef uint32_t    v0trapdesc;     // trap function address + flags
typedef uint32_t    v0pagedesc;     // page address + flags
typedef uint32_t    v0ioperm;       // I/O-permission bits
typedef uint32_t    v0iodesc;

struct v0thr {
    v0word      genregs[V0_GENERAL_REGISTERS];
    v0word      sysregs[V0_SYSTEM_REGISTERS];
};

struct v0vmatr {
    size_t      nbbram;             // size of BRAM-memory in bytes
    int8_t     *bram;               // block RAM
    size_t      nbram;              // size of RAM-memory in bytes
    int8_t     *ram;                // pointer to virtual machine "RAM"
    size_t      clshift;            // cacheline-size log2
    int8_t     *clbits;             // cacheline lock-bits
    size_t      pgshift;            // page-size log2
    v0pagedesc *pagetab;            // page-table of v0pagedesc-entries
    size_t       niomap;            // number of I/O-descriptors
    v0iodesc   **iomap;             // v0iodesc *iomap[niomap]
};

#if 0
struct v0memrom {
    long *hashpage(v0pagedesc pte);
    long *bufpage(void *tlb, v0pagedesc pte);
    long *chkpage(v0pagedesc pte, v0pagedesc flg);
    void *mappage(void *tlb, v0pagedesc pte, v0pagedesc flg);
    void *holdcl(void *adr);
    void *relcl(void *adr);
};

#define V0_BUS_MAX_DEVS 16
#define V0_BUS_ALL_DEVS (-1)
struct v0iorom {
    long *probe(long bus, long dev, void *atr, size_t n);
    void *map(long bus, long dev, void *atr, size_t nb);
    long  reset(long bus, long dev);
    long  start(long bus, long dev);
    long  stop(long bus, long dev);
};
#endif

#endif /* __V0_MACH32_H__ */

