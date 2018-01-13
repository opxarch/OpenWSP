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

#ifndef OPENWSP_STRING_H_
#define OPENWSP_STRING_H_

#include <openwsp/misc.h>

namespace openwsp {

int compareString(const char *src, const char *dest, int len);
int compareStringNocase(const char *src, const char *dest, int len);

int stringToInteger(const char *src, int len, int *val);
int stringToFloat(const char *src, int len, float *val);
int stringToBoolean(const char *src, int len, bool *bl);
char *stringDuplicate(const char *src);
int stringDuplicateEx(const char *src, int len, char **dest);

int integerToString(int src, int radix, char *buff, int len, int *outlen);
int floatToString(float src, int radix, long precision, char *buff, int len, int *outlen);
int booleanToString(bool src, char *buff, int len, int *outlen);

void *memcpyLimited(void *dst, const void *src, size_t size, size_t maxsize);

int encodeString(char *src, const char *fromcode, const char *tocode, char *buff, size_t buffsize);

} // namespace openwsp

// C runtime
#include <cstring>

#endif //!defined(OPENWSP_STRING_H_)
