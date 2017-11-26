/** @file
 * OpenWSP - GUI (Graphical User Interface).
 */

/*
 *  OpenWSP (an opensource webstream codec)
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

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <openwsp/misc.h>
#include "gui.h"

namespace gui {

/*******************************************************************************
*   Imported Functions                                                               *
*******************************************************************************/
extern int guiInvoke(GUI_OPCODE code, void *ptr);

////////////////////////////////////////////////////////////////////////////////

wsGUI::wsGUI() {

}

wsGUI::~wsGUI() {

}

int wsGUI::init() {
    return guiInvoke(GUI_OPCODE_INIT, 0);
}

int wsGUI::uninit() {
    return guiInvoke(GUI_OPCODE_UNINIT, 0);
}

int wsGUI::DoMessages() {
    return guiInvoke(GUI_OPCODE_DOMSG, 0);
}

} // namespace openwsp
