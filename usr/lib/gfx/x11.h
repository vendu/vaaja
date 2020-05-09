#ifndef __GFX_X11_H__
#define __GFX_X11_H__

#define USE_SHM 0

#include <X11/Xlib.h>
#if (USE_SHM)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif
#if (USE_COMPOSITE)
#include <X11/extensions/composite.h>
#endif
#if (USE_XV)
#include <X11/extensions/Xvlib.h>
#endif

//#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xlocale.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <Imlib2.h>

#endif /* __GFX_X11_H__ */

