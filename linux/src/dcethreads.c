/*
 * DCE CMA + draft-4 pthreads on Linux NPTL.
 * PORTING_GUIDE.pdf chapter 4.
 */

#include <dce/cma.h>
#include <dce/pthread.h>
#include <dce/exc_handling.h>
#include "native_pthreads.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KIND_NULL	0
#define KIND_THREAD	1
#define KIND_MUTEX	2
#define KIND_COND	3
#define KIND_ATTR	4

#define HANDLE_PTR(h)	((h).field1)
#define HANDLE_ID(h)	((h).field2)
#define HANDLE_KIND(h)	((h).field3)

struct dce_thread {
    native_thread_t	native;
    unsigned short	id;
    int			detached;
    cma_t_address	result;
    cma_t_exit_status	exit_status;
};

struct dce_mutex {
    native_mutex_t	native;
    int			recursive;
};

struct dce_cond {
    native_cond_t	native;
};

struct dce_attr {
    native_attr_t	native;
    int			mutex_kind;
    long		stacksize;
    long		guardsize;
    int			priority;
    int			sched;
};

static native_key_t self_key;
static native_key_t exc_key;
static native_mutex_t global_lock;
static native_mutex_t id_lock;
static unsigned short next_id = 1;
static int initialized;
static struct dce_thread main_thread;

cma_t_handle cma_c_null;
pthread_attr_t pthread_attr_default;
pthread_mutexattr_t pthread_mutexattr_default;
pthread_condattr_t pthread_condattr_default;

EXCEPTION exc_e_uninitexc, pthread_cancel_e, pthread_badparam_e,
    pthread_exists_e, pthread_in_use_e, pthread_notstack_e,
    pthread_unimp_e, pthread_inialrpro_e, exc_e_illaddr, exc_e_exquota,
    exc_e_insfmem, exc_e_nopriv, pthread_use_error_e, pthread_invalid_e;

const char *dcelocal_path = "/opt/dcelocal";
const char *dceshared_path = "/opt/dcelocal";

static unsigned short
alloc_id(void)
{
    unsigned short id;
    native_mutex_lock(id_lock);
    id = next_id++;
    if (next_id == 0)
        next_id = 1;
    native_mutex_unlock(id_lock);
    return id;
}

static int
is_null_handle(cma_t_handle *h)
{
    return h == NULL || (HANDLE_PTR(*h) == NULL && HANDLE_KIND(*h) == KIND_NULL);
}

static void
set_handle(cma_t_handle *h, void *ptr, unsigned short id, short kind)
{
    h->field1 = ptr;
    h->field2 = (short)id;
    h->field3 = kind;
}

static struct dce_thread *
current_thread(void)
{
    struct dce_thread *t = native_getspecific(self_key);
    if (t == NULL)
        t = &main_thread;
    return t;
}

static void
init_exceptions(void)
{
    EXCEPTION_INIT(exc_e_uninitexc);
    EXCEPTION_INIT(pthread_cancel_e);
    EXCEPTION_INIT(pthread_badparam_e);
    EXCEPTION_INIT(pthread_exists_e);
    EXCEPTION_INIT(pthread_in_use_e);
    EXCEPTION_INIT(pthread_notstack_e);
    EXCEPTION_INIT(pthread_unimp_e);
    EXCEPTION_INIT(pthread_inialrpro_e);
    EXCEPTION_INIT(exc_e_illaddr);
    EXCEPTION_INIT(exc_e_exquota);
    EXCEPTION_INIT(exc_e_insfmem);
    EXCEPTION_INIT(exc_e_nopriv);
    EXCEPTION_INIT(pthread_use_error_e);
    EXCEPTION_INIT(pthread_invalid_e);
}

