#ifndef __STDATOMIC_H__
#define __STDATOMIC_H__

/* https://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/Atomic-Builtins.html */
/* https://apr.apache.org/docs/apr/1.5/group__apr__atomic.html */

#undef __STDC_NO_ATOMICS__

#include <stdbool.h>
#if defined(__x86_64__) || defined(__amd64__)
#include <x86-64/stdatomic.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <ia32/stdatomic.h>
#elif defined(__arm__)
#include <arm/stdatomic.h>
#endif

#define _Atomic    volatile
#define _Atomic(t) volatile t

/* atomic types */
typedef volatile _Bool                atomic_bool;
typedef volatile char                 atomic_char;
typedef volatile signed char          atomic_schar;
typedef volatile unsigned char        atomic_uchar;
typedef volatile short                atomic_short;
typedef volatile unsigned short       atomic_ushort;
typedef volatile int                  atomic_int;
typedef volatile unsigned int         atomic_uint;
typedef volatile long                 atomic_long;
typedef volatile unsigned long        atomic_ulong;
typedef volatile long long            atomic_llong;
typedef volatile unsigned long long   atomic_ullong;
typedef volatile float                atomic_float;
typedef volatile double               atomic_double;
typedef volatile long double          atomic_ldouble;
#if !defined(__STDC_NO_COMPLEX__)
typedef volatile float _Complex       atomic_cfloat;
typedef volatile double _Complex      atomic_cdouble;
typedef volatile long double _Complex atomic_cldouble;
#endif

/* see http://en.cppreference.com/w/c/atomic/memory_order */
enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};
typedef enum memory_order memory_order;

/* lock-free structure type */

#define ATOMIC_FLAG_INIT     { ATOMIC_FLAG_UNSET }
#define ATOMIC_VAR_INIT(val) { (val) }
#define ATOMIC_FLAG_CLEAR    0L
#define ATOMIC_FLAG_SET      1L

typedef struct {
    long __flg;
} atomic_flag;

#if 0
bool atomic_flag_test_and_set(volatile atomic_flag *obj);
bool atomic_flag_test_and_set_explicit(volatile atomic_flag *obj,
                                       memory_order order);
bool atomic_flag_test_and_set_explicit(volatile atomic_flag *obj,
                                       memory_order order,
                                       memory_order scope);
#endif
#define atomic_flag_test_and_set(obj) m_cmpswap((obj)->__flg,           \
                                                ATOMIC_FLAG_CLEAR,      \
                                                ATOMIC_FLAG_SET)

/* TODO: type kill_dependency(type)? */

#endif /* __STDATOMIC_H__ */

