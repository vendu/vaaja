#ifndef API_ZEN_TYPES32_H
#define API_ZEN_TYPES32_H

/* system types */
typedef uint32_t                m_size_t;    // size for memory regions
typedef int32_t                 m_ssize_t;   // signed size for I/O

#if (defined(__amd64__)\
     || defined(__x86_64__)                                             \
     || defined(__i386__)                                               \
     || defined(__i486__)                                               \
     || defined(__i586__)                                               \
     || defined(__i686__))
typedef int64_t                 m_time_t;    // FIXME: wraps around in 2038 :)
typedef int64_t                 m_off_t;     // file-system offset, constrained
#else
typedef int32_t                 m_time_t;    // FIXME: wraps around in 2038 :)
typedef int32_t                 m_off_t;     // file-system offset, constrained
#endif

#endif /* API_ZEN_TYPES64_H */

