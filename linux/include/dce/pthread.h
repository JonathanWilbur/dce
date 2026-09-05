#ifndef PTHREAD
#define PTHREAD

/*
 * DCE POSIX 1003.4a draft-4 pthread API, mapped onto Linux NPTL.
 * PORTING_GUIDE.pdf chapter 4.1: native pthreads instead of CMA user threads.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <dce/cma.h>

#ifndef _POSIX_THREADS
# define _POSIX_THREADS
#endif
#ifndef _POSIX_THREAD_ATTR_STACKSIZE
# define _POSIX_THREAD_ATTR_STACKSIZE
#endif

#define pthread_cleanup_push(routine,arg)	\
    { \
    pthread_cleanup_t _XXX_proc = (pthread_cleanup_t)(routine); \
    pthread_addr_t _XXX_arg = (arg); \
    int _XXX_completed = 0; \
    TRY {

#define pthread_cleanup_pop(execute)	\
    _XXX_completed = 1;} \
    FINALLY { \
	int _XXX_execute = (execute); \
	if ((!_XXX_completed) || (_XXX_execute)) _XXX_proc(_XXX_arg);} \
    ENDTRY}

#define pthread_equal_np(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))

#define pthread_equal(thread1,thread2) pthread_equal_np(thread1, thread2)

#define pthread_getunique_np(handle) \
    ((unsigned int)((pthread_t *)(handle))->field2)

typedef	cma_t_key		pthread_key_t;
typedef cma_t_address		pthread_addr_t;
typedef pthread_addr_t		any_t;
typedef void (*pthread_cleanup_t)(pthread_addr_t);

typedef cma_t_once	pthread_once_t;
#define pthread_once_init	cma_once_init

#define CANCEL_ON	1
#define CANCEL_OFF	0

typedef cma_t_attr	pthread_attr_t;
typedef cma_t_thread	pthread_t;
typedef cma_t_start_routine	pthread_startroutine_t;
typedef pthread_startroutine_t	pthread_func_t;

#define PTHREAD_INHERIT_SCHED	((int)cma_c_sched_inherit)
#define PTHREAD_DEFAULT_SCHED	((int)cma_c_sched_use_default)
#define SCHED_FIFO		((int)cma_c_sched_fifo)
#define SCHED_RR		((int)cma_c_sched_rr)
#define SCHED_FG_NP		((int)cma_c_sched_throughput)
#define SCHED_BG_NP		((int)cma_c_sched_background)
#define SCHED_OTHER		((int)cma_c_sched_throughput)

int pthread_attr_create(pthread_attr_t *);
int pthread_attr_delete(pthread_attr_t *);
int pthread_attr_setprio(pthread_attr_t *, int);
int pthread_attr_getprio(pthread_attr_t);
int pthread_attr_setsched(pthread_attr_t *, int);
int pthread_attr_getsched(pthread_attr_t);
int pthread_attr_setinheritsched(pthread_attr_t *, int);
int pthread_attr_getinheritsched(pthread_attr_t);
int pthread_attr_setstacksize(pthread_attr_t *, long);
long pthread_attr_getstacksize(pthread_attr_t);
int pthread_attr_setguardsize_np(pthread_attr_t *, long);
long pthread_attr_getguardsize_np(pthread_attr_t);

int pthread_create(pthread_t *, pthread_attr_t, pthread_startroutine_t,
    pthread_addr_t);
int pthread_detach(pthread_t *);
void pthread_exit(pthread_addr_t);
int pthread_join(pthread_t, pthread_addr_t *);
int pthread_setprio(pthread_t, int);
int pthread_setscheduler(pthread_t, int, int);
void pthread_yield(void);
pthread_t pthread_self(void);
int pthread_getprio(pthread_t);
int pthread_getscheduler(pthread_t);

#define MUTEX_FAST_NP		0
#define MUTEX_RECURSIVE_NP	1
#define MUTEX_NONRECURSIVE_NP	2

typedef cma_t_attr	pthread_mutexattr_t;
typedef	cma_t_mutex	pthread_mutex_t;

int pthread_mutexattr_create(pthread_mutexattr_t *);
int pthread_mutexattr_delete(pthread_mutexattr_t *);
int pthread_mutexattr_setkind_np(pthread_mutexattr_t *, int);
int pthread_mutexattr_getkind_np(pthread_mutexattr_t);
int pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t);
int pthread_mutex_destroy(pthread_mutex_t *);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);

typedef cma_t_attr	pthread_condattr_t;
typedef cma_t_cond	pthread_cond_t;

int pthread_condattr_create(pthread_condattr_t *);
int pthread_condattr_delete(pthread_condattr_t *);
int pthread_cond_init(pthread_cond_t *, pthread_condattr_t);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_signal_int_np(pthread_cond_t *);
int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
int pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
    struct timespec *);

typedef void (*pthread_initroutine_t)(void);
int pthread_once(pthread_once_t *, pthread_initroutine_t);

typedef cma_t_destructor	pthread_destructor_t;
int pthread_keycreate(pthread_key_t *, pthread_destructor_t);
int pthread_setspecific(pthread_key_t, pthread_addr_t);
int pthread_getspecific(pthread_key_t, pthread_addr_t *);

int pthread_cancel(pthread_t);
void pthread_testcancel(void);
int pthread_setasynccancel(int);
int pthread_setcancel(int);

_CMA_IMPORT_ pthread_attr_t		pthread_attr_default;
_CMA_IMPORT_ pthread_mutexattr_t	pthread_mutexattr_default;
_CMA_IMPORT_ pthread_condattr_t		pthread_condattr_default;

int pthread_get_expiration_np(struct timespec *, struct timespec *);
int pthread_delay_np(struct timespec *);
void pthread_lock_global_np(void);
void pthread_unlock_global_np(void);
int pthread_signal_to_cancel_np(void *set, pthread_t *thread);

#ifdef __cplusplus
}
#endif

#endif /* PTHREAD */
