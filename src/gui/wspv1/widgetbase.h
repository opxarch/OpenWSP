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

#ifndef GUI_WIDGETBASE_H_
#define GUI_WIDGETBASE_H_

namespace gui {

class WidgetBase {
public:
    WidgetBase(int x, int y, int width, int height) {
        resize(x, y, width, height);
    }

    virtual ~WidgetBase() {}

    virtual void resize(int x, int y, int width, int height) {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
    }

    virtual void onPaint() = 0;

protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    inline int getX() { return m_x; }
    inline int getY() { return m_y; }
    inline int getWidth() { return m_width; }
    inline int getHeight() { return m_height; }
};

} // namespace gui


#endif //!defined(GUI_WIDGETBASE_H_)
