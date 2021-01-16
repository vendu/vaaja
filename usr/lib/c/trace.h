#ifndef __TRACE_H__
#define __TRACE_H__

#include <stdint.h>
#include <sys/types.h>

#define POSIX_TRACE_ALL_EVENTS         (~UINT64_C(0))
#define POSIX_TRACE_APPEND             (UINT64_C(1) << 0)
#define POSIX_TRACE_CLOSE_FOR_CHILD    (UINT64_C(1) << 1)
#define POSIX_TRACE_FILTER             (UINT64_C(1) << 2)
#define POSIX_TRACE_FLUSH              (UINT64_C(1) << 3)
#define POSIX_TRACE_FLUSH_START        (UINT64_C(1) << 4)
#define POSIX_TRACE_FLUSH_STOP         (UINT64_C(1) << 5)
#define POSIX_TRACE_FLUSHING           (UINT64_C(1) << 6)
#define POSIX_TRACE_FULL               (UINT64_C(1) << 7)
#define POSIX_TRACE_LOOP               (UINT64_C(1) << 8)
#define POSIX_TRACE_NO_OVERRUN         (UINT64_C(1) << 9)
#define POSIX_TRACE_NOT_FLUSHING       (UINT64_C(1) << 10)
#define POSIX_TRACE_NOT_FULL           (UINT64_C(1) << 11)
#define POSIX_TRACE_INHERITED          (UINT64_C(1) << 12)
#define POSIX_TRACE_NOT_TRUNCATED      (UINT64_C(1) << 13)
#define POSIX_TRACE_OVERFLOW           (UINT64_C(1) << 14)
#define POSIX_TRACE_OVERRUN            (UINT64_C(1) << 15)
#define POSIX_TRACE_RESUME             (UINT64_C(1) << 16)
#define POSIX_TRACE_RUNNING            (UINT64_C(1) << 17)
#define POSIX_TRACE_START              (UINT64_C(1) << 18)
#define POSIX_TRACE_STOP               (UINT64_C(1) << 19)
#define POSIX_TRACE_SUSPENDED          (UINT64_C(1) << 20)
#define POSIX_TRACE_SYSTEM_EVENTS      (UINT64_C(1) << 21)
#define POSIX_TRACE_TRUNCATED_READ     (UINT64_C(1) << 22)
#define POSIX_TRACE_TRUNCATED_RECORD   (UINT64_C(1) << 23)
#define POSIX_TRACE_UNNAMED_USER_EVENT (UINT64_C(1) << 24)
#define POSIX_TRACE_UNTIL_FULL         (UINT64_C(1) << 25)
#define POSIX_TRACE_WOPID_EVENTS       (UINT64_C(1) << 26)

struct posix_trace_event_info {
    struct timespec   posix_timestamp;
    pthread_t         posix_thread_id;
    void             *posix_prog_address;
    trace_event_id_t  posix_event_id;
    pid_t             posix_pid;
    int               posix_truncation_status;
};

struct posix_trace_status_info {
    int posix_stream_status;
    int posix_stream_full_status;
    int posix_stream_overrun_status;
    int posix_stream_flush_status;
    int posix_stream_flush_error;
    int posix_log_overrun_status;
    int posix_log_full_status;
};

#endif /* __TRACE_H__ */

