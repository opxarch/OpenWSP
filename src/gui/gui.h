/*
 *  OpenWSP (an opensource web stream player)
 *  Copyleft (C) 2016, The 1st Middle School in Yongsheng Lijiang China
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

#ifndef OPENWSP_GUI_H_
#define OPENWSP_GUI_H_

namespace gui {

/***************************************************
  *****             GUI object                 *****
  ***************************************************/

class wsGUI {
public:
    wsGUI();
    ~wsGUI();

    int init();
    int uninit();

    int DoMessages();
};

/**
 * GUI opcode
 */
enum GUI_OPCODE {
    GUI_OPCODE_INVALID = 0,
    GUI_OPCODE_INIT,
    GUI_OPCODE_UNINIT,
    GUI_OPCODE_DOMSG
};

}; // namespace openwsp

#endif //!defined(OPENWSP_GUI_H_)
