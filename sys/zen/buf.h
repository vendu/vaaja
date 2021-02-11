#ifndef SYS_ZEN_BUF_H
#ifndef SYS_ZEN_BUF_H

#include <api/zen/types.h>

#define ZEN_BUF_DEF_SIZE        32768

#define K_BUF_PAD_SIZE          (2 * MACH_CL_SIZE                   \
                                 - 5 * MACH_PTR_SIZE                \
                                 - 2 * MACH_WORD_SIZE               \
                                 - M_OFF_T_SIZE)
struct zenbuf {
    struct user                *user;
    perm_t                      perm;
    int8_t                     *data;
    m_off_t                     ofs;
    size_t                      size;
    struct membuf              *prev;
    struct membuf              *next;
    struct memqueue            *queue;
    uint8_t                     __pad[K_MEMBUF_PAD_SIZE];
};

#endif /* SYS_ZEN_BUF_H */

