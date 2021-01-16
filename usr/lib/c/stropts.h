#ifndef __STROPTS_H__
#define __STROPTS_H__

typedef int32_t         t_scalar_t;
typedef uint32_t        t_uscalar_t;

#if !defined(__uid_t_defined)
typedef uint32_t        uid_t;
#define __uid_t_defined 1
#endif
#if !defined(__gid_t_defined)
typedef uint32_t        gid_t;
#define __gid_t_defined 1
#endif

#define I_ATMARK        0x01        // top message marked?
#define I_CANPUT        0x02        // band writable?
#define I_CKBAND        0x03        // check band for messages
#define I_FDINSERT      0x04        // send implementation-defined STREAM info
#define I_FIND          0x05        // look for a STREAMS module

struct bandinfo {
    int                 bi_flag;    // flushing type
    int                 bi_pri;     // priority band
};

struct strbuf {
    char               *buf;
    int                 len;
    int                 maxlen;
};

struct strpeek {
    struct strbuf       ctlbuf;     // control part of message
    struct strbuf       databuf;    // message data
    t_uscalar_t         flags;      // RS_HIPRI or 0
};

struct strfdinsert {
    struct strbuf       ctlbuf;     // control part of message
    struct strbuf       databuf;    // message data
    int                 fd;         // file descriptor of the other STREAM
    t_uscalar_t         flags;      // RS_HIPRI or 0
    int                 offset;     // relative location of stored value
};

struct strioctl {
    int                 ic_cmd;     // ioctl() command
    char               *ic_dp;      // buffer pointer
    int                 ic_len;     // date length
    int                 ic_timout;  // timeout for response
};

struct strrecvfd {
    int                 fd;         // received file descriptor
    gid_t               gid;        // sander group ID
    uid_t               uid;        // sender user ID
};

struct str_mlist {
    char                l_name[FMNAMESZ + 1];
};

struct str_list {
    struct str_mlist    sl_modlist; // STREAMS module names
    int                 sl_nmods;   // number of module names
};

#endif /* __STROPTS_H__ */

