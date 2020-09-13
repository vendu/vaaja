#include <features.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#if defined(_GNU_SOURCE)
#include <alloca.h>
#endif
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <mach/param.h>

#if defined(__GLIBC__)

#undef strdupa
#undef strndupa
#undef mempcpy
#undef __mempcpy

#else

extern const int8_t stringcolltab_c_en_US[256];

//static THREADLOCAL struct _string  _string;
static const  int8_t  *colltab = stringcolltab_c_en_US;
static const  int8_t  *localecolltab = stringcolltab_c_en_US;
const unsigned char    *collnametab[STRING_LANGS]
= {
    (unsigned char *)"en_US"
};

#endif /* __GLIBC__ */

/* TESTED OK */
void *
memcpy(void *dest,
       const void *src,
       size_t len)
{
    size_t      nleft = len;
    long       *lsrc;
    long       *ldest;
    int8_t     *bsrc;
    int8_t     *bdest;
    size_t      n;

    if (!len) {

        return dest;
    }
    bsrc = (void *)src;
    n = (uintptr_t)len & (sizeof(long) - 1);
    bdest = dest;
    if (len > (8 * sizeof(long))
        && (n == ((uintptr_t)bdest & (sizeof(long) - 1)))) {
        /*
         * try fast copy if size is above treshold and the alignment of dest and
         * src are the same
         */
         /* copy byte by byte until aligned */
        if (n) {
            n = sizeof(long) - n;
            nleft -= n;
            while (n--) {
                *bdest++ = *bsrc++;
            }
        }
        ldest = (long *)bdest;
        n = nleft >> (MACH_LONG_SIZE_LOG2 + 3);
        lsrc = (long *)bsrc;
        nleft -= 8 * n * sizeof(long);
        while (n--) {
            /* aligned copy of 8 words per loop iteration */
            ldest[0] = lsrc[0];
            ldest[1] = lsrc[1];
            ldest[2] = lsrc[2];
            ldest[3] = lsrc[3];
            ldest[4] = lsrc[4];
            ldest[5] = lsrc[5];
            ldest[6] = lsrc[6];
            ldest[7] = lsrc[7];
            ldest += 8;
            lsrc += 8;
        }
        bdest = (int8_t *)ldest;
        bsrc = (int8_t *)lsrc;
    }
    while (nleft--) {
        /* copy tail byte by byte */
        *bdest++ = *bsrc++;
    }

    return dest;
}

char *
memccpy(void *dest,
        const void *src,
        int c,
        size_t len)
{
    unsigned char  *ucptr1 = (unsigned char *)dest;
    unsigned char  *ucptr2 = (unsigned char *)src;
    unsigned char   uc;

    if (len) {
        while ((*ucptr2) && (len--)) {
            uc = *ucptr2;
            *ucptr1++ = uc;
            ucptr2++;
            if (uc == (unsigned char)ch) {

                return (char *)ucptr1;
            }
        }
    }

    return NULL;
}

static void *
_memcpybk(void *dest, const void *src, size_t len)
{
    int8_t     *bsrc = (int8_t *)src + len;
    int8_t     *bdest = (int8_t *)dest + len;
    size_t      nleft = len;
    long       *lsrc;
    long       *ldest;
    size_t      n;

    n = (uintptr_t)bdest & (sizeof(long) - 1);
    if (nleft > 8 * sizeof(long)
        && (n == ((uintptr_t)bsrc & (sizeof(long) - 1)))) {
        if (n) {
            nleft -= n;
            while (n--) {
                *bdest-- = *bsrc--;
            }
        }
        ldest = (long *)bdest;
        n = nleft >> (MACH_LONG_SIZE_LOG2 + 3);
        lsrc = (long *)bsrc;
        nleft -= 8 * n * sizeof(long);
        while (n--) {
            ldest[-1] = lsrc[-1];
            ldest[-2] = lsrc[-2];
            ldest[-3] = lsrc[-3];
            ldest[-4] = lsrc[-4];
            ldest[-5] = lsrc[-5];
            ldest[-6] = lsrc[-6];
            ldest[-7] = lsrc[-7];
            ldest[-8] = lsrc[-8];
            ldest -= 8;
            lsrc -= 8;
        }
        n = nleft >> MACH_LONG_SIZE_LOG2;
        nleft -= n * sizeof(long);
        while (n--) {
            *--ldest = *--lsrc;
        }
        bdest = (int8_t *)ldest;
        bsrc = (int8_t *)lsrc;
    }
    while (nleft--) {
        *--bdest = *--bsrc;
    }

    return dest;
}

