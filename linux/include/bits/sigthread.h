#ifndef _BITS_SIGTHREAD_H
#define _BITS_SIGTHREAD_H 1
/*
 * Glibc <signal.h> declares pthread_kill(pthread_t, ...) using POSIX
 * pthread_t. DCE pthread_t is a struct (draft-4). Omit those prototypes
 * when compiling DCE sources; native_pthreads.c does not use this stub.
 */
#endif