void
cma_init(void)
{
    if (initialized)
        return;
    native_threads_init();
    if (native_key_create(&self_key, NULL) != 0) {
        fprintf(stderr, "DCE: native_key_create(self) failed errno=%d\n", errno);
        abort();
    }
    if (native_key_create(&exc_key, NULL) != 0) {
        fprintf(stderr, "DCE: native_key_create(exc) failed errno=%d\n", errno);
        abort();
    }
    global_lock = native_mutex_new(1);
    id_lock = native_mutex_new(0);
    if (global_lock == NULL || id_lock == NULL) {
        fprintf(stderr, "DCE: native_mutex_new failed\n");
        abort();
    }
    memset(&cma_c_null, 0, sizeof(cma_c_null));
    memset(&pthread_attr_default, 0, sizeof(pthread_attr_default));
    memset(&pthread_mutexattr_default, 0, sizeof(pthread_mutexattr_default));
    memset(&pthread_condattr_default, 0, sizeof(pthread_condattr_default));
    memset(&main_thread, 0, sizeof(main_thread));
    main_thread.native = native_thread_self();
    main_thread.id = alloc_id();
    native_setspecific(self_key, &main_thread);
    init_exceptions();
    initialized = 1;
}

static void
ensure_init(void)
{
    if (!initialized)
        cma_init();
}

void
cma_handle_assign(cma_t_handle *dst, cma_t_handle *src)
{
    *dst = *src;
}

cma_t_boolean
cma_handle_equal(cma_t_handle *a, cma_t_handle *b)
{
    return a->field1 == b->field1 && a->field2 == b->field2
        && a->field3 == b->field3;
}

static struct dce_attr *
attr_from_handle(cma_t_attr *attr)
{
    if (is_null_handle(attr) || HANDLE_KIND(*attr) != KIND_ATTR)
        return NULL;
    return HANDLE_PTR(*attr);
}

void
cma_attr_create(cma_t_attr *attr, cma_t_attr *proto)
{
    struct dce_attr *a = calloc(1, sizeof(*a));
    (void)proto;
    ensure_init();
    a->native = native_attr_new();
    a->mutex_kind = MUTEX_FAST_NP;
    set_handle(attr, a, alloc_id(), KIND_ATTR);
}

void
cma_attr_delete(cma_t_attr *attr)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a == NULL)
        return;
    native_attr_free(a->native);
    free(a);
    set_handle(attr, NULL, 0, KIND_NULL);
}

void
cma_attr_set_mutex_kind(cma_t_attr *attr, cma_t_mutex_kind kind)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a)
        a->mutex_kind = (int)kind;
}

void
cma_attr_get_mutex_kind(cma_t_attr *attr, cma_t_mutex_kind *kind)
{
    struct dce_attr *a = attr_from_handle(attr);
    *kind = a ? (cma_t_mutex_kind)a->mutex_kind : cma_c_mutex_fast;
}

struct start_pack {
    struct dce_thread	*thread;
    cma_t_start_routine	fn;
    cma_t_address	arg;
};

static void *
thread_trampoline(void *arg)
{
    struct start_pack pack = *(struct start_pack *)arg;
    cma_t_address result;

    free(arg);
    native_setspecific(self_key, pack.thread);
    result = pack.fn(pack.arg);
    pack.thread->result = result;
    pack.thread->exit_status = cma_c_term_normal;
    return result;
}

void
cma_thread_create(cma_t_thread *thread, cma_t_attr *attr,
    cma_t_start_routine start, cma_t_address arg)
{
    struct dce_thread *t = calloc(1, sizeof(*t));
    struct start_pack *pack = malloc(sizeof(*pack));
    struct dce_attr *a = attr_from_handle(attr);
    native_attr_t nattr = a ? a->native : NULL;

    ensure_init();
    t->id = alloc_id();
    t->exit_status = cma_c_term_normal;
    pack->thread = t;
    pack->fn = start;
    pack->arg = arg;
    if (native_thread_create(&t->native, nattr, thread_trampoline, pack) != 0) {
        free(pack);
        free(t);
        set_handle(thread, NULL, 0, KIND_NULL);
        return;
    }
    set_handle(thread, t, t->id, KIND_THREAD);
}

void
cma_thread_detach(cma_t_thread *thread)
{
    struct dce_thread *t = HANDLE_PTR(*thread);
    if (t == NULL)
        return;
    native_thread_detach(t->native);
    t->detached = 1;
}