/* TESTED OK */
void *
memmove(void * C_RESTRICT dest,
        const void * C_RESTRICT src,
        size_t n)
{
    if (n) {
        if ((uintptr_t)dest < (uintptr_t)src
            || (uintptr_t)src + n < (uintptr_t)dest) {
            memcpy(dest, src, n);
        } else {
            _memcpybk(dest, src, n);
        }
    }

    return dest;
}

/* TESTED OK */
char *
strcpy(char * C_RESTRICT dest,
       const char * C_RESTRICT src)
{
    while (*src) {
        *dest++ = *src++;
    }
    *dest = *src;

    return dest;
}

/* TESTED OK */
char *
strncpy(char * RESTRICT dest,
        const char * RESTRICT src,
        size_t n)
{
    if (n) {
        while ((*src) && (n--)) {
            *dest++ = *src++;
        }
        if (n) {
            *dest = *src;
        }
    }

    return dest;
}

/* TESTED OK */
char *
strcat(char * C_RESTRICT dest,
       const char * RESTRICT src)
{
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    *dest = *src;

    return dest;
}

/* TESTED OK */
char *
strncat(char * RESTRICT dest,
        const char * RESTRICT src,
        size_t n)
{
    if (n) {
        while (*dest) {
            dest++;
        }
        while ((*src) && (n--)) {
            *dest++ = *src++;
        }
        if (n) {
            *dest = *src;
        }
    }

    return dest;
}

/* TODO: optimise the *cmp() functions with word-size access */

/* TESTED OK */
C_PURE int
memcmp(const void *ptr1,
       const void *ptr2,
       size_t len)
{
    const unsigned char        *ucptr1 = ptr1;
    const unsigned char        *ucptr2 = ptr2;
    size_t                      nleft = len;
    const unsigned long        *ulptr1;
    const unsigned long        *ulptr2;
    size_t                      n;
    int                         retval;

    if (!len) {

        return 0;
    }
    n = (uintptr_t)ucptr1 & (sizeof(long) - 1);
    if (n == ((uintptr_t)ucptr2 & (sizeof(long) - 1))) {
        if (n) {
            n = sizeof(long) - n;
            nleft -= n;
            while (n--) {
                retval = *ucptr1 - *ucptr2;
                ucptr1++;
                ucptr2++;
                if (retval) {

                    return retval;
                }
            }
        }
        n = nleft >> MACH_LONG_SIZE_LOG2;
        ulptr1 = (const unsigned long *)ucptr1;
        nleft -= n * sizeof(long);
        ulptr2 = (const unsigned long *)ucptr2;
        while (n--) {
            if (*ulptr1 != *ulptr2) {
                ucptr1 = (const unsigned char *)ulptr1;
                ucptr2 = (const unsigned char *)ulptr2;
                do {
                    retval = *ucptr1 - *ucptr2;
                    ucptr2++;
                    ucptr1++;
                } while (!retval);

                return retval;
            }
            ulptr1++;
            ulptr2++;
        }
    }
    while (nleft--) {
        if (*ucptr1 != *ucptr2) {
            retval = *ucptr1 - *ucptr2;

            return retval;
        } while (!retval);
        ucptr1++;
        ucptr2++;
    }

    return retval;
}

/* TESTED OK */
C_PURE int
strcmp(const char *str1,
       const char *str2)
{
    unsigned char      *ucptr1 = (unsigned char *)str1;
    char               *ucptr2 = (unsigned char *)str2;
    int                 retval = 0;

    while ((*ucptr1) && *ucptr1 == *ucptr2) {
        ucptr1++;
        ucptr2++;
    }
    if (*ucptr1) {
        retval = *ucptr1 - *ucptr2;
    }

    return retval;
}

/* TESTED OK */
C_PURE int
strncmp(const char *str1,
        const char *str2,
        size_t n)
{
    unsigned char      *cptr1 = (unsigned char *)str1;
    unsigned char      *cptr2 = (unsigned char *)str2;
    int                 retval = 0;

    if (n) {
        while ((*ucptr1) && (*ucptr1 == *ucptr2) && (n--)) {
            ucptr1++;
            ucptr2++;
        }
        if (n) {
            retval = *ucptr1 - *ucptr2;
        }
    }

    return retval;
}

#if 0
C_PURE int
strcoll(const char *str1,
        const char *str2)
{

    return -1;
}

