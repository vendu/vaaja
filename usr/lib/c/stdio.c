#include <stddef.h>
#include <stdio.h>
#include <mach/param.h>

char                            term[L_ctermid] C_ALIGNED(MACH_PAGE_SIZE);
FILE                           *stdin;
FILE                           *stdout;
FILE                           *stderr;

