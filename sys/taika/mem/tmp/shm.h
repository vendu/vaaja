#ifndef __KERN_MEM_SHM__
#define __KERN_MEM_SHM__

struct shmhdr {
    long  key;
    long  flg;
    void *adr;
};

#endif /* __KERN_MEM_SHM__ */

