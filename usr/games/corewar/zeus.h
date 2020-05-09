#ifndef __COREWAR_ZEUS_H__
#define __COREWAR_ZEUS_H__

#include <corewar/conf.h>
#if defined(ZEUSWINX11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>

#include <corewar/x11.h>
#endif

#if defined(ZEUSWINX11)
#define ZEUSBUTTONTEXTCOLOR "orange"
//#define ZEUSDATCOLOR        "magenta"
#define ZEUSDATCOLOR        "blue"
#define ZEUSPROG1COLOR      "green"
#define ZEUSPROG1DATCOLOR   "dark green"
#define ZEUSPROG2COLOR      "orange"
#define ZEUSPROG2DATCOLOR   "gold"
#endif /* ZEUSWINX11 */
#define ZEUSTEXTNCOL        80
#define ZEUSTEXTNROW        16
#define ZEUSDBNCOL          40
#define ZEUSDBNROW          16
#define ZEUSSIMNCOL         160
#define ZEUSSIMNROW         50
#define ZEUSBUTTONW         64
#define ZEUSBUTTONH         48

struct zeussel {
    uint8_t *bmap;
    int      last;
};

char * zeusdisasm(long pc, int *lenret);

#endif /* __COREWAR_ZEUS_H__ */