void
cma_thread_join(cma_t_thread *thread, cma_t_exit_status *exit,
    cma_t_address *result)
{
    struct dce_thread *t = HANDLE_PTR(*thread);
    void *retval = NULL;
    if (t == NULL)
        return;
    native_thread_join(t->native, &retval);
    if (exit)
        *exit = t->exit_status;
    if (result)
        *result = retval ? retval : t->result;
}

void
cma_thread_get_self(cma_t_thread *thread)
{
    struct dce_thread *t = current_thread();
    set_handle(thread, t, t->id, KIND_THREAD);
}

void
cma_yield(void)
{
    native_thread_yield();
}

void
cma_delay(cma_t_interval seconds)
{
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - (float)ts.tv_sec) * 1e9);
    if (ts.tv_nsec < 0)
        ts.tv_nsec = 0;
    native_nanosleep(&ts, NULL);
}

void
cma_thread_exit_normal(cma_t_address result)
{
    struct dce_thread *t = current_thread();
    t->result = result;
    t->exit_status = cma_c_term_normal;
    native_thread_exit(result);
}

void
cma_thread_exit_error(void)
{
    struct dce_thread *t = current_thread();
    t->exit_status = cma_c_term_error;
    native_thread_exit(NULL);
}

void
cma_thread_set_priority(cma_t_thread *thread, cma_t_priority prio)
{
    (void)thread;
    (void)prio;
}

void
cma_thread_set_sched(cma_t_thread *thread, cma_t_sched_policy pol,
    cma_t_priority prio)
{
    (void)thread;
    (void)pol;
    (void)prio;
}

void
cma_thread_get_priority(cma_t_thread *thread, cma_t_priority *prio)
{
    (void)thread;
    *prio = cma_c_prio_through_mid;
}

void
cma_thread_get_sched(cma_t_thread *thread, cma_t_sched_policy *pol)
{
    (void)thread;
    *pol = cma_c_sched_throughput;
}

void
cma_mutex_create(cma_t_mutex *mutex, cma_t_attr *attr)
{
    struct dce_attr *a = attr_from_handle(attr);
    struct dce_mutex *m = calloc(1, sizeof(*m));
    int rec = a && a->mutex_kind == MUTEX_RECURSIVE_NP;
    ensure_init();
    m->recursive = rec;
    m->native = native_mutex_new(rec);
    set_handle(mutex, m, alloc_id(), KIND_MUTEX);
}

void
cma_mutex_delete(cma_t_mutex *mutex)
{
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    if (m == NULL)
        return;
    native_mutex_free(m->native);
    free(m);
    set_handle(mutex, NULL, 0, KIND_NULL);
}

void
cma_mutex_lock(cma_t_mutex *mutex)
{
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    native_mutex_lock(m->native);
}

cma_t_boolean
cma_mutex_try_lock(cma_t_mutex *mutex)
{
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    return native_mutex_trylock(m->native) == 0 ? cma_c_true : cma_c_false;
}

void
cma_mutex_unlock(cma_t_mutex *mutex)
{
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    native_mutex_unlock(m->native);
}

void
cma_cond_create(cma_t_cond *cond, cma_t_attr *attr)
{
    struct dce_cond *c = calloc(1, sizeof(*c));
    (void)attr;
    ensure_init();
    c->native = native_cond_new();
    set_handle(cond, c, alloc_id(), KIND_COND);
}

void
cma_cond_delete(cma_t_cond *cond)
{
    struct dce_cond *c = HANDLE_PTR(*cond);
    if (c == NULL)
        return;
    native_cond_free(c->native);
    free(c);
    set_handle(cond, NULL, 0, KIND_NULL);
}

void
cma_cond_wait(cma_t_cond *cond, cma_t_mutex *mutex)
{
    struct dce_cond *c = HANDLE_PTR(*cond);
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    native_cond_wait(c->native, m->native);
}

void
cma_cond_signal(cma_t_cond *cond)
{
    struct dce_cond *c = HANDLE_PTR(*cond);
    native_cond_signal(c->native);
}

void
cma_cond_broadcast(cma_t_cond *cond)
{
    struct dce_cond *c = HANDLE_PTR(*cond);
    native_cond_broadcast(c->native);
}

