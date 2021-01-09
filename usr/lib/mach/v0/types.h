#ifndef V0_TYPES_H
#define V0_TYPES_H

typedef int32_t                 m_reg_t;
typedef uint32_t                m_ureg_t;
typedef int64_t                 m_long_t;
typedef uint64_t                m_ulong_t;
typedef int32_t                 m_word_t;
typedef uint32_t                m_uword_t;
typedef int16_t                 m_half_t;
typedef uint16_t                m_uhalf_t;
typedef int8_t                  m_byte_t;
typedef uint8_t                 m_ubyte_t;
typedef uint32_t                m_adr_t;
typedef void                   *m_ptr_t;
typedef int64_t                 m_long_t;
typedef uint64_t                m_ulong_t;

/* sized types */
typedef int32_t                 m_id_t;
typedef int32_t                 m_dev_t;    // device type
typedef int32_t                 m_blk_t;     // block ID
typedef int32_t                 m_bus_t;     // bus ID
typedef int32_t                 m_devid_t;   // device ID
typedef int32_t                 m_pid_t;     // process ID
typedef int32_t                 m_uid_t;     // user ID
typedef int32_t                 m_gid_t;     // group ID
typedef int32_t                 m_mode_t;    // file access mode
typedef int32_t                 m_perm_t;    // I/O permission flags
typedef int32_t                 m_time_t;    // FIXME: wraps around in 2038 :)
/* system types */
typedef uint32_t                m_size_t;    // size for memory regions
typedef int32_t                 m_ssize_t;   // signed size for I/O
typedef int32_t                 m_off_t;     // file-system offset, constrained

typedef m_dev_t                 zendev_t;
typedef m_dev_t                 zenbus_t;
typedef m_devid_t               zendevid_t;
typedef m_uid_t                 zenuid_t;
typedef m_gid_t                 zengid_t;
typedef m_mode_t                zenmode_t;
typedef m_time_t                zentime_t;

#endif /* V0_TYPES_H */

