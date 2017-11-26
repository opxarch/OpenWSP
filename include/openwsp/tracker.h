/** @file
 * openwsp - Source-code-level tracker
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

#ifndef OPENWSP_TRAKCER_H_
#define OPENWSP_TRAKCER_H_

#include <openwsp/misc.h>

namespace openwsp {

/***************************************************
  *****         Locator object                 *****
  ***************************************************/

class Locator {
public:
    Locator();
    Locator(const char *file, int line, const char *func);
    Locator(const Locator &src);
    Locator &operator =(const Locator &src);

    /**
     * Get the source filename.
     * @return pointer to the constant string buffer.
     */
    inline const char *GetFilename() const {
        return m_file;
    }

    /**
     * Get the source line number
     * @return line number
     */
    inline int GetLine() const {
        return m_line;
    }

    /**
     * Get the source function name.
     * @return pointer to the constant string buffer.
     */
    inline const char *GetFunctionName() const {
        return m_func;
    }

private:
    void copy(const Locator &src);

protected:
    const char *m_file;
    int         m_line;
    const char *m_func;
};

/**@def MakeLocater
 * Create a Locater anonymous object, using the compiling-time information.
 */
#define MakeLocator() Locator(WS_CURRENT_FILE, WS_CURRENT_LINE, WS_CURRENT_FUNCTION)

} // namespace openwsp

#endif //!defined(OPENWSP_TRAKCER_H_)
