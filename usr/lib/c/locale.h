#ifndef __LOCALE_H__
#define __LOCALE_H__

#if (_ZERO_SOURCE)

#if !defined(NULL)
#include <share/null.h>
#endif

/* TODO */
#define LC_GLOBAL_LOCALE  (~0)
typedef int locale_t;

#define LC_ALL            0
#define LC_CTYPE          1
#define LC_NUMERIC        2
#define LC_TIME           3
#define LC_COLLATE        4
#define LC_MONETARY       5
#define LC_MESSAGES       6
#define LC_ALL            7
#define LC_PAPER          8
#define LC_NAME           9
#define LC_ADDRESS        10
#define LC_TELEPHONE      11
#define LC_MEASUREMENT    12
#define LC_IDENTIFICATION 13
#define _LC_LAST          14
#define LC_ALL_MASK       (~0)
#define LC_CTYPE_MASK     (1 << LC_CTYPE)
#define LC_NUMERIC_MASK   (1 << LC_NUMERIC)
#define LC_TIME_MASK      (1 << LC_TIME)
#define LC_COLLATE_MASK   (1 << LC_COLLATE)
#define LC_MONETARY_MASK  (1 << LC_CTYPE)
#define LC_MESSAGES_MASK  (1 << LC_MESSAGES)

struct lconv {
    char *currency_symbol;
    char *decimal_point;
    char  frac_digits;
    char *grouping;
    char *int_curr_symbol;
    char  int_frac_digits;
    char  int_n_cs_precedes;
    char  int_n_sep_by_space;
    char  int_n_sign_posn;
    char  int_p_cs_precedes;
    char  int_p_sep_by_space;
    char  int_p_sign_posn;
    char *mon_decimal_point;
    char *mon_grouping;
    char *mon_thousands_sep;
    char *negative_sign;
    char  n_cs_precedes;
    char  n_sep_by_space;
    char  n_sign_posn;
    char *positive_sign;
    char  p_cs_precedes;
    char  p_sep_by_space;
    char  p_sign_posn;
    char *thousands_sep;
};

char *        setlocale(int catflg, const char *locname);
struct lconv *localeconv(void);
#if (_XOPEN_SOURCE >= 700)
locale_t      duplocale(locale_t loc);
locale_t      newlocale(int catflg, const char *locname, locale_t locbase);
locale_t      uselocale(locale_t loc);
void          freelocale(locale_t loc);
#endif

#endif

#endif /* __LOCALE_H__ */

