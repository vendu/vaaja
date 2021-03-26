#ifndef __KERN_MEM_OBJ_H__
#define __KERN_MEM_OBJ_H__

/* flg-values */
#define MEM_SHARE    (1 << 0)
#define MEM_RESIZE   (1 << 1)
#define MEM_GROWDOWN (1 << 2)
#define MEM_UMAP     (1 << 3)
#define MEM_BUF      (1 << 4)
#define MEM_WIRE     (1 << 5)
#define MEM_PHYS     (1 << 6)
#define MEM_DMA      (1 << 7)
#define MEM_MMIO     (1 << 8)
#define MEM_NOCACHE  (1 << 9)
struct memobj {
    void           *adr;        // base address of region
    size_t          ofs;        // offset into region (for locks and such)
    size_t          len;	// length in bytes
    long            flg;        // flag-bits
    struct perm    *perm;       // permission structure
    struct memzone *zone;       // allocation memory zone
};

/*
 * ctor()       - called on _every_ allocation
 * dtor()       - called after every free
 * init()       - called when memory cached into a zone
 * fini()       - called when zone memory released to system for other use
 */
typedef long __memobjctor(void *adr, size_t size, void *arg, long flg);
typedef void __memobjdtor(void *adr, size_t size, void *arg);
typedef long __memobjinit(void *adr, size_t size, long flg);
typedef void __memobjfini(void *adr, size_t size);
struct memobjfuncs {
    __memobjctor *ctor;
    __memobjdtor *dtor;
    __memobjinit *init;
    __memobjfini *fini;
};

#endif /* __KERN_MEM_OBJ_H__ */

