
#include <cstdlib>

extern "C" {
# include "GUI.h"
# include "FRAMEWIN.h"
# include "LISTBOX.h"
# include "SLIDER.h"
}

#include "openwsp/types.h"
#include "openwsp/err.h"
#include "openwsp/assert.h"

#include "resources.h"
#include "volumedlg.h"

#include "bitmaps/bitmaps.h"
#include "logics.h"

static FRAMEWIN_Handle hFrameVolume;

static const GUI_WIDGET_CREATE_INFO _aVolumeDlg[] = {
	{ FRAMEWIN_CreateIndirect, "Volume",          ID_VOLUMEDLG,           110, 80,  200, 150, 0,0 },
	{ TEXT_CreateIndirect,     "Volume Control:", ID_TEXT_VOLUME_TITLE,   5,   5,   80,  20 , 0,0 },
	{ SLIDER_CreateIndirect,   NULL,              ID_SLIDER_VOLUME,       5,   30,  30,  80,  SLIDER_CF_VERTICAL,0 },
	{ SLIDER_CreateIndirect,   NULL,              ID_SLIDER_VOLUMEL,      45,  30,  30,  80,  SLIDER_CF_VERTICAL,0 },
	{ SLIDER_CreateIndirect,   NULL,              ID_SLIDER_VOLUMER,      85,  30,  30,  80, SLIDER_CF_VERTICAL,0 },
	{ TEXT_CreateIndirect,     "Main",            ID_TEXT_VOLUME_TITLE,   10,   110,   80,  20 , 0,0 },
	{ TEXT_CreateIndirect,     "Left",            ID_TEXT_VOLUME_TITLE,   50,   110,   80,  20 , 0,0 },
	{ TEXT_CreateIndirect,     "Right",           ID_TEXT_VOLUME_TITLE,   90,  110,   80,  20 , 0,0 },
	{ BUTTON_CreateIndirect,   "OK",              ID_BUTTON_CLOSE_VOLUME, 130, 110, 50,  15,  0,0 },
};

#include "DIALOG.h"

namespace gui {

/*********************************************************************
*
*       _cbFrameWin
*/
static void _cbFrameWin(WM_MESSAGE* pMsg, void *opaque)
{
    WS_ASSERT(opaque);
	static_cast<VolumeDlg*>(opaque)->handle_cbFrameWin(pMsg);
}

void VolumeDlg::handle_cbFrameWin(WM_MESSAGE* pMsg)
{
    int NCode, Id;
    WM_HWIN hText, hSliderVolMain, hSliderVolL, hSliderVolR;
    WM_HWIN hWin = pMsg->hWin;

    int vol;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            hSliderVolMain = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUME);
            hSliderVolL = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMEL);
            hSliderVolR = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMER);;

            SLIDER_SetRange(hSliderVolMain,0, 100);
            SLIDER_SetRange(hSliderVolL,0, 100);
            SLIDER_SetRange(hSliderVolR,0, 100);
            break;

        case WM_PAINT:
            GUI_Clear();
            //GUI_DrawBitmap(&bmVolumeIcon, 140, 40);
            break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch(NCode) {
                case WM_NOTIFICATION_RELEASED:
                    switch(Id) {
                    case ID_BUTTON_CLOSE_VOLUME:
                        hide();
                        break;
                    case ID_SLIDER_VOLUME:
                        hSliderVolMain = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUME);

                        vol = 100 - SLIDER_GetValue(hSliderVolMain);
                        gui().setVolume(vol, vol);
                        gui().getVolume();
                        break;
                    case ID_SLIDER_VOLUMEL:
                    case ID_SLIDER_VOLUMER:
                        hSliderVolL = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMEL);
                        hSliderVolR = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMER);

                        int volL = 100 - SLIDER_GetValue(hSliderVolL);
                        int volR = 100 - SLIDER_GetValue(hSliderVolR);
                        gui().setVolume(volL, volR);
                        gui().getVolume();
                        break;
                    }
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

int VolumeDlg::create(void)
{
	WM_HWIN hClient;

	/* Create framewindow */
	hFrameVolume = GUI_CreateDialogBox( _aVolumeDlg,GUI_COUNTOF(_aVolumeDlg),
									&_cbFrameWin, this,
									0, 0, 0);
	if (!hFrameVolume)
	    return WERR_FAILED;

	hide();
	FRAMEWIN_SetMoveable(hFrameVolume, 1);

	return WINF_SUCCEEDED;
}

int VolumeDlg::destroy(void)
{
	if(hFrameVolume)
	GUI_EndDialog(hFrameVolume, 0);

	return WINF_SUCCEEDED;
}

void VolumeDlg::show(void)
{
	if(hFrameVolume) {
	    gui().getVolume();
		WM_ShowWindow(hFrameVolume);

		WM_BringToTop(hFrameVolume);
		//FRAMEWIN_SetActive(hFrameVolume, 1);
	}
}

void VolumeDlg::hide(void)
{
	if(hFrameVolume)
	WM_HideWindow(hFrameVolume);
}

void VolumeDlg::updateVolume(int volL, int volR)
{
	WM_HWIN hSliderVolMain, hSliderVolL, hSliderVolR;

	hSliderVolMain = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUME);
	hSliderVolL = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMEL);
	hSliderVolR = WM_GetDialogItem(hFrameVolume, ID_SLIDER_VOLUMER);

	SLIDER_SetValue(hSliderVolMain, 100-(volL+volR)/2);
	SLIDER_SetValue(hSliderVolL, 100-volL);
	SLIDER_SetValue(hSliderVolR, 100-volR);
}

} // namespace gui
