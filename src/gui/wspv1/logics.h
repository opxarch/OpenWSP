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

#ifndef GUI_LOGICS_H_
#define GUI_LOGICS_H_

#include "maindlg.h"
#include "volumedlg.h"

#include "eventfactory.h"

#define GUI_DOUBLE_CLICK_INTERVAL 500 // ms

namespace gui {

using openwsp::EventFactory;


/***************************************************
  *****      GUI Main logics object            *****
  ***************************************************/

class GuiMain {
public:
    GuiMain();
    ~GuiMain();
    void openCatelogDlg();
    void openChannelsDlg(int id);
    void loadChannel(int id, const std::string &title);
    void setVolume(int left, int right);
    void getVolume();

    void processRc(int rc);

private:
    int eventOpenCatelogDlg(void *opaque);
    int respOpenCatelogDlg(void *opaque, void *list);
    int eventOpenChannelsDlg(void *opaque, int id);
    int respOpenChannelsDlg(void *opaque, void *list);
    int eventLoadChannel(void *opaque, int id);
    int respLoadChannel(void *opaque, void *url);
    int eventSetVoulme(void *opaque, int left, int right);
    int eventGetVoulme(void *opaque);
    int respGetVoulme(void *opaque, int left, int right);

    int eventProcessRc(void *opaque, const void *err);

private:
    EventFactory<GuiMain> event;
};


extern MainDlg *g_maindlg;
extern VolumeDlg *g_volumedlg;

extern GuiMain *g_guimain;

inline MainDlg *mainDlg()
{
    return g_maindlg;
}

inline VolumeDlg *volumeDlg()
{
    return g_volumedlg;
}

inline GuiMain &gui()
{
    if (!g_guimain)
        g_guimain = new GuiMain; //fixme?
    return *g_guimain;
}

} //namespace gui


#endif //!defined(GUI_LOGICS_H_)
