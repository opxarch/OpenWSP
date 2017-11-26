#include <cstdlib>
#include <iostream>

extern "C" {
# include "GUI.h"
# include "FRAMEWIN.h"
# include "MENU.h"
# include "LISTBOX.h"
# include "SLIDER.h"
# include "PROGBAR.h"
# include "MULTIPAGE.h"
}

#include "openwsp/types.h"
#include "openwsp/err.h"
#include "openwsp/assert.h"

#include "logics.h"
#include "resources.h"
#include "maindlg.h"

namespace gui {

/*******************************************************************************
*   GUI Layouts                                                                *
*******************************************************************************/

static const GUI_WIDGET_CREATE_INFO _aMainDlg[] = {
	{ FRAMEWIN_CreateIndirect, "OpenWSP Radio", ID_MAINDLG,       5,   5,   280, 100, 0,0 },
	{ MENU_CreateIndirect,     NULL,                 ID_MENU_MAIN,     0,   0,   280, 15,  MENU_CF_HORIZONTAL,0 },
	{ MENU_CreateIndirect,     NULL,                 ID_MENU_FILE,     280, 120, 0,   0,   MENU_CF_VERTICAL,0 },
	{ MENU_CreateIndirect,     NULL,                 ID_MENU_AUDIO,    280, 120, 0,   0,   MENU_CF_VERTICAL,0 },
	{ TEXT_CreateIndirect,     "00:00:00.0",         ID_TEXT_POS,      5,   20,  55,  15,  0,0 },
	//{ SLIDER_CreateIndirect,   NULL,                 ID_SLIDER_PLAYER, 5,   35,  230, 30,  0,0 },
	{ BUTTON_CreateIndirect,   "Play",               ID_BUTTON_PLAY,   240, 35,  30,  30,  0,0 },
	//{ PROGBAR_CreateIndirect,  "NO CHANNEL",         6,                5,   35,  230,  15,  0,0 }
};

static  MENU_ITEM_DATA _aMenuMainItems[] = {
	{"File(&F)", ID_MENU_FILE, 0, 0},
	{"Audio(&A)", ID_MENU_AUDIO, 0, 0},
	{"Network(&N)", ID_MENU_NETWORK, 0, 0},
	{NULL,0,0,0}
};

static  MENU_ITEM_DATA _aMenuFileItems[] = {
	{"Open channel catalogs",   ID_MENU_OPEN_CATALOGS, 0, 0},
	{"-",              ID_MENU_SEPARATOR,   MENU_IF_SEPARATOR, 0},
	{"Open Play List", ID_MENU_PLAYLIST,   0, 0},
	{"Open Favorites", ID_MENU_FAVORITES,   0, 0},
	{NULL,0,0,0}
};

static  MENU_ITEM_DATA _aMenuAudioItems[] = {
	{"Volume control", ID_MENU_AUDIO_VOLCTL, 0, 0},
	{NULL,0,0,0}
};

////////////////////////////////////////////////////////////////////////////////


#include "DIALOG.h"

#define BS_PLAY 0
#define BS_PAUSE 1
static int ButtonPlay_State = BS_PLAY;

MainDlg::MainDlg()
    : hFrameMain(0),
      m_titlebox("NO CHANNEL")
{
}

void MainDlg::onButtonPlay(void)
{
	switch(ButtonPlay_State) {
		case BS_PLAY:
			//MPI_Play();
			break;
		case BS_PAUSE:
			//MPI_Pause();
			break;
	}
}

void MainDlg::onMenuOpenCatalogs(void)
{
    gui().openCatelogDlg();
}

void MainDlg::onMenuVolumeCtl(void)
{
    volumeDlg()->show();
	//ShowVolumeDlg();
}

/*********************************************************************
*
*       _cbFrameWin
*/
static void _cbFrameWin(WM_MESSAGE* pMsg, void *opaque)
{
    WS_ASSERT(opaque);
    static_cast<MainDlg *>(opaque)->handle_cbFrameWin(pMsg);
}

void MainDlg::handle_cbFrameWin(WM_MESSAGE* pMsg)
{
    int NCode, Id, i;
    MENU_MSG_DATA *pData;

    WM_HWIN hSliderPlayer, hMenu, hMenuFile, hMenuAudio;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            /* Get handle */
            hSliderPlayer = WM_GetDialogItem(hWin, ID_SLIDER_PLAYER);
            hMenu = WM_GetDialogItem(hWin, ID_MENU_MAIN);
            hMenuFile = WM_GetDialogItem(hWin, ID_MENU_FILE);
            hMenuAudio = WM_GetDialogItem(hWin, ID_MENU_AUDIO);

            /* Init control */
            SLIDER_SetRange(hSliderPlayer, 0, 100);
            SLIDER_SetWidth(hSliderPlayer, 10);
            SLIDER_SetValue(hSliderPlayer, 0);

            for(i=0;_aMenuFileItems[i].pText; i++) {
                MENU_AddItem(hMenuFile, &_aMenuFileItems[i]);
            }
            for(i=0;_aMenuAudioItems[i].pText; i++) {
                MENU_AddItem(hMenuAudio, &_aMenuAudioItems[i]);
            }

            _aMenuMainItems[0].hSubmenu = hMenuFile;
            _aMenuMainItems[1].hSubmenu = hMenuAudio;
            for(i=0;_aMenuMainItems[i].pText; i++) {
                MENU_AddItem(hMenu, &_aMenuMainItems[i]);
            }

            break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch(NCode) {
                case WM_NOTIFICATION_RELEASED:
                    switch(Id) {
                    case ID_SLIDER_PLAYER:
                        /* Playback progress has changed */
                        hSliderPlayer = WM_GetDialogItem(hWin, ID_SLIDER_PLAYER);

                        //MPI_Seek(SLIDER_GetValue(hSliderPlayer));
                        break;
                    case ID_BUTTON_PLAY:
                        onButtonPlay();
                        break;
                    }
                    break;
            }
            break;

        case WM_MENU:
            pData = (MENU_MSG_DATA*)pMsg->Data.p;
                               
            switch (pData->MsgType) {
            case MENU_ON_INITMENU:
                break;
            case MENU_ON_ITEMSELECT:
                Id = pData->ItemId;
                switch(Id){
                case ID_MENU_OPEN_CATALOGS:
                    onMenuOpenCatalogs();
                    break;
                case ID_MENU_AUDIO_VOLCTL:
                    onMenuVolumeCtl();
                    break;
                }
                break;
            }
            break;

        case WM_PAINT:
            m_titlebox.onPaint();
    }

