/** @file
 * WSP-V1 - GUI (Graphical User Interface) logics.
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
#include <iostream>
#include <openwsp/misc.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>

extern "C" {
#include "LCDConf.h"
#include "GUI.h"
#include "FRAMEWIN.h"
}

#include "../gui.h"

/*******************************************************************************
*   Imported Functions                                                               *
*******************************************************************************/
#if OS(WIN32)
extern "C" int beginSIM();
extern "C" int endSIM();
extern int simEnded;
#endif

////////////////////////////////////////////////////////////////////////////////

namespace openwsp {

/**
 * Message callback of background
 */
static void _cbBkWindow(WM_MESSAGE* pMsg) {
    static GUI_MEMDEV_Handle _hBkMemDev;

    switch (pMsg->MsgId) {
        case WM_PAINT:
            // draw the background
            if (!_hBkMemDev) {
              GUI_MEMDEV_Handle hMemPrev;
              GUI_RECT r;
              r.x0 = 0;
              r.x1 = LCD_XSIZE - 1;
              r.y0 = 0;
              r.y1 = LCD_YSIZE - 1;
              _hBkMemDev = GUI_MEMDEV_CreateEx(r.x0, r.y0, r.x1 + 1, r.y1 + 1, GUI_MEMDEV_NOTRANS);

              hMemPrev = GUI_MEMDEV_Select(_hBkMemDev);
              GUI_SetBkColor(GUI_BLUE);
              GUI_Clear();
              GUI_MEMDEV_Select(hMemPrev);
            }
            GUI_MEMDEV_Write(_hBkMemDev);
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}

/**
 * Inner, Initialize the GUI.
 * @return status code.
 */
static int initGUI(void) {
    int r;
    r = GUI_Init();

    if (r)
        return WERR_FAILED;

    /* Use memory devices */
    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_EnableMemdev(WM_HBKWIN);

    /* Set callback for background */
    WM_SetCallback(WM_HBKWIN, &_cbBkWindow);

    GUI_CURSOR_Show();
    GUI_MessageBox("OpenWSP smart Radio v0.1","Version",0);

    return WINF_SUCCEEDED;
}

/**
 * Inner, dispatch & process the messages.
 * @return status code.
 */
static int doMessages() {
    int timeDelta;

    timeDelta = GUI_GetTime();
    GUI_Exec();
    timeDelta = 25 - (GUI_GetTime() - timeDelta);

    // exec idle
    if (timeDelta > 0) {
        GUI_Delay(timeDelta);
    }

#if OS(WIN32)
    if (simEnded)
        return WINF_TERMINATED;
#endif

    return WINF_SUCCEEDED;
}


////////////////////////////////////////////////////////////////////////////////

int guiInvoke(GUI_OPCODE code, void *ptr) {
    int rc;
    switch (code) {
    /*
     * Initialize the GUI framework
     * @return status code.
     */
    case GUI_OPCODE_INIT:
#if OS(WIN32)
        rc = beginSIM();
        UPDATE_RC(rc);

        rc = initGUI();
        UPDATE_RC(rc);
#endif
        break;

    /*
     * Destroy the GUI
     * @return status code.
     */
    case GUI_OPCODE_UNINIT:
#if OS(WIN32)
        rc = endSIM();
        AssertRC(rc);
#endif
        break;

    /*
     * Dispatch & process the messages
     * @return status code.
     */
    case GUI_OPCODE_DOMSG:
        rc = doMessages();
        break;

    default:
        return WERR_FAILED;
    }

    return rc;
}


} //namespace openwsp

