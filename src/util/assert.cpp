/** @file
 * Util - Assertions.
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

#include <cassert>

#include <openwsp/misc.h>
#include <openwsp/types.h>
#include <openwsp/assert.h>


/**
 * Report a assertion failure.
 *
 * @param   file       Filename of source.
 * @param   line       The line failure located at
 * @param   func       Function name
 * @param   expr       Expression string
 */
void wpAssertFailure(const char* file, int line, const char* func, const char*expr) {

}

/**
 * Report a assertion failure with message.
 *
 * @param   format    Printf like format string.
 * @param   ...       Arguments to that string.
 */
void wpAssertFailureLog(const char* format,...) {

}
