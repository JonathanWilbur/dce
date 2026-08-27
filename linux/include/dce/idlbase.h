#ifndef _IDLBASE_H
#define _IDLBASE_H

#include <dce/ndrtypes.h>
#include <stddef.h>

#ifndef HAS_GLOBALDEFS
#define globaldef
#define globalref extern
#endif

typedef ndr_boolean	idl_boolean;
typedef ndr_byte	idl_byte;
typedef ndr_char	idl_char;
typedef ndr_small_int	idl_small_int;
typedef ndr_usmall_int	idl_usmall_int;
typedef ndr_short_int	idl_short_int;
typedef ndr_ushort_int	idl_ushort_int;
typedef ndr_long_int	idl_long_int;
typedef ndr_ulong_int	idl_ulong_int;
typedef ndr_hyper_int	idl_hyper_int;
typedef ndr_uhyper_int	idl_uhyper_int;
typedef ndr_short_float	idl_short_float;
typedef ndr_long_float	idl_long_float;

typedef size_t		idl_size_t;
typedef void		*idl_void_p_t;

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#endif /* _IDLBASE_H */
