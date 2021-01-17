#include <stddef.h>
#if !defined(__zen__)
#include <stdlib.h>
#include <vt/term.h>
#endif
#if defined(__zen__)
#define FREE(x)   kfree(x)
#define MALLOC(x) kmalloc(x)
#else
#if !defined(FREE)
#define FREE(x) free(x)
#endif
#if !defined(MALLOC)
#define MALLOC(x) malloc(x)
#endif
#endif
#include <vt/vt.h>

#if defined(__zen__)

long
vtinitbuf(struct vt *vt)
{
    if (!ringinit(&vt->devbuf.in, NULL, VT_BUF_SIZE / sizeof(RING_ITEM))) {
        
        return 0;
    }
    
    return 1;
}

void
vtfreebuf(struct vt *vt)
{
    void *ptr;

    ptr = vt->devbuf.in.base;
    if ((ptr) && ptr != vt->devbuf.in.data) {
        FREE(ptr);
        vt->devbuf.in.base = NULL;
    }
    ptr = vt->devbuf.out.base;
    if ((ptr) && ptr != vt->devbuf.out.data) {
        FREE(ptr);
        vt->devbuf.out.base = NULL;
    }
}

#else /* !__zen__ */

long
vtinitbuf(struct vt *vt)
{
    void *ptr;

    ptr = MALLOC(TERM_INBUF_SIZE);
    if (!ptr) {

        return 0;
    }
    vt->iobuf.nin = TERM_INBUF_SIZE / sizeof(TEXT_T);
    vt->iobuf.inbuf = ptr;
    vt->iobuf.inptr = ptr;
    ptr = MALLOC(TERM_OUTBUF_SIZE);
    if (!ptr) {
        FREE(vt->iobuf.inbuf);
        vt->iobuf.nin = 0;
        vt->iobuf.inbuf = NULL;

        return 0;
    }
    vt->iobuf.nout = TERM_OUTBUF_SIZE / sizeof(TEXT_T);
    vt->iobuf.outbuf = ptr;
    vt->iobuf.outptr = ptr;

    return 1;
}

void
vtfreebuf(struct vt *vt)
{
    void *ptr;

    ptr = vt->iobuf.inbuf;
    if (ptr) {
        FREE(ptr);
    }
    ptr = vt->iobuf.outbuf;
    if (ptr) {
        FREE(ptr);
    }

    return;
}

#endif

