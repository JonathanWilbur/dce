/*
 * UUID library for the Linux port (uuid.idl).
 * RFC 4122 version-4 UUIDs via getrandom(); string format matches DCE.
 */

#include <dce/uuid.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

static const uuid_t uuid_nil;

static void
random_bytes(void *buf, size_t n)
{
    unsigned char *p = buf;
    size_t got = 0;
    while (got < n) {
        ssize_t r = getrandom(p + got, n - got, 0);
        if (r < 0)
            break;
        got += (size_t)r;
    }
}

void
uuid_create(uuid_t *uuid, unsigned32 *status)
{
    random_bytes(uuid, sizeof(*uuid));
    /* RFC 4122 version 4 */
    uuid->time_hi_and_version =
        (unsigned16)((uuid->time_hi_and_version & 0x0fff) | 0x4000);
    uuid->clock_seq_hi_and_reserved =
        (unsigned8)((uuid->clock_seq_hi_and_reserved & 0x3f) | 0x80);
    *status = uuid_s_ok;
}

void
uuid_create_nil(uuid_t *uuid, unsigned32 *status)
{
    memset(uuid, 0, sizeof(*uuid));
    *status = uuid_s_ok;
}

void
uuid_to_string(uuid_p_t uuid, unsigned_char_p_t *uuid_string,
    unsigned32 *status)
{
    char *s = malloc(37);
    if (s == NULL) {
        *status = uuid_s_no_memory;
        *uuid_string = NULL;
        return;
    }
    sprintf(s,
        "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        uuid->time_low, uuid->time_mid, uuid->time_hi_and_version,
        uuid->clock_seq_hi_and_reserved, uuid->clock_seq_low,
        uuid->node[0], uuid->node[1], uuid->node[2],
        uuid->node[3], uuid->node[4], uuid->node[5]);
    *uuid_string = (unsigned_char_p_t)s;
    *status = uuid_s_ok;
}

static int
hexnibble(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

static int
parse_hex(const char **pp, int n, unsigned int *out)
{
    unsigned int v = 0;
    int i;
    for (i = 0; i < n; i++) {
        int h = hexnibble((unsigned char)*(*pp)++);
        if (h < 0)
            return -1;
        v = (v << 4) | (unsigned)h;
    }
    *out = v;
    return 0;
}

void
uuid_from_string(unsigned_char_p_t uuid_string, uuid_t *uuid,
    unsigned32 *status)
{
    const char *p;
    unsigned int v;

    if (uuid_string == NULL || uuid_string[0] == '\0') {
        memset(uuid, 0, sizeof(*uuid));
        *status = uuid_s_ok;
        return;
    }
    p = (const char *)uuid_string;
    if (parse_hex(&p, 8, &v) || *p++ != '-')
        goto bad;
    uuid->time_low = v;
    if (parse_hex(&p, 4, &v) || *p++ != '-')
        goto bad;
    uuid->time_mid = (unsigned16)v;
    if (parse_hex(&p, 4, &v) || *p++ != '-')
        goto bad;
    uuid->time_hi_and_version = (unsigned16)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->clock_seq_hi_and_reserved = (unsigned8)v;
    if (parse_hex(&p, 2, &v) || *p++ != '-')
        goto bad;
    uuid->clock_seq_low = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[0] = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[1] = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[2] = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[3] = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[4] = (unsigned8)v;
    if (parse_hex(&p, 2, &v))
        goto bad;
    uuid->node[5] = (unsigned8)v;
    if (*p != '\0')
        goto bad;
    *status = uuid_s_ok;
    return;
bad:
    *status = uuid_s_invalid_string_uuid;
}

boolean32
uuid_equal(uuid_p_t uuid1, uuid_p_t uuid2, unsigned32 *status)
{
    *status = uuid_s_ok;
    return memcmp(uuid1, uuid2, sizeof(uuid_t)) == 0;
}

boolean32
uuid_is_nil(uuid_p_t uuid, unsigned32 *status)
{
    *status = uuid_s_ok;
    return memcmp(uuid, &uuid_nil, sizeof(uuid_t)) == 0;
}

signed32
uuid_compare(uuid_p_t uuid1, uuid_p_t uuid2, unsigned32 *status)
{
    int c;
    *status = uuid_s_ok;
    c = memcmp(uuid1, uuid2, sizeof(uuid_t));
    if (c < 0)
        return -1;
    if (c > 0)
        return 1;
    return 0;
}

unsigned16
uuid_hash(uuid_p_t uuid, unsigned32 *status)
{
    const unsigned char *p = (const unsigned char *)uuid;
    unsigned h = 0;
    size_t i;
    *status = uuid_s_ok;
    for (i = 0; i < sizeof(uuid_t); i++)
        h = h * 31u + p[i];
    return (unsigned16)h;
}
