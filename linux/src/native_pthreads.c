#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "native_pthreads.h"

#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Resolve glibc pthread symbols at runtime. A static libdcethreads.a also
 * exports the DCE draft-4 pthread_create; if native_pthreads.o bound to
 * that symbol we would recurse until the stack overflowed.
 */

static int (*sys_pthread_create)(pthread_t *, const pthread_attr_t *,
    void *(*)(void *), void *);
static int (*sys_pthread_join)(pthread_t, void **);
static int (*sys_pthread_detach)(pthread_t);
static pthread_t (*sys_pthread_self)(void);
static int (*sys_pthread_equal)(pthread_t, pthread_t);
static int (*sys_pthread_cancel)(pthread_t);
static void (*sys_pthread_testcancel)(void);
static int (*sys_pthread_setcancelstate)(int, int *);
static int (*sys_pthread_setcanceltype)(int, int *);
static void (*sys_pthread_exit)(void *);
static int (*sys_pthread_attr_init)(pthread_attr_t *);
static int (*sys_pthread_attr_destroy)(pthread_attr_t *);
static int (*sys_pthread_attr_setdetachstate)(pthread_attr_t *, int);
static int (*sys_pthread_attr_setstacksize)(pthread_attr_t *, size_t);
static int (*sys_pthread_attr_getstacksize)(const pthread_attr_t *, size_t *);
static int (*sys_pthread_mutexattr_init)(pthread_mutexattr_t *);
static int (*sys_pthread_mutexattr_destroy)(pthread_mutexattr_t *);
static int (*sys_pthread_mutexattr_settype)(pthread_mutexattr_t *, int);
static int (*sys_pthread_mutex_init)(pthread_mutex_t *,
    const pthread_mutexattr_t *);
static int (*sys_pthread_mutex_destroy)(pthread_mutex_t *);
static int (*sys_pthread_mutex_lock)(pthread_mutex_t *);
static int (*sys_pthread_mutex_trylock)(pthread_mutex_t *);
static int (*sys_pthread_mutex_unlock)(pthread_mutex_t *);
static int (*sys_pthread_cond_init)(pthread_cond_t *, const pthread_condattr_t *);
static int (*sys_pthread_cond_destroy)(pthread_cond_t *);
static int (*sys_pthread_cond_wait)(pthread_cond_t *, pthread_mutex_t *);
static int (*sys_pthread_cond_timedwait)(pthread_cond_t *, pthread_mutex_t *,
    const struct timespec *);
static int (*sys_pthread_cond_signal)(pthread_cond_t *);
static int (*sys_pthread_cond_broadcast)(pthread_cond_t *);
static int (*sys_pthread_key_create)(pthread_key_t *, void (*)(void *));
static int (*sys_pthread_setspecific)(pthread_key_t, const void *);
static void *(*sys_pthread_getspecific)(pthread_key_t);

