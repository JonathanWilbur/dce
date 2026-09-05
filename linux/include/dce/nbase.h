#ifndef _NBASE_H
#define _NBASE_H

#include <dce/idlbase.h>
#include <stdint.h>

typedef ndr_usmall_int	unsigned8;
typedef ndr_ushort_int	unsigned16;
typedef ndr_ulong_int	unsigned32;
typedef ndr_small_int	signed8;
typedef ndr_short_int	signed16;
typedef ndr_long_int	signed32;
typedef unsigned32	boolean32;
typedef unsigned32	error_status_t;
typedef unsigned char	unsigned_char_t;
typedef unsigned char	*unsigned_char_p_t;
typedef char		*char_p_t;

#ifndef error_status_ok
#define error_status_ok 0
#endif

typedef struct {
    unsigned32	time_low;
    unsigned16	time_mid;
    unsigned16	time_hi_and_version;
    unsigned8	clock_seq_hi_and_reserved;
    unsigned8	clock_seq_low;
    idl_byte	node[6];
} uuid_t, *uuid_p_t;

typedef struct {
    unsigned32	tower_length;
    idl_byte	*tower_octet_string;
} twr_t, *twr_p_t;

#define ndr_c_int_big_endian		0
#define ndr_c_int_little_endian		1
#define ndr_c_float_ieee		0
#define ndr_c_float_vax			1
#define ndr_c_float_cray		2
#define ndr_c_float_ibm			3
#define ndr_c_char_ascii		0
#define ndr_c_char_ebcdic		1

typedef struct {
    unsigned8	int_rep;
    unsigned8	char_rep;
    unsigned8	float_rep;
    idl_byte	reserved;
} ndr_format_t, *ndr_format_p_t;

typedef struct ndr_context_handle {
    unsigned32	context_handle_attributes;
    uuid_t	context_handle_uuid;
} ndr_context_handle;

#endif /* _NBASE_H */
