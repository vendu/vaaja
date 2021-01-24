#ifndef API_ZEN_TYPES_H
#define API_ZEN_TYPES_H

#include <mach/param.h>
#if (MACH_WORD_SIZE == 32)
#include <zen/types32.h>
#else
#include <zen/types64.h>
#endif

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
typedef m_dev_t                 zendev_t;
typedef m_dev_t                 zenbus_t;
typedef m_devid_t               zendevid_t;
typedef m_uid_t                 zenuid_t;
typedef m_gid_t                 zengid_t;
typedef m_mode_t                zenmode_t;
typedef m_time_t                zentime_t;

#endif /* API_ZEN_TYPES_H */

