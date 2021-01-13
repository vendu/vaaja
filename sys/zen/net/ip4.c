#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
//#include <zero/param.h>
#include <env/trix.h>
#include <sys/zen/net/ip4.h>
#if !defined(NET_IP4_TEST)
#define NET_IaP4_TEST    0
#else
#define NET_IP4_NPKT    4096
#endif
#if (NET_IP4_TEST)
#include <stdio.h>
#include <string.h>
#include <zero/prof.h>
#include <zero/randmt32.h>
#endif

/*
 * This code is based on what I found at
 * http://locklessinc.com/articles/tcp_checksum/
 */

#if (MACH_WORD_SIZE == 4)
#define ip4chksum(buf, sz) ip4chksum16(buf, sz)
#elif (MACH_WORD_SIZE == 8)
#define ip4chksum(buf, sz) ip4chksum64(buf, sz)
#endif

#if (MACH_WORD_SIZE == 4) || (NET_IP4_TEST)

uint_fast16_t
ip4chksum16(const uint8_t *buf, size_t size)
{
    uint32_t sum = 0;
    uint16_t ndx;

    size = min(size, NET_IP4_PKT_MAX);
    /* Accumulate checksum */
    for (ndx = 0; ndx < size - 1; ndx += 2) {
        uint16_t word16 = *(uint16_t *)&buf[ndx];

        sum += word16;
    }

    /* Handle odd-sized case */
    if (size & 1) {
        uint16_t word16 = (uint8_t)buf[ndx];

        sum += word16;
    }

    /* Fold to get the ones-complement result */
    while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);

    /* Invert to get the negative in ones-complement arithmetic */
    return ~sum;
}

#endif /* MACH_WORD_SIZE == 4 || NET_IP4_TEST */

#if (MACH_WORD_SIZE == 8) || (NET_IP4_TEST)

uint_fast16_t
ip4chksum64(const uint8_t *buf, size_t size)
{
    uint64_t        sum = 0;
    const uint64_t *ptr = (uint64_t *)buf;
    size_t          len = size >> 3;
    uint32_t        tmp1;
    uint32_t        tmp2;
    uint16_t        tmp3;
    uint16_t        tmp4;

    size -= len << 3;
    while (len--) {
        uint64_t u = *ptr;

        sum += u;
        ptr++;
        if (sum < u) {
            sum++;
        }
    }

    /* Handle tail less than 8-bytes long */
    buf = (const uint8_t *)ptr;
    if (size & 4) {
        uint32_t u = *(uint32_t *)buf;

        sum += u;
        buf += sizeof(uint32_t);
        if (sum < u) {
            sum++;
        }
    }

    if (size & 2) {
        uint16_t u = *(uint16_t *)buf;

        sum += u;
        buf += sizeof(uint16_t);
        if (sum < u) {
            sum++;
        }
    }

    if (size) {
        uint8_t u = *(uint8_t *)buf;

        sum += u;
        if (sum < u) sum++;
    }

    /* Fold down to 16 bits */
    tmp1 = sum;
    tmp2 = sum >> 32;
    tmp1 += tmp2;
    if (tmp1 < tmp2) {
        tmp1++;
    }
    tmp3 = tmp1;
    tmp4 = tmp1 >> 16;
    tmp3 += tmp4;
    if (tmp3 < tmp4) {
        tmp3++;
    }

    return ~tmp3;
}