    WM_DefaultProc(pMsg);
}

int MainDlg::create(void)
{
	/* Create framewindow */
	hFrameMain = GUI_CreateDialogBox( _aMainDlg,GUI_COUNTOF(_aMainDlg),
									&_cbFrameWin, this,
									0, 0, 0); 

	if (!hFrameMain) {
	    return WERR_FAILED;
	}

	FRAMEWIN_SetActive(hFrameMain, 1);
	FRAMEWIN_SetMoveable(hFrameMain, 1);

	return WINF_SUCCEEDED;
}

int MainDlg::destroy(void)
{
	if(hFrameMain)
	GUI_EndDialog(hFrameMain, 0);

	return WINF_SUCCEEDED;
}

#if 0
static void Format_hhmmss(char *buf, float time)
{
	long tenths = 10 * time;
	int f1 = tenths % 10;
	int ss = (tenths /  10) % 60;
	int mm = (tenths / 600) % 60;
	int hh = tenths / 36000;
	if (time <= 0) {
		sprintf(buf,"(Unknown)");
		return;
	}
	if(ss > 0)
		sprintf(buf,"00:00:%02d.%1d", ss, f1);
	if(hh > 0 || ss > 0)
		sprintf(buf,"00:%2d:%02d.%1d", hh, ss, f1);
	if(hh > 0 || mm > 0 || ss > 0)
		sprintf(buf,"%2d:%02d:%02d.%1d", hh, mm, ss, f1);
}
#endif

void MainDlg::updateTitle(const std::string &title)
{
    m_titlebox.setText(title.c_str());
    WM_Invalidate(hFrameMain);
    FRAMEWIN_SetActive(hFrameMain, 1);
}

void MainDlg::refershState(int State, int AudioClock, int AudioClockLength)
{
	char _buff[16];
	WM_HWIN hButtonPlay = WM_GetDialogItem(hFrameMain, ID_BUTTON_PLAY);
	WM_HWIN hTextPos = WM_GetDialogItem(hFrameMain, ID_TEXT_POS);
	WM_HWIN hSliderPlayer = WM_GetDialogItem(hFrameMain, ID_SLIDER_PLAYER);

#if 0
	switch(State) {
		case STATE_STOP:
		case STATE_PAUSE:
			BUTTON_SetText(hButtonPlay, "Play");
			ButtonPlay_State = BS_PLAY;
			break;

		case STATE_PLAY:
			Format_hhmmss(_buff, AudioClock);
			TEXT_SetText(hTextPos, _buff);

			SLIDER_SetRange(hSliderPlayer, 0, AudioClockLength);
			SLIDER_SetValue(hSliderPlayer, AudioClock);

			BUTTON_SetText(hButtonPlay, "Pause");
			ButtonPlay_State = BS_PAUSE;
			break;

	}
#endif
}

} // namespace gui
