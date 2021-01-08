#ifndef __SYS_ZEN_VAR_H__
#define __SYS_ZEN_VAR_H__

#include <stddef.h>
#include <stdint.h>

#define ZEN_VAR_INVAL           (-1)
#define ZEN_VAR_HIRES_TMR       offsetof(struct zenvars, hirestmr)
#define ZEN_VAR_RTC_TIME        offsetof(struct zenvars, rtctime)
#define ZEN_VAR_TMR_HZ          offsetof(struct zenvars, tmrhz)
#define ZEN_VAR_OPEN_MAX        offsetof(struct zenvars, hiopenmax)
#define ZEN_VAR_STREAM_MAX      offsetof(struct zenvars, streammax)
#define ZEN_VAR_WORD_SIZE       offsetof(struct zenvars, wordsize)
#define ZEN_VAR_ADR_BITS        offsetof(struct zenvars, adrbits)
#define ZEN_VAR_CL_SIZE         offsetof(struct zenvars, clsize)
#define ZEN_VAR_BANK_SIZE       offsetof(struct zenvars, banksize)
#define ZEN_VAR_PAGE_SIZE       offsetof(struct zenvars, pagesize)
#define ZEN_VAR_SHM_MAX         offsetof(struct zenvars, shmmax)
#define ZEN_VAR_EVQ_MAX         offsetof(struct zenvars, evqmax)

struct zenvars {
    m_time_t                   *hirestmr;
    zentime_t                  *rtctime;
    int16_t                     tmrhz;
    int16_t                     openmax;
    int16_t                     streammax;
    int16_t                     wordsize;
    int16_t                     adrbits;
    int16_t                     clsize;
    int16_t                     banksize;
    int16_t                     pagesize;
    int16_t                     shmmax;
    int16_t                     evqmax;
};

#endif /* __SYS_ZEN_VAR_H__ */

