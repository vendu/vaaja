#ifndef __DLFCN_H__
#define __DLFCN_H__

#include <features.h>
#include <stddef.h>

#if defined(_GNU_SOURCE)

#define RTLD_NEXT    ((void *)-1L)
#define RTLD_DEFAULT ((void *)0L)

typedef long Lmid_t;

#define LM_ID_BASE   0          // initial namespaces
#define LM_ID_NEWLM  (-1)       // dlmopen(); request new namespace

#endif /* defined(_GNU_SOURCE) */

extern void *dlopen(const char *file, int mode);
extern int   dlclose(void *handle);
extern void *dlsym(void *restrict handle, const char *restrict name);

#if defined(_GNU_SOURCE)

/* allocate object in a new namespace */
extern void *dlmopen(Lmid_t namespace, const char *file, int mode);
extern void *dlvsym(void *restrict handle, const char *restrict name,
                    const char *restrict version);

#endif /* defined(_GNU_SOURCE) */

extern char *dlerror(void);

#if defined(_GNU_SOURCE)

typedef struct {
    const char *dli_fname;      // file name
    void       *dli_fbase;      // load address
    const char *dli_sname;      // name of nearest symbol
    void       *dli_saddr;      // exact value of nearest symbol
} Dl_info;

extern int   dladdr(const void *adr, Dl_info *info);
extern int   dladdr1(const void *adr, Dl_info *info, void **extrainfo, int flg);

#define RTLD_DL_SYMENT  1       // matching symbol (const ElfNN_Sym)
#define RTLD_DL_LINKMAP 2       // struct link_map * containing the address

extern int   dlinfo(void *restrict handle, int req, void *restrict arg);

/* req(uest) values for dlinfo() */
#define RTLD_DI_LMID        1
#define RTLD_DI_LINKMAP     2
#define RTLD_DI_CONFIGADDR  3
#define RTLD_DI_SERINFO     4
#define RTLD_DI_SERINFOSIZE 5
#define RTLD_DI_ORIGIN      6
#define RTLD_DI_PROFILENAME 7
#define RTLD_DI_PROFILEOUT  8
#define RTLD_DI_TLS_MODID   9
#define RTLD_DI_TLS_DATA    10
#define RTLD_DI_MAX         10

/* type of elements in Dl_serinfo */
typedef struct {
    char         *dls_name;     // library search path directory
    unsigned int  dls_flags;    // directory origin
} Dl_serpath;

typedef struct {
    size_t       dls_size;
    unsigned int dls_cnt;
    Dl_serpath   dls_serpath[1];
} Dl_serinfo;

#endif /* defined(_GNU_SOURCE) */

#endif /* __DLFCN_H__ */

