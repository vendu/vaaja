#ifndef __KERN_UNIT_IA32_LINK_H__
#define __KERN_UNIT_IA32_LINK_H__

#if !defined(__ASSEMBLER__)

#include <stdint.h>
#include <kern/conf.h>

//extern char _kerntss;
extern char _eboot;
extern char _text;
extern char _etext;
extern char _data;
extern char _bss;
extern char _ebss;
#if (SMP)
extern char _mpentry;
extern char _emp;
#endif
extern char _usysinfo;
extern char _eusysinfo;
extern char _dmabuf;
extern char _edmabuf;
extern char _pagetab;
extern char _epagetab;
extern char _textvirt;
extern char _etextvirt;
extern char _datavirt;
extern char _bssvirt;
extern char _ebssvirt;
//extern char _ebssphys;
extern long _kernsize;
extern char _usr;
extern char _usrvirt;
extern char _eusr;
extern char _eusrvirt;
//extern char _eubssphys;

#endif /* !defined(__ASSEMBLER__) */

#endif /* __KERN_UNIT_IA32_LINK_H__ */

