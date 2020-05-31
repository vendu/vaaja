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
#define ZEUSDATCOLOR        "magenta"
#define ZEUSPROG1COLOR      "green"
#define ZEUSPROG1DATCOLOR   "dark green"
#define ZEUSPROG2COLOR      "orange"
#define ZEUSPROG2DATCOLOR   "dark orange"
#if 0
#define ZEUSDATCOLOR        "blue"
#define ZEUSPROG1COLOR      "green"
#define ZEUSPROG1DATCOLOR   "dark green"
#define ZEUSPROG2COLOR      "orange"
#define ZEUSPROG2DATCOLOR   "gold"
#endif
#endif /* ZEUSWINX11 */
#define ZEUSTEXTNCOL        80
#define ZEUSTEXTNROW        8
#define ZEUSDBNCOL          40
#define ZEUSDBNROW          16
#define ZEUSSIMNCOL         50
#define ZEUSSIMNROW         160
#define ZEUSBUTTONW         48
#define ZEUSBUTTONH         24

struct zeussel {
    uint8_t *bmap;
    int      last;
};

char * zeusdisasm(long pc, int *lenret);

#endif /* __COREWAR_ZEUS_H__ */