#define LOAD(fn) do { \
    sys_##fn = dlsym(RTLD_NEXT, #fn); \
    if (sys_##fn == NULL) \
        sys_##fn = dlsym(RTLD_DEFAULT, #fn); \
    if (sys_##fn == NULL) { \
        fprintf(stderr, "DCE: cannot resolve %s: %s\n", #fn, dlerror()); \
        abort(); \
    } \
} while (0)

struct native_mutex {
    pthread_mutex_t m;
};

struct native_cond {
    pthread_cond_t c;
};

struct native_attr {
    pthread_attr_t a;
};

int
native_threads_init(void)
{
    static int ready;
    if (ready)
        return 0;
    LOAD(pthread_create);
    LOAD(pthread_join);
    LOAD(pthread_detach);
    LOAD(pthread_self);
    LOAD(pthread_equal);
    LOAD(pthread_cancel);
    LOAD(pthread_testcancel);
    LOAD(pthread_setcancelstate);
    LOAD(pthread_setcanceltype);
    LOAD(pthread_exit);
    LOAD(pthread_attr_init);
    LOAD(pthread_attr_destroy);
    LOAD(pthread_attr_setdetachstate);
    LOAD(pthread_attr_setstacksize);
    LOAD(pthread_attr_getstacksize);
    LOAD(pthread_mutexattr_init);
    LOAD(pthread_mutexattr_destroy);
    LOAD(pthread_mutexattr_settype);
    LOAD(pthread_mutex_init);
    LOAD(pthread_mutex_destroy);
    LOAD(pthread_mutex_lock);
    LOAD(pthread_mutex_trylock);
    LOAD(pthread_mutex_unlock);
    LOAD(pthread_cond_init);
    LOAD(pthread_cond_destroy);
    LOAD(pthread_cond_wait);
    LOAD(pthread_cond_timedwait);
    LOAD(pthread_cond_signal);
    LOAD(pthread_cond_broadcast);
    LOAD(pthread_key_create);
    LOAD(pthread_setspecific);
    LOAD(pthread_getspecific);
    ready = 1;
    return 0;
}

int
native_thread_create(native_thread_t *tid, native_attr_t attr,
    void *(*start)(void *), void *arg)
{
    pthread_attr_t *ap = attr ? &((struct native_attr *)attr)->a : NULL;
    pthread_t t;
    int rc = sys_pthread_create(&t, ap, start, arg);
    if (rc == 0)
        *tid = (native_thread_t)t;
    return rc;
}

int
native_thread_join(native_thread_t tid, void **retval)
{
    return sys_pthread_join((pthread_t)tid, retval);
}

int
native_thread_detach(native_thread_t tid)
{
    return sys_pthread_detach((pthread_t)tid);
}

native_thread_t
native_thread_self(void)
{
    return (native_thread_t)sys_pthread_self();
}

int
native_thread_equal(native_thread_t a, native_thread_t b)
{
    return sys_pthread_equal((pthread_t)a, (pthread_t)b);
}

int
native_thread_cancel(native_thread_t tid)
{
    return sys_pthread_cancel((pthread_t)tid);
}

void
native_thread_testcancel(void)
{
    sys_pthread_testcancel();
}

int
native_thread_setcancelstate(int enable)
{
    int old;
    return sys_pthread_setcancelstate(
        enable ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE, &old);
}

int
native_thread_setcanceltype_async(int async)
{
    int old;
    return sys_pthread_setcanceltype(
        async ? PTHREAD_CANCEL_ASYNCHRONOUS : PTHREAD_CANCEL_DEFERRED, &old);
}

void
native_thread_exit(void *retval)
{
    sys_pthread_exit(retval);
}

int
native_thread_yield(void)
{
    return sched_yield();
}

native_attr_t
native_attr_new(void)
{
    struct native_attr *a = calloc(1, sizeof(*a));
    if (a == NULL)
        return NULL;
    if (sys_pthread_attr_init(&a->a) != 0) {
        free(a);
        return NULL;
    }
    sys_pthread_attr_setdetachstate(&a->a, PTHREAD_CREATE_JOINABLE);
    return a;
}

void
native_attr_free(native_attr_t attr)
{
    struct native_attr *a = attr;
    if (a == NULL)
        return;
    sys_pthread_attr_destroy(&a->a);
    free(a);
}

int
native_attr_setstacksize(native_attr_t attr, size_t size)
{
    struct native_attr *a = attr;
    return sys_pthread_attr_setstacksize(&a->a, size);
}

size_t
native_attr_getstacksize(native_attr_t attr)
{
    struct native_attr *a = attr;
    size_t size = 0;
    sys_pthread_attr_getstacksize(&a->a, &size);
    return size;
}

native_mutex_t
native_mutex_new(int recursive)
{
    struct native_mutex *m = calloc(1, sizeof(*m));
    pthread_mutexattr_t attr;
    if (m == NULL)
        return NULL;
    sys_pthread_mutexattr_init(&attr);
    if (recursive)
        sys_pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    else
        sys_pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    if (sys_pthread_mutex_init(&m->m, &attr) != 0) {
        sys_pthread_mutexattr_destroy(&attr);
        free(m);
        return NULL;
    }
    sys_pthread_mutexattr_destroy(&attr);
    return m;
}

int
native_mutex_free(native_mutex_t m)
{
    struct native_mutex *nm = m;
    int rc;
    if (nm == NULL)
        return 0;
    rc = sys_pthread_mutex_destroy(&nm->m);
    free(nm);
    return rc;
}

int
native_mutex_lock(native_mutex_t m)
{
    return sys_pthread_mutex_lock(&((struct native_mutex *)m)->m);
}

int
native_mutex_trylock(native_mutex_t m)
{
    return sys_pthread_mutex_trylock(&((struct native_mutex *)m)->m);
}

int
native_mutex_unlock(native_mutex_t m)
{
    return sys_pthread_mutex_unlock(&((struct native_mutex *)m)->m);
}

native_cond_t
native_cond_new(void)
{
    struct native_cond *c = calloc(1, sizeof(*c));
    if (c == NULL)
        return NULL;
    if (sys_pthread_cond_init(&c->c, NULL) != 0) {
        free(c);
        return NULL;
    }
    return c;
}

int
native_cond_free(native_cond_t c)
{
    struct native_cond *nc = c;
    int rc;
    if (nc == NULL)
        return 0;
    rc = sys_pthread_cond_destroy(&nc->c);
    free(nc);
    return rc;
}

int
native_cond_wait(native_cond_t c, native_mutex_t m)
{
    return sys_pthread_cond_wait(
        &((struct native_cond *)c)->c,
        &((struct native_mutex *)m)->m);
}

int
native_cond_timedwait(native_cond_t c, native_mutex_t m,
    const struct timespec *abs)
{
    return sys_pthread_cond_timedwait(
        &((struct native_cond *)c)->c,
        &((struct native_mutex *)m)->m,
        abs);
}

int
native_cond_signal(native_cond_t c)
{
    return sys_pthread_cond_signal(&((struct native_cond *)c)->c);
}

int
native_cond_broadcast(native_cond_t c)
{
    return sys_pthread_cond_broadcast(&((struct native_cond *)c)->c);
}

int
native_key_create(native_key_t *key, native_destructor_t dtor)
{
    pthread_key_t k;
    int rc = sys_pthread_key_create(&k, dtor);
    if (rc == 0)
        *key = (native_key_t)k;
    return rc;
}

int
native_setspecific(native_key_t key, const void *value)
{
    return sys_pthread_setspecific((pthread_key_t)key, value);
}

void *
native_getspecific(native_key_t key)
{
    return sys_pthread_getspecific((pthread_key_t)key);
}

int
native_nanosleep(const struct timespec *req, struct timespec *rem)
{
    return nanosleep(req, rem);
}
