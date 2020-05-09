/*
 * THANKS
 * ------
 * - Matthew 'kinetik' Gregan for suggested fixes
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zero/unix.h>

/* initialize 'daemon mode' for server processes */
int
daemonize(char *rootpath)
{
    int   maxfd;
    int   tmpfd;
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        /* fork() failed */

        return -1;
    } else if (pid) {
        /* PARENT PROCESS */

        exit(0);
    }

    /* CHILD PROCESS */

    /* create new session */
    setsid();
    /* clear environment */
    clearenv();
    /* fork again */
    pid = fork();
    if (pid < 0) {
        /* fork failed */

        return -1;
    } else if (pid) {
        /* PARENT PROCESS */

        exit(0);
    }

    /* CHILD PROCESS */

    /* change current working directory */
    if (chdir(rootpath) < 0) {

        return -1;
    }
    /* change root directory; sandbox server process */
    if (chroot(rootpath) < 0) {

        return -1;
    }
    /* mask all but user and group read-permissions */
    umask(0337);
    /* query maximum number of open files */
    maxfd = get_open_max();
    /* close all open descriptors */
    for (tmpfd = 0 ; tmpfd < maxfd ; tmpfd++) {
	close(tmpfd);
    }
    /* set stdin, stdout, and stderr to /dev/null */
    tmpfd = open("/dev/null", O_RDWR, 0);
    if (tmpfd >= 0) {
#if defined(STDIN_FILENO)
	dup2(tmpfd, STDIN_FILENO);
	dup2(tmpfd, STDOUT_FILENO);
	dup2(tmpfd, STDERR_FILENO);
#else
	dup2(tmpfd, 0);
	dup2(tmpfd, 1);
	dup2(tmpfd, 2);
#endif
    }
    /* close descriptor for "/dev/null if not stdin, stdout, or stderr */
    if (tmpfd > 2) {
        close(tmpfd);
    }

    return 0;
}

