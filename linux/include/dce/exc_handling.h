#ifndef EXC_HANDLING
#define EXC_HANDLING

/*
 * DCE TRY/CATCH exception package on Linux (PORTING_GUIDE.pdf §4.2,
 * exc_handling.h). Uses _setjmp/_longjmp as recommended in §4.2.5.
 */

#include <setjmp.h>
#include <stddef.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _EXC_VOLATILE_		volatile
#define _EXC_PROTO_
#define _EXC_IMPORT_		extern
#define _EXC_PROTOTYPE_(a)	a

typedef void *exc_address_t;
typedef long int exc_int_t;

typedef enum EXC_KIND_T {
    exc_kind_address_c	= 0x02130455,
    exc_kind_status_c	= 0x02130456
} exc_kind_t;

typedef struct EXC_EXT_T {
    exc_int_t		sentinel;
    exc_int_t		version;
    exc_address_t	extend;
    unsigned int	*args;
} exc_ext_t;

typedef struct EXC_KIND_ADDRESS_T {
    exc_kind_t		kind;
    exc_address_t	address;
    exc_ext_t		ext;
} exc_kind_address_t;

typedef struct EXC_KIND_STATUS_T {
    exc_kind_t		kind;
    exc_int_t		status;
    exc_ext_t		ext;
} exc_kind_status_t;

typedef union EXC_EXCEPTION_T {
    exc_kind_t		kind;
    exc_kind_status_t	status;
    exc_kind_address_t	address;
} EXCEPTION;

typedef enum EXC_STATE_T {
    exc_active_c	= 0,
    exc_none_c		= 1,
    exc_handled_c	= 2,
    exc_popped_c	= 3
} exc_state_t;

#define exc_excargs_c	40
#define exc_newexc_c	0x45586732
#define exc_v2exc_c	2
#define exc_v2ctx_c	2

typedef jmp_buf cma__t_jmp_buf;

typedef struct EXC_CONTEXT_T {
    cma__t_jmp_buf	jmp;
    _EXC_VOLATILE_ struct EXC_CONTEXT_T *link;
    EXCEPTION		cur_exception;
    exc_state_t		exc_state;
    exc_int_t		sentinel;
    exc_int_t		version;
    unsigned int	exc_args[exc_excargs_c];
} exc_context_t;

extern void exc_push_ctx(_EXC_VOLATILE_ exc_context_t *cb);
extern void exc_pop_ctx(_EXC_VOLATILE_ exc_context_t *cb);
extern void exc_raise(EXCEPTION *exc);
extern void exc_raise_status(exc_int_t status);
extern void exc_report(EXCEPTION *exc);

#define EXCEPTION_INIT(e)   (	\
    (e).address.address = (exc_address_t)&(e),	\
    (e).address.kind = exc_kind_address_c, \
    (e).address.ext.sentinel = exc_newexc_c, \
    (e).address.ext.version = exc_v2exc_c, \
    (e).address.ext.extend = (exc_address_t)0, \
    (e).address.ext.args = (unsigned int *)0)

#define exc_set_status(e,s) ( \
    (e)->status.status = (s), \
    (e)->status.kind = exc_kind_status_c)

#define exc_get_status(e,s) ( \
    (e)->kind == exc_kind_status_c ? \
	(*(s) = (e)->status.status, 0) : \
	-1)

#define exc_matches(e1,e2) \
    ((e1)->kind == (e2)->kind \
    && (e1)->address.address == (e2)->address.address)

#define RAISE(e) exc_raise(&(e))

#define exc_establish(_exc_ctx_)
#define exc_unestablish(_exc_ctx_)

#define exc_setjmp(j)	_setjmp(*(jmp_buf *)(j))

#define TRY \
    { \
	_EXC_VOLATILE_ exc_context_t exc_ctx; \
	exc_ctx.sentinel = exc_newexc_c; \
	exc_ctx.version = exc_v2ctx_c; \
	exc_ctx.exc_args[0] = 0; \
	exc_push_ctx(&exc_ctx); \
	exc_establish(&exc_ctx); \
	if (!exc_setjmp(exc_ctx.jmp)) {

#define CATCH(e) \
	    } \
	    else if (exc_matches(&exc_ctx.cur_exception, &(e))) { \
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&exc_ctx.cur_exception; \
		exc_ctx.exc_state = exc_handled_c;

#define CATCH_ALL \
	    } \
	    else { \
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&exc_ctx.cur_exception; \
		exc_ctx.exc_state = exc_handled_c;

#define RERAISE exc_raise(THIS_CATCH)

#define FINALLY   } \
	if (exc_ctx.exc_state == exc_none_c) \
	    exc_pop_ctx(&exc_ctx); \
	{

#define ENDTRY \
	} \
    exc_unestablish(&exc_ctx); \
    if (exc_ctx.exc_state == exc_none_c \
	    || exc_ctx.exc_state == exc_active_c) { \
	exc_pop_ctx(&exc_ctx); \
	} \
    }

#ifndef _CMA_SUPPRESS_EXTERNALS_
_EXC_IMPORT_ EXCEPTION
    exc_e_uninitexc,
    pthread_cancel_e,
    pthread_badparam_e,
    pthread_exists_e,
    pthread_in_use_e,
    pthread_notstack_e,
    pthread_unimp_e,
    pthread_inialrpro_e,
    exc_e_illaddr,
    exc_e_exquota,
    exc_e_insfmem,
    exc_e_nopriv,
    exc_e_uninitexc,
    pthread_use_error_e,
    pthread_invalid_e;
#endif

#ifdef __cplusplus
}
#endif

#endif /* EXC_HANDLING */
