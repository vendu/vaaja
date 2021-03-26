/*
 * Thanks to
 * - Martin 'bluet' Stensgård
 * - Tim 'amon' Zebulla
 * for testing this code. :)
 * -vendu
 */

/* TODO: fix this stuff to run on SMP (per-CPU m_cpuinfo structures) */

/*
 * README: http://softpixel.com/~cwright/programming/simd/cpuid.php
 */
#if !defined(TEST)
#define TEST 0
#endif

#if (TEST)
#include <stdio.h>
#include <stdlib.h>
#endif

#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <mach/types.h>
#include <kern/util.h>
#include <kern/cpu.h>

#if defined(__ZEROKERNEL__) /* kernel-level stuff only */
#include <kern/unit/x86/cpu.h>
#endif

struct m_cacheinfo cpuidcacheinfo[16] ALIGNED(PAGESIZE);
struct m_cpuinfo   cpuinfo;

/* vendor strings. */
static const char *_vendortab[]
= {
    "AuthenticAMD",
    "GenuineIntel",
    "CentaurHauls",
    "GenuineTMx86",
    "CyrixInstead",
    "CentaurHauls",
    "NexGenDriven",
    "UMC UMC UMC ", /* FIXME - Is the space right? */
    "SiS SiS SiS ", /* FIXME - Same. */
    "Geode by NSC",
    "RiseRiseRise"
};

#define cpuidgetinfo(ptr)                                               \
    cpuid(M_CPUIDINFO, ptr)
#define cpuidgetexti(ptr)                                               \
    cpuid(M_CPUIDEXTINFO, ptr)
#define cpuidgetci_intel(ptr)                                           \
    cpuid(M_CPUIDCACHE, ptr)
#define cpuidgetl1_amd(ptr)                                             \
    cpuid(M_CPUIDL1_AMD, ptr)
#define cpuidgetl2_amd(ptr)                                             \
    cpuid(M_CPUIDL2_AMD, ptr)

/* M_CPUIDINFO */
#define cpuidstepping(ptr)                                              \
    ((ptr)->eax & 0x0000000f)
#define cpuidmodel(ptr)                                                 \
    (((ptr)->eax & 0x000000f0) >> 4)
#define cpuidfamily(ptr)                                                \
    (((ptr)->eax & 0x00000f00) >> 8)
#define cpuidtype(ptr)                                                  \
    (((ptr)->eax & 0x00003000) >> 12)
#define cpuidextmodel(ptr)                                              \
    (((ptr)->eax & 0x000f0000) >> 16)
#define cpuidextfamily(ptr)                                             \
    (((ptr)->eax & 0x0ff00000) >> 20)
#define cpuidhastsc(ptr)                                                \
    ((ptr)->edx & CPUIDTSC)
#define cpuidhaspse(ptr)                                                \
    ((ptr)->edx & CPUIDPSE)
#define cpuidhaspge(ptr)                                                \
    ((ptr)->edx & CPUIDPGE)
#define cpuidhassep(ptr)                                                \
    ((ptr)->edx & CPUIDSEP)
#define cpuidhasmmx(ptr)                                                \
    ((ptr)->edx & CPUIDMMX)
#define cpuidhasclfl(ptr)                                               \
    ((ptr)->edx & CPUIDCLFL)
#define cpuidhasfxsr(ptr)                                               \
    ((ptr)->edx & CPUIDFXSR)
#define cpuidhassse(ptr)                                                \
    ((ptr)->edx & CPUIDSSE)
#define cpuidhassse2(ptr)                                               \
    ((ptr)->edx & CPUIDSSE2)
#define cpuidhassse3(ptr)                                               \
    ((ptr)->ecx & CPUIDSSE3)
#define cpuidhasapic(ptr)                                               \
    ((ptr)->edx & CPUIDAPIC)

/* CPUIDEXTINFO */
#define cpuidhasamd_mmx(ptr)                                            \
    ((ptr)->edx & CPUIDAMD_MMX)
#define cpuidhas3dnow(ptr)                                              \
    ((ptr)->edx & CPUID3DNOW)
#define cpuidhas3dnow2(ptr)                                             \
    ((ptr)->edx & CPUID3DNOW2)

/* %edx flags. */