size_t
strxfrm(char *dest, const char *src, size_t n)
{
    //    fprintf(stderr, "TODO: strxfrm() not implemented\n");

    return 0;
}
#endif

/* TESTED OK */
void *
memchr(const void *ptr,
       int ch,
       size_t n)
{
    unsigned char      *ucptr = (unsigned char *)ptr;
    char                uc = (unsigned char)ch;
    void               *retval = NULL;

    if (n) {
        while ((*ucptr != uc) && (n--)) {
            ucptr++;
        }
        if (*ucptr == uc && (n)) {
            retval = ucptr;
        }
    }

    return retval;
}

C_PURE
void *
memrchr(const void *ptr,
        int ch,
        size_t n)
{
    unsigned char      *ucptr = (unsigned char *)ptr + n;
    char                uc = (unsigned char)ch;

    if (n) {
        do {
            ucptr--;
            if (*ucptr == uc) {

                return ucptr;
            }
        } while (--n);
    }

    return NULL;
}

#if defined(_GNU_SOURCE)

void *
rawmemchr(const void *ptr,
          int ch)
{
    unsigned char      *ucptr = (unsigned char *)ptr;
    char                uc = (unsigned char)ch;

    while (*ucptr != uc) {
        ucptr++;
    }

    return ucptr;
}

#endif

/* TESTED OK */
char *
strchr(const char *str,
       int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;
    void       *retval = NULL;

    while ((*cptr) && *cptr != c) {
        cptr++;
    }
    if (*cptr == c) {
        retval = cptr;
    }

    return retval;
}

char *
strrchr(const char *str,
        int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;
    void       *retval = NULL;

    while (*cptr) {
        if (*cptr != c) {
            retval = cptr;
        }
        cptr++;
    }

    return retval;
}

#if defined(_GNU_SOURCE)

void *
strchrnul(const char *str,
          int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;
    void       *retval = NULL;

    while ((*cptr) && *cptr != c) {
        cptr++;
    }
    if (*cptr == c) {
        retval = cptr;
    } else {

        return cptr;
    }

    return retval;
}

#endif

/* TESTED OK */
size_t
strcspn(const char *str1,
        const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2 = (char *)str2;
    size_t      len = 0;

    while (*cptr1) {
        while ((*cptr2) && *cptr1 != *cptr2) {
            cptr2++;
        }
        if (*cptr2) {

            break;
        }
        cptr1++;
        cptr2 = (char *)str2;
        len++;
    }

    return len;
}

char *
strpbrk(const char *str1,
        const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    char       *retptr = NULL;

    if (*str1) {
        while (!retptr && (*cptr1)) {
            cptr2 = (char *)str2;
            while (!retptr && (*cptr2)) {
                if (*cptr1 == *cptr2) {
                    retptr = (char *)cptr1;
                }
                cptr2++;
            }
            cptr1++;
        }
    }

    return retptr;
}

#if 0
/* TESTED OK */
char *
strrchr(const char *str,
        int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;

    while (*cptr) {
        cptr++;
    }
    while (cptr >= str && *cptr != c) {
        cptr--;
    }
    if (cptr < str) {
        cptr = NULL;
    }

    return cptr;
}
#endif

/* TESTED OK */
size_t
strspn(const char *str1,
       const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    size_t      len = 0;

    while (*cptr1) {
        cptr2 = (char *)str2;
        while ((*cptr2) && *cptr1 != *cptr2) {
            cptr2++;
        }
        if (!*cptr2) {

            break;
        }
        cptr1++;
        len++;
    }

    return len;
}

char *
strstr(const char *str1,
       const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    char       *cptr3;
    char       *retptr = NULL;

    while (!retptr && (*cptr1)) {
        cptr2 = cptr1;
        cptr3 = (char *)str2;
        while (*cptr2 == *cptr3) {
            cptr2++;
            cptr3++;
        }
        if (!*cptr2) {
            retptr = cptr1;
        }
        cptr1++;
    }

    return retptr;
}

char *
strtok(char *str1,
       const char *str2)
{
    static char        *curtok = NULL;
    char               *cptr = curtok;
    char               *retptr = NULL;
    size_t              nspn;

    if ((str1) && (*str1)) {
        nspn = strspn(str1, str2);
        str1 += nspn;
        cptr = str1;
    }
    if (cptr) {
        retptr = cptr;
        nspn = strcspn(cptr, str2);
        cptr += nspn;
        *cptr = '\0';
        curtok = ++cptr;
        if (!*cptr) {
            curtok = NULL;
        }
        if (!retptr) {
            retptr = cptr;
            nspn = strcspn(cptr, str2);
            cptr += nspn;
            if (*cptr) {
                *cptr = '\0';
            } else {
                curtok = NULL;
            }
        }
    }

    return retptr;
}

