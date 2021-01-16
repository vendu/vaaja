#ifndef __UI_WIN_H__
#define __UI_WIN_H__

struct uiwin {
    long           id;
    void         **evtab;
    struct uiwin  *prev;
    struct uiwin  *next;
};

#endif /* __UI_WIN_H__ */

