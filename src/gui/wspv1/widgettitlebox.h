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

#ifndef GUI_WIDGETTITLEBOX_H_
#define GUI_WIDGETTITLEBOX_H_

#include <string>

namespace gui {

class WidgetTitlebox {
public:
    WidgetTitlebox();
    WidgetTitlebox(const char *text);

    void setText(const char *text);
    void onPaint();
private:
    std::string m_text;
    const void *m_icon;
};

} // namespace gui

#endif //!defined(GUI_WIDGETTITLEBOX_H_)
