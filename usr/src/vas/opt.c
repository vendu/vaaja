/* zero assembler configuration and command-line options */

#include <vas/conf.h>
#include <vas/vas.h>
#include <vas/opt.h>

#if (VASALIGN)
extern vasmemadr_t vastokalntab[VASNTOKEN];
#endif

#if (VASALIGN)
void
vasinitalign(void)
{
    vasmemadr_t aln = sizeof(vasword_t);

    vastokalntab[VASTOKENVALUE] = aln;
    vastokalntab[VASTOKENLABEL] = aln;
    vastokalntab[VASTOKENINST] = aln;
    vastokalntab[VASTOKENSYM] = aln;
    vastokalntab[VASTOKENDATA] = aln;
    vastokalntab[VASTOKENSPACE] = aln;

    return;
}
#endif

