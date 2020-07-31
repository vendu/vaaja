#ifndef __COREWAR_CONF_H__
#define __COREWAR_CONF_H__

#define CWPIDMAP         1
//#define CWRANDMT32       0
#if defined(ZEUS) && !(ZEUS)
#undef  ZEUS
#endif
#undef  ZEUSHOVERTOOLTIP
//#define ZEUSHOVERTOOLTIP 1
#if !defined(ZEUS)
#undef  ZEUS
#undef  ZEUSWINX11
#undef  ZEUSWINEFL
#else
//#define ZEUSWINX11       1
//#define ZEUSIMLIB2       1
//#define ZEUSWINSDL       1
#endif

#endif /* __COREWAR_CONF_H__ */

