/** @file
 * all about webservice API file wrappers are here
 */

/*
 *  OpenWSP (an opensource web stream player)
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

#ifndef OPENWSP_APIFILE_H_
#define OPENWSP_APIFILE_H_

#include <openwsp/misc.h>
#include <openwsp/types.h>

/** magic number for apifile */
#define NUMBER_MAGIC_APIFILE   (0x46495041)

/** ascii base offset */
#define ASCII_BASE_OFFSET (0)

#pragma pack (push)
#pragma pack (0)

/*
 API file structure
 */
typedef struct apifile_s {
    /** magic number (NUMBER_MAGIC_APIFILE) */
    uint32_t        magic;
    /** CRC checksum of code page data */
    uint32_t        codepage_checksum;
    /** Length of code page */
    uint32_t        codepage_len;
    /** offset in bytes (start from 0) */
    uint32_t        codepage_offset;

    /* {reservation} */
    /* {data area} */
} apifile_t;

#pragma pack (pop)

#define APIFILE_SUFFIX ".api"

#endif //!defined(OPENWSP_APIFILE_H_)