cma_t_status
cma_cond_timed_wait(cma_t_cond *cond, cma_t_mutex *mutex,
    cma_t_date_time *timeout)
{
    struct dce_cond *c = HANDLE_PTR(*cond);
    struct dce_mutex *m = HANDLE_PTR(*mutex);
    struct timespec ts;
    int rc;
    ts.tv_sec = timeout->tv_sec;
    ts.tv_nsec = timeout->tv_usec * 1000;
    rc = native_cond_timedwait(c->native, m->native, &ts);
    return rc == ETIMEDOUT ? cma_s_timed_out : cma_s_normal;
}

void
cma_time_get_expiration(cma_t_date_time *when, cma_t_interval interval)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    when->tv_sec = now.tv_sec + (time_t)interval;
    when->tv_usec = (suseconds_t)((interval - (float)(time_t)interval) * 1e6)
        + now.tv_nsec / 1000;
    if (when->tv_usec >= 1000000) {
        when->tv_sec++;
        when->tv_usec -= 1000000;
    }
}

void
cma_once(cma_t_once *block, cma_t_init_routine init, cma_t_address arg)
{
    ensure_init();
    native_mutex_lock(global_lock);
    if (block->field1 == 0) {
        init(arg);
        block->field1 = 1;
    }
    native_mutex_unlock(global_lock);
}

void
cma_key_create(cma_t_key *key, cma_t_attr *attr, cma_t_destructor dtor)
{
    native_key_t k;
    (void)attr;
    ensure_init();
    native_key_create(&k, (native_destructor_t)dtor);
    *key = (cma_t_key)k;
}

void
cma_key_set_context(cma_t_key key, cma_t_address value)
{
    native_setspecific((native_key_t)key, value);
}

void
cma_key_get_context(cma_t_key key, cma_t_address *value)
{
    *value = native_getspecific((native_key_t)key);
}

void
cma_thread_alert(cma_t_thread *thread)
{
    struct dce_thread *t = HANDLE_PTR(*thread);
    if (t)
        native_thread_cancel(t->native);
}

void
cma_alert_test(void)
{
    native_thread_testcancel();
}

void
cma_alert_restore(cma_t_alert_state *state)
{
    native_thread_setcancelstate(state->state1);
    native_thread_setcanceltype_async(state->state2);
}

void
cma_alert_disable_general(cma_t_alert_state *state)
{
    state->state1 = 1;
    state->state2 = 0;
    native_thread_setcancelstate(0);
}

void
cma_alert_enable_general(cma_t_alert_state *state)
{
    state->state1 = 1;
    native_thread_setcancelstate(1);
}

void
cma_alert_disable_asynch(cma_t_alert_state *state)
{
    state->state2 = 0;
    native_thread_setcanceltype_async(0);
}

void
cma_alert_enable_asynch(cma_t_alert_state *state)
{
    state->state2 = 1;
    native_thread_setcanceltype_async(1);
}

/* ---------- draft-4 pthread API ---------- */

int
pthread_attr_create(pthread_attr_t *attr)
{
    cma_attr_create(attr, &cma_c_null);
    return 0;
}

int
pthread_attr_delete(pthread_attr_t *attr)
{
    cma_attr_delete(attr);
    return 0;
}

int
pthread_attr_setstacksize(pthread_attr_t *attr, long size)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a == NULL)
        return -1;
    a->stacksize = size;
    native_attr_setstacksize(a->native, (size_t)size);
    return 0;
}

long
pthread_attr_getstacksize(pthread_attr_t attr)
{
    struct dce_attr *a = attr_from_handle(&attr);
    if (a == NULL)
        return 0;
    return a->stacksize ? a->stacksize : (long)native_attr_getstacksize(a->native);
}

int
pthread_attr_setguardsize_np(pthread_attr_t *attr, long size)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a)
        a->guardsize = size;
    return 0;
}

long
pthread_attr_getguardsize_np(pthread_attr_t attr)
{
    struct dce_attr *a = attr_from_handle(&attr);
    return a ? a->guardsize : 0;
}

int
pthread_attr_setprio(pthread_attr_t *attr, int prio)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a)
        a->priority = prio;
    return 0;
}

