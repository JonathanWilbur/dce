/*
 * Linux NDR scalar types.
 *
 * PORTING_GUIDE.pdf Table 5-3: ndr_long_int / ndr_ulong_int are 32 bits.
 * Linux LP64 makes `long` 64 bits, so these must not follow the AT386
 * ILP32 `long` mapping.
 */
#ifndef _NDR_TYPES_H
#define _NDR_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef unsigned char		ndr_boolean;
#define ndr_false		false
#define ndr_true		true

typedef unsigned char		ndr_byte;
typedef unsigned char		ndr_char;
typedef signed char		ndr_small_int;
typedef unsigned char		ndr_usmall_int;
typedef int16_t			ndr_short_int;
typedef uint16_t		ndr_ushort_int;
typedef int32_t			ndr_long_int;
typedef uint32_t		ndr_ulong_int;

struct ndr_hyper_int_rep_s_t {
    ndr_ulong_int low;
    ndr_long_int high;
};
typedef struct ndr_hyper_int_rep_s_t ndr_hyper_int;

struct ndr_uhyper_int_rep_s_t {
    ndr_ulong_int low;
    ndr_ulong_int high;
};
typedef struct ndr_uhyper_int_rep_s_t ndr_uhyper_int;

typedef float			ndr_short_float;
typedef double			ndr_long_float;

#endif /* _NDR_TYPES_H */
