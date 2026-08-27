#ifndef _UUID_H
#define _UUID_H

#include <dce/nbase.h>

#ifdef __cplusplus
extern "C" {
#endif

#define uuid_c_version		1
#define uuid_c_version_highest	2
#define uuid_s_ok		error_status_ok
#define uuid_s_internal_error		0x16c9a002
#define uuid_s_bad_version		0x16c9a003
#define uuid_s_invalid_string_uuid	0x16c9a008
#define uuid_s_no_memory		0x16c9a00a
#define uuid_s_coding_error		0x16c9a000

typedef struct {
    unsigned32	count;
    uuid_p_t	uuid[1];
} uuid_vector_t, *uuid_vector_p_t;

void uuid_create(uuid_t *uuid, unsigned32 *status);
void uuid_create_nil(uuid_t *uuid, unsigned32 *status);
void uuid_to_string(uuid_p_t uuid, unsigned_char_p_t *uuid_string,
    unsigned32 *status);
void uuid_from_string(unsigned_char_p_t uuid_string, uuid_t *uuid,
    unsigned32 *status);
boolean32 uuid_equal(uuid_p_t uuid1, uuid_p_t uuid2, unsigned32 *status);
boolean32 uuid_is_nil(uuid_p_t uuid, unsigned32 *status);
signed32 uuid_compare(uuid_p_t uuid1, uuid_p_t uuid2, unsigned32 *status);
unsigned16 uuid_hash(uuid_p_t uuid, unsigned32 *status);

#ifdef __cplusplus
}
#endif

#endif /* _UUID_H */