/* CPUIDINFO */
#define CPUIDFPU       0x00000001 /* fpu present. */
#define CPUIDVME       0x00000002 /* virtual mode extensions. */
#define CPUIDDE        0x00000004 /* debugging extensions. */
#define CPUIDPSE       0x00000008 /* page size extensions. */
#define CPUIDTSC       0x00000010 /* time stamp counter. */
#define CPUIDMSR       0x00000020 /* model-specific registers. */
#define CPUIDPAE       0x00000040 /* physical address extensions. */
#define CPUIDMCE       0x00000080 /* machine check exceptions. */
#define CPUIDCXCHG8    0x00000100 /* compare and exchange 8-byte. */
#define CPUIDAPIC      0x00000200 /* on-chip apic. */
#define CPUIDSEP       0x00000800 /* sysexit and sysenter. */
#define CPUIDMTRR      0x00001000 /* memory type range registers. */
#define CPUIDPGE       0x00002000 /* page global enable. */
#define CPUIDMCA       0x00004000 /* machine check architecture. */
#define CPUIDCMOV      0x00008000 /* conditional move-instruction. */
#define CPUIDPAT       0x00010000 /* page attribute table. */
#define CPUIDPSE36     0x00020000 /* 36-bit page size extensions. */
#define CPUIDPSN       0x00040000 /* processor serial number. */
#define CPUIDCLFL      0x00080000 /* CLFLUSH - fixme? */
#define CPUIDDTES      0x00200000 /* debug trace and emon store msr's. */
#define CPUIDACPI      0x00400000 /* thermal control msr. */
#define CPUIDMMX       0x00800000 /* mmx instruction set. */
#define CPUIDFXSR      0x01000000 /* fast fpu save/restore. */
#define CPUIDSSE       0x02000000 /* sse. */
#define CPUIDSSE2      0x04000000 /* sse2. */
#define CPUIDSS        0x08000000 /* selfsnoop. */
#define CPUIDHTT       0x10000000 /* hyper-threading technology. */
#define CPUIDTM1       0x20000000 /* thermal interrupts, status msr's. */
#define CPUIDIA64      0x40000000 /* ia-64 (64-bit m_cpu). */
#define CPUIDPBE       0x80000000 /* pending break event. */

/* CPUIDEXTINFO */
#define CPUIDAMD_MMX   0x00400000
#define CPUID3DNOW2    0x40000000
#define CPUID3DNOW     0x80000000

/* %ecx flags. */
#define CPUIDSSE3      0x00000001 /* sse3. */

void
cpuprobe(struct cpu *cpu)
{
    struct m_cpuinfo      *info = &cpu->info;
    struct m_cpucacheinfo *cache = &cpu->info.cache;
    struct m_cacheinfo    *cbuf;
    struct m_cpuid         buf;
    union  m_cpuidvendor   vbuf;
    long                   flg;

    cpuidgetvendor(&vbuf);
    if (!__strcmp((const char *)vbuf.str, _vendortab[CPUIDINTEL])) {
        cpuidinitci_intel(cpuidcacheinfo);
        cpuidgetci_intel(&buf);
        cbuf = &cpuidcacheinfo[M_CPUIDINSTRCACHE];
        cache->l1i.size = cbuf->size;
        cache->l1i.clsz = cbuf->xsize;
        cache->l1i.nway = cbuf->nway;
        cbuf = &cpuidcacheinfo[M_CPUIDINSTRTLB];
        cache->l1i.ntlb = cbuf->xsize;
        cbuf = &cpuidcacheinfo[M_CPUIDDATACACHE];
        cache->l1d.size = cbuf->size;
        cache->l1d.clsz = cbuf->xsize;
        cache->l1d.nway = cbuf->nway;
        cbuf = &cpuidcacheinfo[M_CPUIDDATATLB];
        cache->l1i.ntlb = cbuf->xsize;
        cbuf = &cpuidcacheinfo[M_CPUIDUNICACHE];
        cache->l2.size = cbuf->size;
        cache->l2.clsz = cbuf->xsize;
        cache->l2.nway = cbuf->nway;
    } else if (!__strcmp((const char *)vbuf.str, _vendortab[CPUIDAMD])) {
        cpuidgetl1_amd(&buf);
        cache->l1i.size = buf.edx >> 14;
        cache->l1i.clsz = buf.edx & 0xff;
        cache->l1i.nway = (buf.edx >> 16) & 0xff ;
        cache->l1i.ntlb = buf.ebx & 0xff;
        cache->l1d.size = buf.ecx >> 14;
        cache->l1d.clsz = buf.ecx & 0xff;
        cache->l1d.nway = (buf.ecx >> 16) & 0xff ;
        cache->l1d.ntlb = (buf.ebx >> 16) & 0xff;
        cpuidgetl2_amd(&buf);
        cache->l2.size = buf.ecx >> 6 & 0xffc00;
        cache->l2.clsz = buf.ecx & 0xff;
        cache->l2.nway = (buf.ebx >> 12) & 0x0f;
        cache->l2.ntlb = buf.ebx & 0x0fff;
    }
    cpuidgetinfo(&info->id);
    flg = CPUHASINFO;
    if (cpuidhasfxsr(&info->id)) {
        flg |= CPUHASFXSR;
    }
    if (cpuidhasapic(&info->id)) {
        flg |= CPUHASAPIC;
    }
    cpu->flg = flg;

    return;
}

