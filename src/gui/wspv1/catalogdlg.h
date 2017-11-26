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

#ifndef GUI_CATALOGDLG_H_
#define GUI_CATALOGDLG_H_

#include <map>

#include "GUI.h"

#include "eventfactory.h"

namespace gui {

using openwsp::EventFactory;

class CatalogDlg {
public:
    CatalogDlg();
    int create();
    int destroy();
    void minimize();
    void restore();

    void clearList();
    void insertListItem(const char *name, const char *desc, const char *catalog, int id);

    void handle_cbFrameWin(WM_MESSAGE* pMsg);

private:
    void onListCatalogSelChanged();
    void onButtonOpen();
    void onClose();
    int eventCloseDlg(void *opaque, CatalogDlg *dlg);

private:
    FRAMEWIN_Handle hFrameMain;
    WM_HWIN hListViewCatalog, hSliderV, hSliderH;

    std::map<int, int> mapIndex;

    int m_listsel;

    EventFactory<CatalogDlg> event;
};

} // namespace gui

#endif //!defined(GUI_CATALOGDLG_H_)