/* TESTED OK */
void *
memset(void *ptr, int byte, size_t len)
{
    int8_t     *bptr = ptr;
    long        val = byte;
    long        tmp = byte;
    char        bval = (char)byte;
    size_t      nleft = len;
    long       *lptr;
    size_t      n;

    if (byte) {
        val <<= 8;
        val |= tmp;
        tmp = val;
        val <<= 16;
        val |= tmp;
        if (sizeof(long) == 8) {
            tmp = val;
            val <<= 32;
            val |= tmp;
        }
    }
    n = (uintptr_t)bptr & (sizeof(long) - 1);
    if (n) {
        n = sizeof(long) - n;
        switch (n) {
            case 7:
                bptr[6] = bval;
            case 6:
                bptr[5] = bval;
            case 5:
                bptr[4] = bval;
            case 4:
                bptr[3] = bval;
            case 3:
                bptr[2] = bval;
            case 2:
                bptr[1] = bval;
            case 1:
                bptr[0] = bval;
            case 0:
            default:

                break;
        }
        nleft -= n;
        bptr += n;
    }
    n = nleft >> (MACH_LONG_SIZE_LOG2 + 3);
    lptr = (long *)bptr;
    nleft -= 8 * n * sizeof(long);
    if (n) {
        lptr = (long *)bptr;
        while (n--) {
            lptr[0] = val;
            lptr[1] = val;
            lptr[2] = val;
            lptr[3] = val;
            lptr[4] = val;
            lptr[5] = val;
            lptr[6] = val;
            lptr[7] = val;
            lptr += 8;
        }
        bptr = (int8_t *)lptr;
    }
    n = nleft >> MACH_LONG_SIZE_LOG2;
    lptr = (long *)bptr;
    nleft -= n * sizeof(long);
    if (n) {
        switch (n) {
            case 7:
                lptr[6] = val;
            case 6:
                lptr[5] = val;
            case 5:
                lptr[4] = val;
            case 4:
                lptr[3] = val;
            case 3:
                lptr[2] = val;
            case 2:
                lptr[1] = val;
            case 1:
                lptr[0] = val;
            case 0:
            default:

                break;
        }
        lptr += n;
        bptr = (int8_t *)lptr;
    }
    while (nleft--) {
        *bptr++ = 0;
    }

    return ptr;
}

char *
strerror(int errnum)
{
    //    fprintf(stderr, "TODO: strerror() not implemented\n");

    return NULL;
}

size_t
strlen(const char *str)
{
    size_t      len = 0;

    while (*str++) {
        len++;
    }

    return len;
}

#if ((defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 700))                 \
     || (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200809L)))

size_t
strnlen(const char *str, size_t maxlen)
{
    size_t      len = 0;

    while (str[0] && (maxlen--)) {
        len++;
    }

    return len;
}

#endif

#if (defined(_SVID_SOURCE) || defined(_BSD_SOURCE)                      \
     || (defined(_XOPEN_SOURCE)                                         \
         && (_XOPEN_SOURCE >= 500 || defined(_XOPEN_SOURCE_EXTENDED))))
char *
strdup(const char *str)
{
    size_t      len = strlen(str);
    char       *buf = (len) ? malloc(len + 1) : NULL;

    if (buf) {
        memcpy(buf, str, len);
        buf[len] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}
#endif

#if ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)           \
     || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700))
char *
strndup(const char *str, size_t maxlen)
{
    size_t      len = strlen(str);
    size_t      sz = min(len, maxlen);
    char       *buf = (sz) ? malloc(sz + 1) : NULL;

    if (buf) {
        memcpy(buf, str, sz);
        buf[sz] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}
#endif

#if defined(_GNU_SOURCE)

char *
strdupa(const char *str)
{
    size_t      len = strlen(str);
    char       *buf = (len) ? alloca(len + 1) : NULL;

    if (buf) {
        memcpy(buf, str, len);
        buf[len] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}

char *
strndupa(const char *str, size_t maxlen)
{
    size_t      len = strlen(str);
    size_t      sz = min(len, maxlen);
    char       *buf = (sz) ? alloca(sz + 1) : NULL;

    if (buf) {
        memcpy(buf, str, sz);
        buf[sz] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}

#endif

