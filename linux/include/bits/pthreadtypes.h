#ifndef _BITS_PTHREADTYPES_COMMON_H
#define _BITS_PTHREADTYPES_COMMON_H 1
#endif
#ifndef _BITS_PTHREADTYPES_H
#define _BITS_PTHREADTYPES_H 1
#endif
/*
 * Glibc's <sys/types.h> and <signal.h> include this file and define POSIX
 * pthread_t / pthread_mutex_t. The DCE draft-4 API uses the same names with
 * different types (PORTING_GUIDE.pdf ch. 4). Provide an empty stand-in so
 * linux/include/dce/pthread.h can define the DCE types.
 *
 * native_pthreads.c is compiled without -Ilinux/include, so it still sees
 * the real glibc header.
 */
