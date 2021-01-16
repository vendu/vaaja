#ifndef __UI_FONT_H__
#define __UI_FONT_H__

#define UI_FONT_XORG      0
#define UI_FONT_FREETYPE2 1
struct uifont {
    const char                 *name;
    long                        type;
    void                        *data;
};

#endif /* __UI_FONT_H__ */

