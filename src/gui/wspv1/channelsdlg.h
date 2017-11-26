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

#ifndef GUI_CHANNELSDLG_H_
#define GUI_CHANNELSDLG_H_

#include <map>
#include <vector>
#include <openwsp/list.h>

#include "eventfactory.h"

#include "GUI.h"
#include "LISTBOX.h"

namespace gui {

using openwsp::EventFactory;

class ChannelsDlg {
public:
    ChannelsDlg();
    int create();
    int destroy();
    void minimize();
    void restore();

    void clearList();
    void insertListItem(class openwsp::channel *ch);

    void handle_cbFrameWin(WM_MESSAGE* pMsg);
    int handle_cbOwnerDraw(const void * pDrawItemInfo);

private:
    void onButtonOpen();
    void onClose();
    void onListChannelsSelChanged();
    void onListChannelsDbClick();

private:
    FRAMEWIN_Handle hFrameMain;
    WM_HWIN hListBoxChannels, hSliderV, hSliderH;

    std::map<int, class openwsp::channel *> mapIndex;

    int m_listClickTime;
    int m_listClickIndex;
    int m_listsel;

    EventFactory<ChannelsDlg> event;
};

} // namespace gui

#endif //!defined(GUI_CHANNELSDLG_H_)
