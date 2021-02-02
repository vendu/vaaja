#ifndef SYS_ZEN_BUF_H
#ifndef SYS_ZEN_BUF_H

#define ZEN_BUF_DEF_SIZE    32768

struct membuf {
    struct user                *user;
    perm_t                      perm;
    vint8_t                    *data;
    m_off_t                     ofs;
    aize_t                      size;
    struct membuf              *prev;
    struct membuf              *next;
    struct memqueue            *queue;
    uint8_t                     __pad[K_MEMBUF_PAD_SIZE];
};

#endif /* SYS_ZEN_BUF_H */

