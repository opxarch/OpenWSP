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
#include <openwsp/webservice.h>
#include <openwsp/list.h>
#include <openwsp/string.h>
#include <openwsp/timer.h>
#include <openwsp/autoptr.h>

extern "C" {
# include "LCDConf.h"
# include "GUI.h"
# include "FRAMEWIN.h"
}

#include "openwsp.h"

#include "gui/gui.h"

#include "maindlg.h"
#include "volumedlg.h"
#include "catalogdlg.h"
#include "channelsdlg.h"

#include "logics.h"

/*******************************************************************************
*   Imported Functions                                                               *
*******************************************************************************/
#if OS(WIN32)
extern "C" int beginSIM();
extern "C" int endSIM();
extern int simEnded;
#endif

/*******************************************************************************
*   Apply named spaces                                                           *
*******************************************************************************/
using openwsp::app;
using openwsp::THREAD_IO;
using openwsp::THREAD_MAIN;
using openwsp::THREAD_AUDIO;
using openwsp::Webservice;
using openwsp::catalog;
using openwsp::channel;
using openwsp::channelURL;
using openwsp::WSList;
using openwsp::encodeString;
using openwsp::memcpyLimited;
using openwsp::AutoPtr;
using openwsp::Locator;


namespace gui {

/*******************************************************************************
*   Global variables                                                           *
*******************************************************************************/
GuiMain *g_guimain = 0;

/** Main dialog */
MainDlg *g_maindlg = 0;

/** Volume controlling dialog */
VolumeDlg *g_volumedlg = 0;


////////////////////////////////////////////////////////////////////////////////

/**
 * Message callback of background
 */
static void handle_cbBkWindow(WM_MESSAGE* pMsg, void *opaque) {
    static GUI_MEMDEV_Handle _hBkMemDev;

    UNUSED(opaque);

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
              GUI_SetBkColor(GUI_BLUE);//0xA8A857);//
              GUI_Clear();
              GUI_SetFont(&GUI_FontTahoma12);
              //GUI_DispString("ËÎÌåabcd");
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
static int invoke_initGUI(void) {
    int r, rc;

    g_guimain = new (std::nothrow) GuiMain;
    if (!g_guimain) {
        return WERR_ALLOC_MEMORY;
    }

    r = GUI_Init();

    if (r)
        return WERR_FAILED;

    /* Use memory devices */
    WM_SetCreateFlags(WM_CF_MEMDEV);
    WM_EnableMemdev(WM_HBKWIN);

    /* Set callback for background */
    WM_SetCallback(WM_HBKWIN, &handle_cbBkWindow, 0);

    //GUI_CURSOR_Select(&GUI_CursorBusyM);
    GUI_CURSOR_Show();
    //GUI_MessageBox("OpenWSP smart Radio v0.1","Version",0);

    /*
     * Create all the dialogs
     */
    g_maindlg = new (std::nothrow) MainDlg;
    g_volumedlg = new (std::nothrow) VolumeDlg;

    if (!g_maindlg || !g_volumedlg) {
        return WERR_ALLOC_MEMORY;
    }

    rc = g_maindlg->create();
    UPDATE_RC(rc);

    rc = g_volumedlg->create();

    return rc;
}

/*
 * Inner, start up the GUI first.
 * @return status code.
 */
static int invoke_startUp() {
    /*
     * Connect to the servers
     */
    g_guimain->connectService();
    return WINF_SUCCEEDED;
}

/**
 * Inner, dispatch & process the messages.
 * @return status code.
 */
static int invoke_doMessages() {
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
    case GUI_OPCODE_INIT: {
#if OS(WIN32)
        rc = beginSIM();
        UPDATE_RC(rc);
#endif
        rc = invoke_initGUI();
        UPDATE_RC(rc);
    }
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
     * Start up the GUI first.
     * @return status code.
     */
    case GUI_OPCODE_STARTUP:
        rc = invoke_startUp();
        break;

    /*
     * Dispatch & process the messages
     * @return status code.
     */
    case GUI_OPCODE_DOMSG:
        rc = invoke_doMessages();
        break;

    default:
        return WERR_FAILED;
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////

GuiMain::GuiMain() :
    event(this) {
}
GuiMain::~GuiMain() {
}

////////////////////////////////////////////////////////////////////////////////

void GuiMain::connectService() {
    app().threadLoop().PostEvent(
            THREAD_IO,
            event.bind(&GuiMain::eventConnectService));
}

// THREAD_IO

int GuiMain::eventConnectService(void *opaque) {
    int rc = app().threadIO().onConnectService();

    app().threadLoop().PostEvent(
            THREAD_MAIN,
            event.bind(&GuiMain::respConnectService, rc));
    return 1;
}

// THREAD_MAIN

int GuiMain::respConnectService(void *opaque, int rc) {
    processRc(rc);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

void GuiMain::openCatelogDlg() {
    app().threadLoop().PostEvent(
            THREAD_IO,
            event.bind(&GuiMain::eventOpenCatelogDlg));
}

// THREAD_IO

int GuiMain::eventOpenCatelogDlg(void *opaque) {
    int rc;

    WSList<catalog *> *catalogs = new (std::nothrow) WSList<catalog *>;
    if (!catalogs) {
        rc = WERR_ALLOC_MEMORY;
    } else {
        rc = app().threadIO().onGetCatalogs(catalogs);

        /*
         * Response it to the main thread
         */
        if (WS_SUCCESS(rc)) {
            app().threadLoop().PostEvent(
                    THREAD_MAIN,
                    event.bind(&GuiMain::respOpenCatelogDlg, catalogs)
                    );
        }
    }

    gui().processRc(rc);
    return 1;
}

// THREAD_MAIN

int GuiMain::respOpenCatelogDlg(void *opaque, void *list) {

    WSList<catalog *> *catalogs = static_cast<WSList<catalog *> *>(list);
    WS_ASSERT(catalogs);

    int rc;
    std::size_t srclen;
    char src[255];
    char s_name[255], s_desc[255], s_catalog[255];

    CatalogDlg *dlg = new (std::nothrow) CatalogDlg;

 if (!dlg)
  {
    rc = WERR_ALLOC_MEMORY;
  }
 else
  {
    rc = dlg->create();

    if (WS_SUCCESS(rc)) {

        dlg->clearList();

        /*
         * Resolve all the catalogs and add then to the list
         * to display.
         */
        catalog *c;
        for (catalogs->begin(); catalogs->end(); (*catalogs)++)
        {
            rc = catalogs->get(c);
            if (WS_SUCCESS(rc)) {

                memcpyLimited(src, c->m_name.c_str(), c->m_name.length()+1, sizeof(src));
                rc = encodeString(src, "utf-8", "gb2312", s_name, sizeof(s_name));

                if (WS_FAILURE(rc)) break;

                memcpyLimited(src, c->m_desc.c_str(), c->m_desc.length()+1, sizeof(src));
                rc = encodeString(src, "utf-8", "gb2312", s_desc, sizeof(s_desc));

                if (WS_FAILURE(rc)) break;

                memcpyLimited(src, c->m_catalog.c_str(), c->m_catalog.length()+1, sizeof(src));
                rc = encodeString(src, "utf-8", "gb2312", s_catalog, sizeof(s_catalog));

                if (WS_FAILURE(rc)) break;

                dlg->insertListItem(s_name, s_desc, s_catalog, c->m_id);
            }
        }

    }

  }

    catalogs->RemoveAllPtr();

    delete catalogs;

    gui().processRc(rc);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////


void GuiMain::openChannelsDlg(int id) {
    app().threadLoop().PostEvent(
            THREAD_IO,
            event.bind(&GuiMain::eventOpenChannelsDlg, id));
}

// THREAD_IO

int GuiMain::eventOpenChannelsDlg(void *opaque, int id) {
    int rc;

    WSList<channel *> *channels = new (std::nothrow) WSList<channel *>;
    if (!channels) {
        rc = WERR_ALLOC_MEMORY;
    } else {
        rc = app().threadIO().onGetChannels(id, channels);

        /*
         * Response it to the main thread
         */
        if (WS_SUCCESS(rc)) {
            app().threadLoop().PostEvent(
                    THREAD_MAIN,
                    event.bind(&GuiMain::respOpenChannelsDlg, channels)
                    );
        }
    }

    gui().processRc(rc);
    return 1;
}

// THREAD_MAIN

int GuiMain::respOpenChannelsDlg(void *opaque, void *list) {
    WSList<channel *> *channels = static_cast<WSList<channel *> *>(list);
    WS_ASSERT(channels);

    int rc;

    ChannelsDlg *dlg = new (std::nothrow) ChannelsDlg;

    if (!dlg)
      {
        rc = WERR_ALLOC_MEMORY;
      }
    else
      {
        rc = dlg->create();

        if (WS_SUCCESS(rc)) {

            dlg->clearList();

            /*
             * Resolve all the channels and add then to the list
             * to display.
             */
            channel *ch;
            for (channels->begin(); channels->end(); (*channels)++)
            {
                rc = channels->get(ch);
                if (WS_SUCCESS(rc)) {
                    dlg->insertListItem(ch);
                }
            }
        }
      }

    channels->RemoveAll();

    delete channels;

    gui().processRc(rc);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

void GuiMain::loadChannel(int id, const std::string &title) {
    app().threadLoop().PostEvent(
            THREAD_IO,
            event.bind(&GuiMain::eventLoadChannel, id));

    mainDlg()->updateTitle(title);
}


// THREAD_IO

int GuiMain::eventLoadChannel(void *opaque, int id) {
    int rc;

    channel *ch = 0;
    channelURL *url = 0;
    /*
     * query the web address of station and try to load it
     */
    rc = app().threadIO().onGetChannelURL(id, &url);
    if (WS_SUCCESS(rc)) {
        app().threadLoop().PostEvent(
                THREAD_MAIN,
                event.bind(&GuiMain::respLoadChannel, url));
    }
    gui().processRc(rc);
    return 1;
}


// THREAD_MAIN

int GuiMain::respLoadChannel(void *opaque, void *p) {
    channelURL *url = static_cast<channelURL *>(p);
    WS_ASSERT(url);

    int rc = app().threadMain().onLoadMedia(url->m_url);
    delete url;

    gui().processRc(rc);
    return 1;
}


////////////////////////////////////////////////////////////////////////////////


void GuiMain::setVolume(int left, int right) {
    app().threadLoop().PostEvent(
                THREAD_AUDIO,
                event.bind(&GuiMain::eventSetVoulme, left, right));
}

// THREAD_AUDIO

int GuiMain::eventSetVoulme(void *opaque, int left, int right) {
    app().threadAudio().onSetVolume(left, right);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

void GuiMain::getVolume() {
    app().threadLoop().PostEvent(
            THREAD_AUDIO,
            event.bind(&GuiMain::eventGetVoulme));
}

// THREAD_AUDIO

int GuiMain::eventGetVoulme(void *opaque) {
    int rc;
    int left = 0, right = 0;

    rc = app().threadAudio().onGetVolume(&left, &right);

    /*
     * Response it
     */
    if (WS_SUCCESS(rc)) {
        app().threadLoop().PostEvent(
                THREAD_AUDIO,
                event.bind(&GuiMain::respGetVoulme, left, right));
    }
    return 1;
}

// THREAD_MAIN

int GuiMain::respGetVoulme(void *opaque, int left, int right) {
    volumeDlg()->updateVolume(left, right);
    return 1;
}


////////////////////////////////////////////////////////////////////////////////

void GuiMain::processRc(int rc) {
    if (WS_FAILURE(rc)) {
        const ErrorInfo *err = GetErrorMsg(rc);

        app().threadLoop().PostEvent(
            THREAD_MAIN,
            event.bind(&GuiMain::eventProcessRc, err)
            );
    }
}

// THREAD_MAIN

int GuiMain::eventProcessRc(void *opaque, const void *p) {
    const ErrorInfo *err = static_cast<const ErrorInfo *>(p);

    char buff[255];
    std::string msg("Operation has been failed!\n");

    /*
     * format the message text
     */
    if (err->code != WERR_FAILED)
      {
        snprintf(buff, sizeof(buff), "  - error: %s\n", err->msgFull);
        msg += buff;
      }
    snprintf(buff, sizeof(buff), "  - rc = (%s).\n", err->msgDefine);
    msg += buff;

    /*
     * Display it
     */
    GUI_MessageBox(msg.c_str(),"Error", GUI_MESSAGEBOX_CF_MOVEABLE);

    return 1;
}

} //namespace openwsp

