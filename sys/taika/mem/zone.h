#ifndef SYS_TAIKA_MEM_ZONE_H
#define SYS_TAIKA_MEM_ZONE_H

/* flags for creating memzones */
#define MEMZONE_ZERO            0x00000001 // initialize with zeroes
#define MEMZONE_STATIC          0x00000002 // static-size zone
#define MEMZONE_HEADER          0x00000004 // real memory (slab etc. structures)
#define MEMZONE_MALLOC          0x00000008 // zone is for use with (k)malloc()
#define MEMZONE_NOFREE          0x00000010 // slabs never returned to virtual
#define MEMZONE_NODUMP          0x00000020 // pages not included into mini-dumps
#define MEMZONE_SHADOW          0x00000040 // shadow copies allocated for every CPU
#define MEMZONE_OFFPAGE         0x00000080 // book-keeping separate from slab pages
#define MEMZONE_HASH            0x00000100 // hash book-keeping information
#define MEMZONE_VTOSLAB         0x00000200 // locate book-keeping with struct vmpage
#define MEMZONE_VIRTUAL         0x00000400 // zone is for virtual memory subsystem
#define MEMZONE_REFCNT          0x00000800 // keep allocation reference counts

/* allocation memory zones */
#define MEM_LOW_ZONE            0 // low-memory (x86: boot)
#define MEM_IO_ZONE             1 // I/O zone (x86: DMA)
#define MEM_SYS_ZONE            2 // system zone (page-tables and such)
#define MEM_VIRT_ZONE           3 // virtual memory zone
#define MEM_PHYS_ZONE           4 // physical memory zone
#define MEM_DEV_ZONE            5 // memory-mapped device zone
#define MEM_BUF_ZONE            6 // buffer cache zone
#define MEM_STK_ZONE            7 // stack segments
#define MEM_ZONES               8

struct memzone {
    struct membkt  tab[MACH_PTR_BITS];
    MEM_LK_T       lk;
    m_size_t       flg;
    uintptr_t      base;
    unsigned long  nblk;
    void          *hdrtab;
};

void * slaballoc(struct memzone *zone, m_size_t nb, long flg);
void   slabfree(struct memzone *zone, void *ptr);

//void * zonegetslab(struct memzone *zone, unsigned long nb, unsigned long flg);
//#void   zonefreeslab(struct memzone *zone, void *ptr);

void meminitphys(struct memzone *zone, m_size_t base, m_size_t nbyte);
void meminitvirt(struct memzone *zone, m_size_t nbvirt);
void memfree(struct memzone *zone, void *ptr);

#endif /* SYS_TAIKA_MEM_ZONE_H */

