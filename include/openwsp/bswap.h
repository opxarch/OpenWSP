/** @file
 * Openwsp - Swapping word order.
 */

/*
 *  OpenWSP (an opensource webstream codec)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *  please contact with <diyer175@hotmail.com> if you have any problems.
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef OPENWSP_BSWAP_H_
#define OPENWSP_BSWAP_H_

#include <openwsp/misc.h>
#include <openwsp/types.h>

#define BSWAP16C(x) (((x) << 8 & 0xff00)  | ((x) >> 8 & 0x00ff))
#define BSWAP32C(x) (BSWAP16C(x) << 16 | BSWAP16C((x) >> 16))
#define BSWAP64C(x) (BSWAP32C(x) << 32 | BSWAP32C((x) >> 32))

#define BSWAPC(s, x) BSWAP##s##C(x)

#ifndef bswap16
static inline uint16_t bswap16(uint16_t x) {
    x= (x>>8) | (x<<8);
    return x;
}
#endif

#ifndef bswap32
static inline uint32_t bswap32(uint32_t x)
{
    x= ((x<<8)&0xFF00FF00) | ((x>>8)&0x00FF00FF);
    x= (x>>16) | (x<<16);
    return x;
}
#endif

#ifndef bswap64
static inline uint64_t bswap64(uint64_t x)
{
#if 0
    x= ((x<< 8)&0xFF00FF00FF00FF00ULL) | ((x>> 8)&0x00FF00FF00FF00FFULL);
    x= ((x<<16)&0xFFFF0000FFFF0000ULL) | ((x>>16)&0x0000FFFF0000FFFFULL);
    return (x>>32) | (x<<32);
#else
    union {
        uint64_t ll;
        uint32_t l[2];
    } w, r;
    w.ll = x;
    r.l[0] = bswap32 (w.l[1]);
    r.l[1] = bswap32 (w.l[0]);
    return r.ll;
#endif
}
#endif

// be2ne ... big-endian to native-endian
// le2ne ... little-endian to native-endian

#if HAVE(BIGENDIAN)
#define be2ne16(x) (x)
#define be2ne32(x) (x)
#define be2ne64(x) (x)
#define le2ne16(x) bswap16(x)
#define le2ne32(x) bswap32(x)
#define le2ne64(x) bswap64(x)
#define BE2NEC(s, x) (x)
#define LE2NEC(s, x) BSWAPC(s, x)
#else
#define be2ne16(x) bswap16(x)
#define be2ne32(x) bswap32(x)
#define be2ne64(x) bswap64(x)
#define le2ne16(x) (x)
#define le2ne32(x) (x)
#define le2ne64(x) (x)
#define BE2NEC(s, x) BSWAPC(s, x)
#define LE2NEC(s, x) (x)
#endif


// ne2be ... native-endian to big-endian
// ne2le ... native-endian to little-endian

#if HAVE(BIGENDIAN)
#define ne2be16(x) (x)
#define ne2be32(x) (x)
#define ne2be64(x) (x)
#define ne2le16(x) bswap16(x)
#define ne2le32(x) bswap32(x)
#define ne2le64(x) bswap64(x)
#define NE2BEC(s, x) (x)
#define NE2LEC(s, x) BSWAPC(s, x)
#else
#define ne2be16(x) bswap16(x)
#define ne2be32(x) bswap32(x)
#define ne2be64(x) bswap64(x)
#define ne2le16(x) (x)
#define ne2le32(x) (x)
#define ne2le64(x) (x)
#define NE2BEC(s, x) BSWAPC(s, x)
#define NE2LEC(s, x) (x)
#endif

#define BE2NE16C(x) BE2NEC(16, x)
#define BE2NE32C(x) BE2NEC(32, x)
#define BE2NE64C(x) BE2NEC(64, x)
#define LE2NE16C(x) LE2NEC(16, x)
#define LE2NE32C(x) LE2NEC(32, x)
#define LE2NE64C(x) LE2NEC(64, x)

#define NE2BE16C(x) NE2BEC(16, x)
#define NE2BE32C(x) NE2BEC(32, x)
#define NE2BE64C(x) NE2BEC(64, x)
#define NE2LE16C(x) NE2LEC(16, x)
#define NE2LE32C(x) NE2LEC(32, x)
#define NE2LE64C(x) NE2LEC(64, x)

/*@todo */
#if HAVE(BIGENDIAN)
#define be2me_16(x) (x)
#define be2me_32(x) (x)
#define be2me_64(x) (x)
#define le2me_16(x) bswap16(x)
#define le2me_32(x) bswap32(x)
#define le2me_64(x) bswap64(x)
#else
#define be2me_16(x) bswap16(x)
#define be2me_32(x) bswap32(x)
#define be2me_64(x) bswap64(x)
#define le2me_16(x) (x)
#define le2me_32(x) (x)
#define le2me_64(x) (x)
#endif

#endif //!defined(OPENWSP_BSWAP_H_)