uint_fast16_t
ip4chksum64_2(const uint8_t *buf, size_t size)
{
    uint64_t        sum = 0;
    const uint64_t *ptr = (uint64_t *)buf;
    size_t          len = size >> 3;
    uint32_t        tmp1;
    uint32_t        tmp2;
    uint16_t        tmp3;
    uint16_t        tmp4;

    size -= len << 3;
    while (len--) {
        uint64_t u = *ptr;

        sum += u;
        ptr++;
        if (sum >= u) {

            continue;
        }
        sum++;
    }

    buf = (const uint8_t *)ptr;
    if (size & 4) {
        uint32_t u = *(uint32_t *)buf;

        sum += u;
        buf += sizeof(uint32_t);
        if (sum < u) {
            sum++;
        }
    }
    if (size & 2) {
        uint16_t u = *(uint16_t *)buf;

        sum += u;
        buf += sizeof(uint16_t);
        if (sum < u) {
            sum++;
        }
    }
    if (size) {
        uint8_t u = *(uint8_t *)buf;

        sum += u;
        if (sum < u) sum++;
    }
    tmp1 = sum;
    tmp2 = sum >> 32;
    tmp1 += tmp2;
    if (tmp1 < tmp2) {
        tmp1++;
    }
    tmp3 = tmp1;
    tmp4 = tmp1 >> 16;
    tmp3 += tmp4;
    if (tmp3 < tmp4) {
        tmp3++;
    }

    return ~tmp3;
}

#endif /* MACH_WORD_SIZE == 8 || NET_IP4_TEST */

#if (NET_IP4_TEST)

int
main(int argc, char *argv[])
{
    uint64_t           *pkttab[NET_IP4_NPKT];
    size_t              lentab[NET_IP4_NPKT];
    uint16_t            chk1tab[NET_IP4_NPKT];
    uint16_t            chk2tab[NET_IP4_NPKT];
    unsigned long long  nbyte = 0;
    uint8_t            *ptr;
    long                nus;
    long                l;
    long                n;
    uint16_t            len;
    PROFDECLCLK(clk);

    srandmt32(666);
    fprintf(stderr, "NET_IP4_test, %d packets\n", NET_IP4_NPKT);
    for (l = 0 ; l < NET_IP4_NPKT ; l++) {
b        //        fprintf(stderr, "%ld\n", l);
        len = (randmt32() & (NET_IP4_PKT_MAX - 1)) + 1;
        lentab[l] = len;
        nbyte += len;
//        pkttab[l] = calloc(rounduppow2(len, sizeof(uint64_t)) / sizeof(uint64_t), sizeof(uint64_t));
        pkttab[l] = malloc(rounduppow2(len, sizeof(uint64_t)) * sizeof
                           (uint64_t));
        //        pkttab[l] = malloc(len);
        ptr = (uint8_t *)pkttab[l];
        for (n = 0 ; n < len ; n++) {
            ptr[n] = randmt32() % 0xff;
        }
    }
#if 0
    nbyte = 0;
    for (l = 0 ; l < NET_IP4_NPKT ; l++) {
        nbyte += lentab[l];
    }
#endif
    fprintf(stderr, "allocated %llu bytes\n", nbyte);
    profstartclk(clk);
    for (l = 0 ; l < NET_IP4_NPKT ; l++) {
        //        profstartclk(clk);
        chk1tab[l] = ip4chksum64((const uint8_t *)pkttab[l], lentab[l]);
        //        profstopclk(clk);
#if 0
        nus = profclkdiff(clk);
        fprintf(stderr, "%ld microseconds for %ld bytes - %lfMBps\n",
                nus, (long)lentab[l], lentab[l] / (1000000000.0 * nus));
#endif
    }
    profstopclk(clk);
    fprintf(stderr, "%ld microseconds\n", profclkdiff(clk));
    fprintf(stderr, "%f MBps\n", (1000000.0 * ((double)nbyte / (double)profclkdiff(clk))) / 1048576.0);
    profstartclk(clk);
    for (l = 0 ; l < NET_IP4_NPKT ; l++) {
        chk2tab[l] = ip4chksum64_2((const uint8_t *)pkttab[l], lentab[l]);
    }
    profstopclk(clk);
    fprintf(stderr, "%ld microseconds\n", profclkdiff(clk));
#if 0
    memcpy(chk2tab, chk1tab, sizeof(uint16_t) * NET_IP4_NPKT);
#endif
    for (l = 0 ; l < NET_IP4_NPKT ; l++) {
        if (chk1tab[l] != chk2tab[l]) {
            fprintf(stderr, "%ld\n", l);

            exit(1);
        }
    }

    return 0;
}

#endif

