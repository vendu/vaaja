#ifndef __SYS_ZEN_VAR_H__
#define __SYS_ZEN_VAR_H__

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
    m_word_t                   *hirestmr;
    m_time_t                   *rtctime;
    m_half_t                    tmrhz;
    m_half_t                    openmax;
    m_half_t                    streammax;
    m_half_t                    wordsize;
    m_half_t                    adrbits;
    m_half_t                    clsize;
    m_half_t                    banksize;
    m_half_t                    pagesize;
    m_half_t                    shmmax;
    m_half_t                    evqmax;
};

#endif /* __SYS_ZEN_VAR_H__ */