int
pthread_attr_getprio(pthread_attr_t attr)
{
    struct dce_attr *a = attr_from_handle(&attr);
    return a ? a->priority : 0;
}

int
pthread_attr_setsched(pthread_attr_t *attr, int sched)
{
    struct dce_attr *a = attr_from_handle(attr);
    if (a)
        a->sched = sched;
    return 0;
}

int
pthread_attr_getsched(pthread_attr_t attr)
{
    struct dce_attr *a = attr_from_handle(&attr);
    return a ? a->sched : SCHED_OTHER;
}

int
pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit)
{
    (void)attr;
    (void)inherit;
    return 0;
}

int
pthread_attr_getinheritsched(pthread_attr_t attr)
{
    (void)attr;
    return PTHREAD_DEFAULT_SCHED;
}

int
pthread_create(pthread_t *thread, pthread_attr_t attr,
    pthread_startroutine_t start, pthread_addr_t arg)
{
    cma_thread_create(thread, &attr, start, arg);
    return HANDLE_PTR(*thread) ? 0 : -1;
}

int
pthread_detach(pthread_t *thread)
{
    cma_thread_detach(thread);
    return 0;
}

void
pthread_exit(pthread_addr_t result)
{
    cma_thread_exit_normal(result);
}

int
pthread_join(pthread_t thread, pthread_addr_t *result)
{
    cma_t_exit_status st;
    cma_thread_join(&thread, &st, result);
    return 0;
}

void
pthread_yield(void)
{
    cma_yield();
}

pthread_t
pthread_self(void)
{
    pthread_t self;
    cma_thread_get_self(&self);
    return self;
}

int
pthread_setprio(pthread_t thread, int prio)
{
    cma_thread_set_priority(&thread, prio);
    return prio;
}

int
pthread_getprio(pthread_t thread)
{
    cma_t_priority p = 0;
    cma_thread_get_priority(&thread, &p);
    return p;
}

int
pthread_setscheduler(pthread_t thread, int policy, int prio)
{
    cma_thread_set_sched(&thread, (cma_t_sched_policy)policy, prio);
    return policy;
}

int
pthread_getscheduler(pthread_t thread)
{
    cma_t_sched_policy p = cma_c_sched_throughput;
    cma_thread_get_sched(&thread, &p);
    return (int)p;
}

int
pthread_mutexattr_create(pthread_mutexattr_t *attr)
{
    return pthread_attr_create(attr);
}

int
pthread_mutexattr_delete(pthread_mutexattr_t *attr)
{
    return pthread_attr_delete(attr);
}

int
pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind)
{
    cma_attr_set_mutex_kind(attr, (cma_t_mutex_kind)kind);
    return 0;
}

int
pthread_mutexattr_getkind_np(pthread_mutexattr_t attr)
{
    cma_t_mutex_kind k = cma_c_mutex_fast;
    cma_attr_get_mutex_kind(&attr, &k);
    return (int)k;
}

int
pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t attr)
{
    cma_mutex_create(mutex, &attr);
    return 0;
}

int
pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    cma_mutex_delete(mutex);
    return 0;
}

int
pthread_mutex_lock(pthread_mutex_t *mutex)
{
    cma_mutex_lock(mutex);
    return 0;
}

int
pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    return cma_mutex_try_lock(mutex) ? 1 : 0;
}

int
pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    cma_mutex_unlock(mutex);
    return 0;
}

int
pthread_condattr_create(pthread_condattr_t *attr)
{
    return pthread_attr_create(attr);
}

int
pthread_condattr_delete(pthread_condattr_t *attr)
{
    return pthread_attr_delete(attr);
}

int
pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t attr)
{
    cma_cond_create(cond, &attr);
    return 0;
}

int
pthread_cond_destroy(pthread_cond_t *cond)
{
    cma_cond_delete(cond);
    return 0;
}

int
pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    cma_cond_wait(cond, mutex);
    return 0;
}

int
pthread_cond_signal(pthread_cond_t *cond)
{
    cma_cond_signal(cond);
    return 0;
}

