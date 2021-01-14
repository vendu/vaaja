#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#if !defined(EAGAIN)
#define EAGAIN EWOULDBLOCK
#endif

/* TODO: zmapfile() */

/*
 * read a whole file into memory
 * - deal with interrupted system calls (errno == EINTR)
 * - return address of dynamically allocated buffer
 *   - needs to be freed explicitly after use
 *   - return number of bytes read in sizeret
 */
void *
zreadfile(char *filename, size_t *sizeret)
{
    void        *buf = NULL;
    struct stat  statbuf;
    size_t       nleft;
    ssize_t      res;
    int          fd;

    if (sizeret) {
        *sizeret = 0;
    }
    if (stat(filename, &statbuf) < 0) {

        return NULL;
    }
    if (!S_ISREG(statbuf.st_mode)) {

        return NULL;
    }
    fd = open(filename, O_RDONLY);
    if (fd < 0) {

        return NULL;
    }
    nleft = statbuf.st_size;
    if (nleft) {
        buf = malloc(nleft);
        if (buf) {
            while (nleft) {
                res = read(fd, buf, nleft);
                if (res < 0) {
                    if (errno == EINTR) {

                        continue;
                    } else {
                        free(buf);

                        return NULL;
                    }
                } else {
                    nleft -= res;
                }
            }
        }
    }
    close(fd);
    if (sizeret) {
        *sizeret = statbuf.st_size;
    }

    return buf;
}

/*
 * write nb bytes from buf into file
 * - deal with interrupted system calls (errno == EINTR)
 * - return 0 on success, -1 on failure
 */
int
zwritefile(char *filename, void *buf, size_t nb, size_t *sizeret)
{
    size_t  nleft;
    ssize_t res;
    int     fd;

    fd = open(filename, O_WRONLY | O_TRUNC);
    if (fd < 0) {
        fprintf(stderr, "failed to create %s\n", filename);

        return -1;
    }
    nleft = nb;
    if (nleft) {
        buf = malloc(nleft);
        if (buf) {
            while (nleft) {
                res = write(fd, buf, nleft);
                if (res < 0) {
                    if (errno == EINTR) {

                        continue;
                    } else {
                        free(buf);

                        return -1;
                    }
                } else {
                    nleft -= res;
                }
            }
        }
    }
    close(fd);
    if (sizeret) {
        *sizeret = nb;
    }

    return 0;
}

/* read in blocking mode */
ssize_t
zread(int fd, void *buf, size_t nb)
{
    char    *cptr = buf;
    size_t   len;
    ssize_t  res;

    len = 0;
    while (nb) {
        errno = 0;
        res = read(fd, cptr, nb);
        if (res <= 0) {
            if (errno == EINTR) {

                continue;
            } else if (len == 0) {

                return -1;
            } else {

                return len;
            }
        }
        nb -= res;
        len += res;
        cptr += res;
    }

    return len;
}

/* read in non-blocking mode */
ssize_t
zreadnb(int fd, void *buf, size_t nb)
{
    char    *cptr = buf;
    size_t   len;
    ssize_t  res;

    len = 0;
    while (nb) {
        errno = 0;
        res = read(fd, cptr, nb);
        if (res <= 0) {
            if (errno == EINTR) {

                continue;
            } else if (errno == EAGAIN) {

                return len;
            } else {

                return -1;
            }
        }
        nb -= res;
        len += res;
        cptr += res;
    }

    return len;
}

/* write in blocking mode */
ssize_t
zwrite(int fd, void *buf, size_t nb)
{
    char    *cptr = buf;
    size_t   len;
    ssize_t  res;

    len = 0;
    while (nb) {
        errno = 0;
        res = write(fd, cptr, nb);
        if (res <= 0) {
            if (errno == EINTR) {

                continue;
            } else {

                return len;
            }
        }
        nb -= res;
        len += res;
        cptr += res;
    }

    return len;
}

/* write in non-blocking mode */
ssize_t
zwritenb(int fd, void *buf, size_t nb)
{
    char    *cptr = buf;
    size_t   len;
    ssize_t  res;

    len = 0;
    while (nb) {
        errno = 0;
        res = write(fd, cptr, nb);
        if (res <= 0) {
            if (errno == EINTR) {

                continue;
            } else if (errno == EAGAIN) {

                return len;
            } else {

                return -1;
            }
        }
        nb -= res;
        len += res;
    	cptr += res;
    }

    return len;
}

