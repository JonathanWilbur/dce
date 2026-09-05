/*
 * Local NDR representation for Linux (little-endian IEEE ASCII).
 * Closest reference platform: AT386 (PORTING_GUIDE.pdf ch. 5 / 12).
 */
#ifndef _NDR_REP_H
#define _NDR_REP_H

#define NDR_LOCAL_INT_REP     ndr_c_int_little_endian
#define NDR_LOCAL_FLOAT_REP   ndr_c_float_ieee
#define NDR_LOCAL_CHAR_REP    ndr_c_char_ascii

#endif /* _NDR_REP_H */