int
pthread_cond_broadcast(pthread_cond_t *cond)
{
    cma_cond_broadcast(cond);
    return 0;
}

int
pthread_cond_signal_int_np(pthread_cond_t *cond)
{
    return pthread_cond_signal(cond);
}

int
pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
    struct timespec *abs)
{
    cma_t_date_time dt;
    dt.tv_sec = abs->tv_sec;
    dt.tv_usec = abs->tv_nsec / 1000;
    return cma_cond_timed_wait(cond, mutex, &dt) == cma_s_timed_out ? -1 : 0;
}

int
pthread_once(pthread_once_t *once, pthread_initroutine_t init)
{
    cma_once(once, (cma_t_init_routine)init, NULL);
    return 0;
}

int
pthread_keycreate(pthread_key_t *key, pthread_destructor_t dtor)
{
    cma_key_create(key, &cma_c_null, dtor);
    return 0;
}

int
pthread_setspecific(pthread_key_t key, pthread_addr_t value)
{
    cma_key_set_context(key, value);
    return 0;
}

int
pthread_getspecific(pthread_key_t key, pthread_addr_t *value)
{
    cma_key_get_context(key, value);
    return 0;
}

int
pthread_cancel(pthread_t thread)
{
    cma_thread_alert(&thread);
    return 0;
}

void
pthread_testcancel(void)
{
    cma_alert_test();
}

int
pthread_setcancel(int state)
{
    if (state == CANCEL_ON)
        native_thread_setcancelstate(1);
    else
        native_thread_setcancelstate(0);
    return state;
}

int
pthread_setasynccancel(int state)
{
    native_thread_setcanceltype_async(state == CANCEL_ON);
    return state;
}

int
pthread_get_expiration_np(struct timespec *delta, struct timespec *abstime)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    abstime->tv_sec = now.tv_sec + delta->tv_sec;
    abstime->tv_nsec = now.tv_nsec + delta->tv_nsec;
    if (abstime->tv_nsec >= 1000000000L) {
        abstime->tv_sec++;
        abstime->tv_nsec -= 1000000000L;
    }
    return 0;
}

int
pthread_delay_np(struct timespec *interval)
{
    native_nanosleep(interval, NULL);
    return 0;
}

void
pthread_lock_global_np(void)
{
    ensure_init();
    native_mutex_lock(global_lock);
}

void
pthread_unlock_global_np(void)
{
    native_mutex_unlock(global_lock);
}

int
pthread_signal_to_cancel_np(void *set, pthread_t *thread)
{
    (void)set;
    (void)thread;
    return 0;
}

/* ---------- exception package ---------- */

void
exc_push_ctx(_EXC_VOLATILE_ exc_context_t *cb)
{
    ensure_init();
    cb->link = native_getspecific(exc_key);
    cb->exc_state = exc_none_c;
    native_setspecific(exc_key, (void *)cb);
}

void
exc_pop_ctx(_EXC_VOLATILE_ exc_context_t *cb)
{
    native_setspecific(exc_key, (void *)cb->link);
    if (cb->exc_state == exc_active_c) {
        EXCEPTION copy = cb->cur_exception;
        cb->exc_state = exc_popped_c;
        exc_raise(&copy);
    }
    cb->exc_state = exc_popped_c;
}

void
exc_raise(EXCEPTION *exc)
{
    exc_context_t *cb;

    ensure_init();
    cb = native_getspecific(exc_key);
    if (cb == NULL) {
        fprintf(stderr, "DCE: unhandled exception (kind=%d)\n",
            (int)exc->kind);
        abort();
    }
    cb->cur_exception = *exc;
    cb->exc_state = exc_active_c;
    _longjmp(*(jmp_buf *)cb->jmp, 1);
}

void
exc_raise_status(exc_int_t status)
{
    EXCEPTION e;
    EXCEPTION_INIT(e);
    exc_set_status(&e, status);
    exc_raise(&e);
}

void
exc_report(EXCEPTION *exc)
{
    fprintf(stderr, "DCE exception kind=%d addr=%p status=%ld\n",
        (int)exc->kind, exc->address.address,
        (long)exc->status.status);
}
