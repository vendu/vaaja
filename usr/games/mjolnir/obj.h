#ifndef __MJOLNIR_OBJ_H__
#define __MJOLNIR_OBJ_H__

#include <zero/cdefs.h>
#include <mjolnir/chr.h>
#include <mjolnir/item.h>

struct mjolobjfunc {
    long              (*inv)(void *);                   // chr, returns objtab
    long              (*bite)(void *);                  // obj
    long              (*bargain)(void *, void *, long); // obj, chr, price
    long              (*pay)(void *, long);             // obj, price or 0
    long              (*use)(void *, void *, long);     // obj, dest, parm/flg
    long              (*eat)(void *);                   // obj
    long              (*drink)(void *);                 // obj
    long              (*hit)(void *, void *);           // obj, dest
    long              (*defend)(void *, void *);        // obj, dest
    long              (*pick)(void *);                  // loc, returns objtab
    long              (*menu)(void *);                  // objtab
    long              (*drop)(void *, void *);          // obj, loc
};

struct mjolobjparm {
    long                sym;        // ASCII-presentation/type
    long                ntotal;     // # of objects created
    long                nmin;       // minimum number of objects per game
    long                nmax;       // maximum number of objects
    long                minlvl;     // minimum level to create
    long                maxlvl;     // maxiimum level to create
    long                problim;    // for upper probability bound
    long                probsft;    // for constructing rand-masks
};

union mjolobjdata {
    struct mjolchr      chr;
    struct mjolitem     item;
};

struct mjolobj {
    struct mjolobj     *prev;
    struct mjolobj     *next;
    long                type;
    union mjolobjdata   data;
};

#endif /* __MJOLNIR_OBJ_H__ */

