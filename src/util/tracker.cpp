/** @file
 * Util - Source-code-level tracker.
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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <openwsp/misc.h>

#include <openwsp/tracker.h>

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

Locator::Locator(const char *file, int line, const char *func)
    : m_file(file ? file : "(null)"),
      m_line(line),
      m_func(func ? func : "(null)")
{
}

Locator::Locator()
    : m_file("(unset)"),
      m_line(0),
      m_func("(unset")
{
}

Locator::Locator(const Locator &src) {
    copy(src);
}

Locator &Locator::operator =(const Locator &src) {
    copy(src);
    return (*this);
}

void Locator::copy(const Locator &src) {
    m_file = src.m_file;
    m_line = src.m_line;
    m_func = src.m_func;
}

} //namespace openwsp
