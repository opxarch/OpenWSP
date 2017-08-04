/** @file
 * Openwsp - string operation.
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

/*******************************************************************************
 *   Header Files                                                               *
 *******************************************************************************/
#include <string.h>
#include <new>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/string.h>

namespace openwsp {

/**
 * Convert the string to integer.
 *
 * The process of conversation will stop if it meets
 * any non-number characters, expect '+' or '-' that can
 * sign positive and negative.
 *
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma val           Where to store the result.
 */
int stringToInteger(const char *src, int len, int *val) {
    WS_ASSERT(src && len && val);
    bool minus = false;
    int res = 0;
    for (int i = 0; i < len; i++) {
        if (src[i] == '-') {
            minus = true;
            continue;
        }
        if (src[i] == ' ')
            continue;
        if ('0' > src[i] || src[i] > '9')
            break;
        else {
            res = res * 10 + (src[i] - '0');
            continue;
        }
    }
    *val = minus ? -res : res;
    return WINF_SUCCEEDED;
}

/**
 * Convert the string to float.
 *
 * The process of conversation will stop if it meets
 * any non-number characters, expect '+' or '-' that can
 * sign positive and negative.
 *
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma val           Where to store the result.
 */
int stringToFloat(const char *src, int len, float *val) {
    WS_ASSERT(src && len && val);
    int i;
    double d = 0.0,power = 1.0;
    int sign = 1;
    for (i = 0; i < len; i++) {
        if (src[i] == ' ') continue;
        if (src[i] == '-') { sign = -1; continue; }
        if (src[i] == '+') { sign = 1; continue; }
        break;
    }
    if (i == (len - 1))
        return WERR_FAILED;
    // read the integer part
    for (; i < len; i++) {
      if ((src[i] >= '0')&&(src[i] <= '9'))
        d = d* 10.0 + (src[i] - '0');
      else if (src[i] == '.') {
        // read the float part
        for (i++; i < len; ++i) {
          if ((src[i] >= '0')&&(src[i] <= '9')) {
            d = d* 10.0 + (src[i] - '0');
            power *= 10;
          } else
            break;
        }
        break;
      } else
         break;
    }
    *val = sign*d/power;
    return WINF_SUCCEEDED;
}

/**
 * Convert the string to boolean.
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma bl            Where to store the result.
 */
int stringToBoolean(const char *src, int len, bool *bl) {
    WS_ASSERT(src && len && bl);
    int istrue = compareStringNocase(src, "true", sizeof("true")) == 0;
    int isfalse = compareStringNocase(src, "false", sizeof("false")) == 0;

    if (istrue || isfalse) {
        *bl = istrue;
        return WINF_SUCCEEDED;
    } else
        return WERR_FAILED;
}

/**
 * Convert the integer to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source integer value.
 * @param radix         The radix by which the integer is shown in string.
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int integerToString(int src, int radix, char *buff, int len, int *outlen) {
    WS_ASSERT(buff && len);
    int wlen = 0;
    char *p;
    char *firstdig;
    unsigned digval;
    if (len < 2)
        return WERR_BUFFER_OVERFLOW;
    p = buff;
    // process the sign
    if (src < 0) {
        *p++ = '-';
        src = (unsigned int)(-(int)src);
    }
    firstdig = p;
    // do the conversation
    wlen = 1;
    do {
        digval = (unsigned)(src % radix);
        src /= radix;
        if (digval > 9)
             *p++ = (char)(digval - 10 + 'a');
        else *p++ = (char)(digval + '0');
        if (wlen++ > len)
            return WERR_BUFFER_OVERFLOW;
    } while (src > 0);

    if (outlen)
        *outlen = wlen;
    if (wlen > len)
        return WERR_BUFFER_OVERFLOW;
    else {
        // reserve all
        char tmp;
        *p-- = '\0';
        do{ tmp = *p;
            *p = *firstdig;
            *firstdig = tmp;
            --p;
            ++firstdig;
        } while (firstdig < p);
    }
    return WINF_SUCCEEDED;
}

/**
 * Convert the float to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source integer value.
 * @param radix         The radix by which the integer is shown in string.
 *                      A float number can be ff.ff(16h) or 17.17(10h).
 * @param prec          The precision. Must be counted in power. eg. 1000(10^3)
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int floatToString(float src, int radix, long precision, char *buff, int len, int *outlen) {
    WS_ASSERT(buff && len);
    int rc;
    int outl = 0, outl1 = 0;
    int int_part = (int)src;
    int float_part = (int) ((src - (float)int_part) * (float)precision);
    // convert the integer part
    rc = integerToString(int_part, radix, buff, len, &outl);
    if (WS_FAILURE(rc))
        return rc;
    buff += outl;
    buff--; // to skip '\0' that counted in outl.
    len -= outl;
    // insert point
    *(buff++) = '.';
    outl++;
    len--;
    if (len < 0)
        return WERR_BUFFER_OVERFLOW;
    // convert the float part
    rc = integerToString(float_part, radix, buff, len, &outl1);
    if (WS_FAILURE(rc))
        return rc;
    if (outlen)
        *outlen = outl + outl1;
    return WINF_SUCCEEDED;
}

/**
 * Convert the boolean to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source boolean value.
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int booleanToString(bool src, char *buff, int len, int *outlen) {
    WS_ASSERT(buff && len);
    int wlen = src?     sizeof("true") : sizeof("false");
    const char *dispaly = src? "true" :         "false";
    if (wlen > len)
        return WERR_BUFFER_OVERFLOW;
    else {
        memcpy(buff, dispaly, wlen);
        if (outlen)
            *outlen = wlen;
        return WINF_SUCCEEDED;
    }
}

/**
 * Allocate the heap memory and copy the string.
 * This function is NOT compatible with C malloc/free,
 * and we should release it with C++ delete.
 * @return status
 * @param src           Pointer to the source string.
 * @param len           Length of string. 0 if the string is
 *                      terminated by '\0' char.
 * @param dest          Where to store the result.
 */
int stringDuplicate(const char *src, int len, char **dest) {
    WS_ASSERT(src && dest);
    if (!len)
        len = strlen(src);
    *dest = new (std::nothrow) char[len];
    if (!*dest)
        return WERR_ALLOC_MEMORY;
    memcpy(*dest, src, len);
    return WINF_SUCCEEDED;
}

/**
 * Comapre the two string.
 * @returns 0 if the two string is identical.
 * @returns (ASCII of src) - (ASCII of dest) if not identical.
 * @param src           Pointer to the source string.
 * @param dest          Pointer to the tareget string.
 * @param len           How many bytes to comapre.
 */
int compareString(const char *src, const char *dest, int len) {
    WS_ASSERT(src && dest);
    register const char* fini = src + len;
    while (src < fini) {
        int res = *src - *dest;
        if (res)
            return res;
        if (!*src || !*dest)
            return 0;
        ++src;
        ++dest;
    }
    return 0;
}

/**
 * Comapre the two string, don't consider the upper or lower of characters.
 * @returns 0 if the two string is identical.
 * @returns (ASCII of src) - (ASCII of dest) if not identical.
 * @param src           Pointer to the source string.
 * @param dest          Pointer to the tareget string.
 * @param len           How many bytes to comapre.
 */
int compareStringNocase(const char *src, const char *dest, int len) {
    WS_ASSERT(src && dest);
    register unsigned char ch1, ch2;
    register const char* fini = src + len;
    while (src < fini) {
        if (((ch1 = (unsigned char)(*(dest++))) >= 'A') && (ch1 <= 'Z'))
            ch1 += 0x20; // to lower
        if (((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z'))
            ch2 += 0x20;
        int res = ch1 - ch2;
        if (res)
            return res;
        if (!*src || !*dest)
            return 0;
        ++src;
        ++dest;
    }
    return 0;
}

} // namespace openwsp
