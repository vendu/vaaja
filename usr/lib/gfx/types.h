#ifndef __GFX_TYPES_H__
#define __GFX_TYPES_H__

/* FIXME: make X11 optional */

#include <gfx/x11.h>
#include <gfx/rgb.h>

#if 0
struct gfximg {
    long  flg;
    long  w;
    long  h;
    void *data;
};
#endif

struct gfximg_x11 {
    char            *fname;     // file name
    gfxargb32       *data;      // image data
    gfxargb32       *maskdata;  // image mask
    unsigned long    w;         // width
    unsigned long    h;         // height
    Imlib_Image     *imlib2;    // for Imlib2 support
    XImage          *xim;       // for X11/XOrg support
#if (USE_SHM)
    /* support for X11/XOrg shared memory extension XShm */
    int              shmid;
    int              pmapshmid;
    XShmSegmentInfo  ximseg;
    XShmSegmentInfo  pmapseg;
#endif
    Pixmap           pmap;      // cached rendition of image
    Pixmap           mask;      // cached mask
};

#endif /* __GFX_TYPES_H__ */

