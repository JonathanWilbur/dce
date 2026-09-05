#ifndef CMA_CONFIG
#define CMA_CONFIG

/*
 * DCE Threads configuration for Linux (PORTING_GUIDE.pdf chapter 4).
 *
 * Chapter 4.1: use the platform's pthreads implementation when it is a
 * true kernel-thread POSIX library. Linux NPTL qualifies:
 *   _CMA_THREAD_IS_VP_ = 1, reentrant libc, thread-synchronous I/O.
 */

#define _CMA__CC		1
#define _CMA__CFRONT		2
#define _CMA__DECC		3
#define _CMA__DECCPLUS		4
#define _CMA__GCC		5
#define _CMA__GCPLUS		6
#define _CMA__SIEMENSC		7
#define _CMA__VAXC		8

#define _CMA__ALPHA		1
#define _CMA__CPLMIPS		2
#define _CMA__HPPA		3
#define _CMA__IBMR2		4
#define _CMA__MIPS		5
#define _CMA__MX300I		6
#define _CMA__M68K		7
#define _CMA__VAX		8
#define _CMA__I386		9
#define _CMA__X86_64		10

#define _CMA__BSD		1
#define _CMA__SVR4		2
#define _CMA__UNIX		3
#define _CMA__VMS		4

#define _CMA__OS_AIX		1
#define _CMA__OS_BSD		2
#define _CMA__OS_OSF		3
#define _CMA__OS_SYSV		4
#define _CMA__OS_VMS		5
#define _CMA__OS_LINUX		6

#define _CMA__APOLLO		1
#define _CMA__DIGITAL		2
#define _CMA__HP		3
#define _CMA__IBM		4
#define _CMA__OSF		5
#define _CMA__PTC		6
#define _CMA__SNI		7
#define _CMA__SUN		8
#define _CMA__GNU		9

#define _CMA__NONE		0
#define _CMA__MACH		1
#define _CMA__NPTL		2

#define _CMA__ALPHA_UNIX	1
#define _CMA__HPPA_UNIX		2
#define _CMA__IBMR2_UNIX	3
#define _CMA__MIPS_UNIX		4
#define _CMA__LINUX_X86_64	10
#define _CMA__LINUX_I386	11

#ifndef _CMA_COMPILER_
# define _CMA_COMPILER_		_CMA__GCC
#endif

#if defined(__x86_64__) || defined(__aarch64__)
# define _CMA_HARDWARE_		_CMA__X86_64
# define _CMA_PLATFORM_		_CMA__LINUX_X86_64
#elif defined(__i386__)
# define _CMA_HARDWARE_		_CMA__I386
# define _CMA_PLATFORM_		_CMA__LINUX_I386
#else
# define _CMA_HARDWARE_		_CMA__X86_64
# define _CMA_PLATFORM_		_CMA__LINUX_X86_64
#endif

#define _CMA_OS_		_CMA__UNIX
#define _CMA_OSIMPL_		_CMA__OS_LINUX
#define _CMA_VENDOR_		_CMA__GNU
#define _CMA_KTHREADS_		_CMA__NPTL
#define _CMA_MP_HARDWARE_	1
#define _CMA_MULTIPLEX_		0
#define _CMA_UNIPROCESSOR_	0
#define _CMA_THREAD_IS_VP_	1
#define _CMA_THREAD_SYNC_IO_	1
#define _CMA_REENTRANT_CLIB_	1
#define _CMA_NOWRAPPERS_	1
#define _CMA_PROTECT_MEMORY_	1
#define _CMA_SPINLOOP_		0
#define _CMA_VOID_		1
#define _CMA_VOLATILE_		volatile
#define _CMA_VSSCANF_		1
#define _CMA_PER_THD_SYNC_SIGS_	1

#ifndef _CMA_PROTO_
# define _CMA_PROTO_		1
#endif

#define _CMA_IMPORT_		extern
#define _CMA_EXPORT_
#define _CMA_EXPORT_FLAG_	0

#define _CMA_UNIX_TYPE		_CMA__SVR4

#endif /* CMA_CONFIG */
