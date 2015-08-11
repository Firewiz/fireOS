#ifndef LOCALE_H
#define LOCALE_H

#ifndef _CROSS_INCLUDE
#define NEED_LOCALE_T
#endif

#ifdef NEED_LOCALE_T
typedef struct lconv * locale_t;
#endif

#ifndef _CROSS_INCLUDE

struct lconv {
  char *currency_symbol;
  char *decimal_point;
  char frac_digits;
  char *grouping;
  char *int_curr_symbol;
  char int_frac_digits;
  char int_n_cs_precedes;
  char int_n_sep_by_space;
  char int_n_sign_posn;
  char int_p_cs_precedes;
  char int_p_sep_by_space;
  char int_p_sign_posn;
  char *mon_decimal_point;
  char *mon_grouping;
  char *mon_thousands_sep;
  char *negative_sign;
  char n_cs_precedes;
  char n_sep_by_space;
  char n_sign_posn;
  char *positive_sign;
  char p_cs_precedes;
  char p_sep_by_space;
  char p_sign_posn;
  char *thousands_sep;
};

#define LC_ALL 0
#define LC_COLLATE 1
#define LC_CTYPE 2
#define LC_MESSAGES 3
#define LC_MONETARY 4
#define LC_NUMERIC 5
#define LC_TIME 6

#define LC_COLLATE_MASK 0x01
#define LC_CTYPE_MASK 0x02
#define LC_MESSAGES_MASK 0x04
#define LC_MONETARY_MASK 0x08
#define LC_NUMERIC_MASK 0x10
#define LC_TIME_MASK 0x20
#define LC_ALL_MASK 0x2F

#ifndef DEFS
extern locale_t _global_locale;
#endif

#define LC_GLOBAL_LOCALE _global_locale

locale_t duplocale(locale_t);
void freelocale(locale_t);
struct lconv *localeconv(void);
locale_t newlocale(int, const char *, locale_t);
char *setlocale(int, const char *);
locale_t uselocale(locale_t);

#endif

#endif
