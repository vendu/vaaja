#include <stdlib.h>
#include <unistd.h>
#include <vt/vt.h>

int
vtspawncmd(struct vt *vt)
{
    int   retval = 0;
//    pid_t pid;
    /*
     * set input field separator for shell to TAB for old shells that don't
     * reset it. This is for security reasons to prevent users from setting
     * IFS to make system() execute a different program.
     */
    putenv("IFS= \t");
    /* export terminal type */
    putenv("TERM=vt100");

    return retval;
}
