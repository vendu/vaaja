#include <stdlib.h>
#include <string.h>
#include <corewar/cw.h>

#define CW_SYM_STRLEN   8
#define CW_SYM_STRSIZE  16

struct cwsymtab         g_symtab[128] C_ALIGNED(MACH_PAGE_SIZE);

void
cwaddsym(char *name, long adr)
{
    long                ndx = name[0];
    struct cwsymtab    *symtab = &g_symtab[ndx];
    long                nsym = symtab->nsym;
    struct cwsymtab    *prev;

    fprintf(stderr, "ADD: %s (%lx)\n", name, adr);
    while ((symtab) && nsym == 7) {
        prev = symtab;
        symtab = symtab->next;
        nsym = symtab->nsym;
    }
    if (nsym == 7) {
        symtab = calloc(1, sizeof(struct cwsymtab));
        nsym = 0;
        if (!symtab) {
            fprintf(stderr, "failure to allocate a symbol-table\n");

            exit(1);
        }
        prev->next = symtab;
    }
    nsym++;
    symtab->nsym = nsym;
    symtab->tab[nsym].name = name;
    symtab->tab[nsym].adr = adr;

    return;
}

long
cwfindsym(char *name)
{
    long                ndx = name[0];
    struct cwsymtab    *symtab = &g_symtab[ndx];

    while (symtab) {
        for (ndx = 0 ; ndx < 7 ; ndx++) {
            while (symtab->tab[ndx].name) {
                fprintf(stderr, "%s (%s)\n", symtab->tab[ndx].name, name);
                if (!strcasecmp(symtab->tab[ndx].name, name)) {

                    return symtab->tab[ndx].adr;
                }
            }
        }
        symtab = symtab->next;
    }

    return -1;
}