void
cpuprintinfo(long id)
{
    volatile struct cpu  *cpu = &k_cputab[id];
    struct m_cacheinfo    cbuf;
    struct m_cpuid        buf;
    union  m_cpuidvendor  vbuf;
    struct m_cpuidcregs   crbuf;

    cpuidgetvendor(&vbuf);
    __printf("CPU: vendor: %s\n", vbuf.str);
    if (!__strcmp((const char *)vbuf.str, _vendortab[CPUIDINTEL])) {
        cpuidinitci_intel(&cbuf);
        cpuidgetci_intel(&buf);
        cpuid_print_cache_info_intel(buf.eax);
        cpuid_print_cache_info_intel(buf.ebx);
        cpuid_print_cache_info_intel(buf.ecx);
        cpuid_print_cache_info_intel(buf.edx);
    } else if (!__strcmp((const char *)vbuf.str, _vendortab[CPUIDAMD])) {
        cpuidgetl1_amd(&buf);
        cpuid_print_l1_info_amd(&buf);
        cpuidgetl2_amd(&buf);
        cpuid_print_l2_info_amd(&buf);
    }
    /* stepping, model, family, type, ext_model, ext_family */
    cpuidgetinfo(&buf);
#if 0
    __printf("cpu info:\n");
    __printf("\tstepping: %u\n", cpuidstepping(&buf));
    __printf("\tmodel: %u\n", cpuidmodel(&buf));
    __printf("\tfamily: %u\n", cpuidfamily(&buf));
    __printf("\ttype: %u\n", cpuidtype(&buf));
    __printf("\text_model: %u\n", cpuidextmodel(&buf));
    __printf("\text_family: %u\n", cpuidextfamily(&buf));
#endif

    __printf("cpu features:");
    if (cpuidhassep(&buf)) {
        __printf(" sep");
    }
    if (cpuidhasmmx(&buf)) {
        __printf(" mmx");
    }
    if (cpuidhasclfl(&buf)) {
        __printf(" clfl");
    }
    if (cpuidhasfxsr(&buf)) {
        __printf(" fxsr");
    }
    if (cpuidhassse(&buf)) {
        __printf(" sse");
    }
    if (cpuidhassse2(&buf)) {
        __printf(" sse2");
    }
    if (cpuidhassse3(&buf)) {
        __printf(" sse3");
    }
//    __printf("\n");

    cpuidgetexti(&buf);
//    __printf("amd features: ");
    if (cpuidhasamd_mmx(&buf)) {
        __printf(" mmx");
    }
    if (cpuidhas3dnow(&buf)) {
        __printf(" 3dnow");
    }
    if (cpuidhas3dnow2(&buf)) {
        __printf(" 3dnow2");
    }
    __printf("\n");

#if defined(__ZEROKERNEL__)
    cpugetmodes(&crbuf);
    __printf("cpu modes:");
    if (cpuhastsc(&crbuf)) {
        __printf(" tsc");
    }
    if (cpuhasde(&crbuf)) {
        __printf(" de");
    }
    if (cpuhaspse(&crbuf)) {
        __printf(" pse");
    }
    if (cpuhaspge(&crbuf)) {
        __printf(" pge");
    }
    if (cpuhaspce(&crbuf)) {
        __printf(" pce");
    }
    __printf("\n");
#endif

    return;
}

#if (TEST) && 0
int
main(int argc,
     char *argv[])
{
    cpuprobe(&cpuinfo);
    cpuprintinfo();

    exit(0);
}
#endif /* TEST */

