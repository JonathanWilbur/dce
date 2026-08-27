#ifndef LINUX_NATIVE_PTHREADS_H
#define LINUX_NATIVE_PTHREADS_H

/*
 * Thin wrappers around Linux NPTL so dcethreads.c can implement the
 * DCE draft-4 pthread API without including the system pthread.h
 * (the two APIs share names and differ in signatures).
 */

#include <stddef.h>
#include <time.h>

typedef unsigned long native_thread_t;
typedef void *native_mutex_t;
typedef void *native_cond_t;
typedef void *native_attr_t;
typedef unsigned int native_key_t;
typedef void (*native_destructor_t)(void *);

int native_threads_init(void);

int native_thread_create(native_thread_t *tid, native_attr_t attr,
    void *(*start)(void *), void *arg);
int native_thread_join(native_thread_t tid, void **retval);
int native_thread_detach(native_thread_t tid);
native_thread_t native_thread_self(void);
int native_thread_equal(native_thread_t a, native_thread_t b);
int native_thread_cancel(native_thread_t tid);
void native_thread_testcancel(void);
int native_thread_setcancelstate(int enable);
int native_thread_setcanceltype_async(int async);
void native_thread_exit(void *retval);
int native_thread_yield(void);

native_attr_t native_attr_new(void);
void native_attr_free(native_attr_t attr);
int native_attr_setstacksize(native_attr_t attr, size_t size);
size_t native_attr_getstacksize(native_attr_t attr);

native_mutex_t native_mutex_new(int recursive);
int native_mutex_free(native_mutex_t m);
int native_mutex_lock(native_mutex_t m);
int native_mutex_trylock(native_mutex_t m);
int native_mutex_unlock(native_mutex_t m);

native_cond_t native_cond_new(void);
int native_cond_free(native_cond_t c);
int native_cond_wait(native_cond_t c, native_mutex_t m);
int native_cond_timedwait(native_cond_t c, native_mutex_t m,
    const struct timespec *abs);
int native_cond_signal(native_cond_t c);
int native_cond_broadcast(native_cond_t c);

int native_key_create(native_key_t *key, native_destructor_t dtor);
int native_setspecific(native_key_t key, const void *value);
void *native_getspecific(native_key_t key);

int native_nanosleep(const struct timespec *req, struct timespec *rem);

#endif
