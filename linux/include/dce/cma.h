#ifndef CMA_INCLUDE
#define CMA_INCLUDE

/*
 * DCE CMA / POSIX 1003.4a draft-4 types, implemented on Linux NPTL.
 * See PORTING_GUIDE.pdf chapter 4.
 */

#include <dce/cma_config.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dce/exc_handling.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _DECTHREADS_	1

typedef int			cma_t_integer;
typedef unsigned int		cma_t_boolean;
typedef unsigned int		cma_t_natural;
typedef float			cma_t_interval;
typedef int			cma_t_key;
typedef int			cma_t_status;
typedef int			cma_t_priority;
typedef void			*cma_t_address;

#define cma_c_false		((cma_t_boolean)0)
#define cma_c_true		((cma_t_boolean)1)
#define cma_c_null_ptr		((cma_t_address)0)

#define cma_c_prio_fifo_min	16
#define cma_c_prio_fifo_mid	24
#define cma_c_prio_fifo_max	31
#define cma_c_prio_rr_min	16
#define cma_c_prio_rr_mid	24
#define cma_c_prio_rr_max	31
#define cma_c_prio_through_min	8
#define cma_c_prio_through_mid	12
#define cma_c_prio_through_max	15
#define cma_c_prio_back_min	1
#define cma_c_prio_back_mid	4
#define cma_c_prio_back_max	7
#define cma_c_prio_ada_low_min	0
#define cma_c_prio_ada_low_mid	4
#define cma_c_prio_ada_low_max	7

typedef struct timeval		cma_t_date_time;

typedef struct CMA_T_HANDLE {
    cma_t_address	field1;
    short int		field2;
    short int		field3;
} cma_t_handle;

#define cma_thread_get_unique(handle) \
    ((unsigned int)((cma_t_thread *)(handle))->field2)

#define cma_c_handle_size sizeof(cma_t_handle)

typedef cma_t_handle	cma_t_mutex;

#ifndef _CMA_SUPPRESS_EXTERNALS_
_CMA_IMPORT_ cma_t_handle	cma_c_null;
#endif

typedef struct CMA_T_ONCE {
    cma_t_integer	field1;
    cma_t_integer	field2;
    cma_t_integer	field3;
} cma_t_once;

#define cma_once_init	{0, 0, 0}

typedef struct CMA_T_ALERT_STATE {
    cma_t_integer	state1;
    cma_t_integer	state2;
} cma_t_alert_state;

extern void cma_handle_assign(cma_t_handle *, cma_t_handle *);
extern cma_t_boolean cma_handle_equal(cma_t_handle *, cma_t_handle *);

typedef cma_t_handle	cma_t_attr;
extern void cma_attr_create(cma_t_attr *, cma_t_attr *);
extern void cma_attr_delete(cma_t_attr *);

typedef cma_t_handle	cma_t_thread;
typedef cma_t_address	(*cma_t_start_routine)(cma_t_address);

typedef enum CMA_T_EXIT_STATUS {
    cma_c_term_error	= 0,
    cma_c_term_normal	= 1,
    cma_c_term_alert	= 2
} cma_t_exit_status;

typedef enum CMA_T_SCHED_INHERIT {
    cma_c_sched_inherit = 0,
    cma_c_sched_use_default = 1
} cma_t_sched_inherit;

typedef enum CMA_T_SCHED_POLICY {
    cma_c_sched_fifo = 0,
    cma_c_sched_rr = 1,
    cma_c_sched_throughput = 2,
    cma_c_sched_background = 3,
    cma_c_sched_ada_low = 4
} cma_t_sched_policy;

#define cma_c_sched_default	cma_c_sched_throughput
#define cma_c_sched_other	cma_c_sched_default

extern void cma_thread_create(cma_t_thread *, cma_t_attr *,
    cma_t_start_routine, cma_t_address);
extern void cma_thread_detach(cma_t_thread *);
extern void cma_thread_exit_error(void);
extern void cma_thread_exit_normal(cma_t_address);
extern void cma_thread_join(cma_t_thread *, cma_t_exit_status *,
    cma_t_address *);
extern void cma_thread_set_priority(cma_t_thread *, cma_t_priority);
extern void cma_thread_set_sched(cma_t_thread *, cma_t_sched_policy,
    cma_t_priority);
extern void cma_yield(void);
extern void cma_delay(cma_t_interval);
extern void cma_thread_get_self(cma_t_thread *);
extern void cma_thread_get_priority(cma_t_thread *, cma_t_priority *);
extern void cma_thread_get_sched(cma_t_thread *, cma_t_sched_policy *);

typedef enum CMA_T_MUTEX_KIND {
    cma_c_mutex_fast = 0,
    cma_c_mutex_recursive = 1,
    cma_c_mutex_nonrecursive = 2
} cma_t_mutex_kind;

extern void cma_attr_set_mutex_kind(cma_t_attr *, cma_t_mutex_kind);
extern void cma_attr_get_mutex_kind(cma_t_attr *, cma_t_mutex_kind *);
extern void cma_mutex_create(cma_t_mutex *, cma_t_attr *);
extern void cma_mutex_delete(cma_t_mutex *);
extern void cma_mutex_lock(cma_t_mutex *);
extern cma_t_boolean cma_mutex_try_lock(cma_t_mutex *);
extern void cma_mutex_unlock(cma_t_mutex *);

typedef cma_t_handle	cma_t_cond;
extern void cma_cond_create(cma_t_cond *, cma_t_attr *);
extern void cma_cond_delete(cma_t_cond *);
extern void cma_cond_wait(cma_t_cond *, cma_t_mutex *);
extern void cma_cond_signal(cma_t_cond *);
extern void cma_cond_broadcast(cma_t_cond *);
extern cma_t_status cma_cond_timed_wait(cma_t_cond *, cma_t_mutex *,
    cma_t_date_time *);
extern void cma_time_get_expiration(cma_t_date_time *, cma_t_interval);

extern void cma_init(void);
typedef void (*cma_t_init_routine)(cma_t_address);
extern void cma_once(cma_t_once *, cma_t_init_routine, cma_t_address);

typedef void (*cma_t_destructor)(cma_t_address);
extern void cma_key_create(cma_t_key *, cma_t_attr *, cma_t_destructor);
extern void cma_key_set_context(cma_t_key, cma_t_address);
extern void cma_key_get_context(cma_t_key, cma_t_address *);

extern void cma_thread_alert(cma_t_thread *);
extern void cma_alert_restore(cma_t_alert_state *);
extern void cma_alert_disable_general(cma_t_alert_state *);
extern void cma_alert_disable_asynch(cma_t_alert_state *);
extern void cma_alert_enable_general(cma_t_alert_state *);
extern void cma_alert_enable_asynch(cma_t_alert_state *);
extern void cma_alert_test(void);

#define cma_s_normal		0
#define cma_s_timed_out		49

#ifdef __cplusplus
}
#endif

#endif /* CMA_INCLUDE */
