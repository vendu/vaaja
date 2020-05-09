#ifndef __ZEN_NET_IP4_H__
#define __ZEN_NET_IP4_H__

struct zennetudp4dgram {
    unsigned            version : 4;
    unsigned            hdrlen  : 4;
    uint8_t             tos;
    uint16_t            len;
    uint16_t            ident;
    unsigned            fragflg : 2;
    unsigned            _res    : 1;
    unsigned            fragofs : 13;
    uint8_t             ttl;
    uint8_t             protocol;
    uint16_t            hdrcrc;
    uint32_t            srcadr;
    uint32_t            destadr;
    uint8_t             data[VLA];
};

struct zennettcp4pkt {
    uint16_t            srcport;
    uint16_t            destport;
    uint32_t            seqnum;
    uint32_t            acknum;
    unsigned            dataofs : 4;
    unsigned            _res    : 6;
    unsigned            ctlflg  : 6;
    uint16_t            winsize;
    uint16_t            crc;
    uint16_t            urgptr;
    uint8_t             data[VLA];
};

#endif /* __ZEN_NET_IP4_H__ */

