#if !defined(_DCE_H)
#define _DCE_H

#include <endian.h>
#include <stdint.h>

#define FALSE 0
#define TRUE  1

#if !defined(MIN)
#  define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#if !defined(MAX)
#  define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#if defined(__STDC__)
#  define _DCE_PROTO_
#endif
#if defined(_DCE_PROTO_)
#  define _DCE_PROTOTYPE_(arg) arg
#else
#  define _DCE_PROTOTYPE_(arg) ()
#endif

#if defined(__STDC__)
#  define _DCE_VOID_
#endif
#if defined(_DCE_VOID_)
  typedef void * pointer_t;
#else
  typedef char * pointer_t;
#endif

#if defined(__STDC__)
#  define _DCE_TOKENCONCAT_
#endif
#if defined(_DCE_TOKENCONCAT_)
#  define DCE_CONCAT(a, b) a ## b
#else
#  define DCE_CONCAT(a, b) a/**/b
#endif

extern const char *dcelocal_path;
extern const char *dceshared_path;

#define DCE_DEBUG

#include <dce/nbase.h>

typedef idl_byte        byte;
typedef idl_boolean     boolean;

typedef struct unsigned64_s_t {
    uint32_t hi;
    uint32_t lo;
} unsigned64;

typedef struct signed64_s_t {
    uint32_t hi;
    uint32_t lo;
} signed64;

typedef struct unsigned48_s_t {
    uint32_t lo;
    uint16_t hi;
} unsigned48;

typedef struct unsigned128_s_t {
    uint32_t lolo;
    uint32_t lohi;
    uint32_t hilo;
    uint32_t hihi;
} unsigned128;

#endif /* _DCE_H */
