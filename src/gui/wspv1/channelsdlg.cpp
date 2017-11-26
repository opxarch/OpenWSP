#include <cstdlib>
#include <iostream>

extern "C" {
# include "GUI.h"
# include "TEXT.h"
# include "LISTBOX.h"
# include "LISTVIEW.h"
# include "SLIDER.h"
}

#include "bitmaps/bitmaps.h"

#include <openwsp/types.h>
#include <openwsp/err.h>
#include <openwsp/assert.h>
#include <openwsp/webservice.h>
#include <openwsp/string.h>
#include <openwsp/list.h>

#include "openwsp.h"

#include "logics.h"
#include "resources.h"
#include "channelsdlg.h"

using openwsp::app;
using openwsp::THREAD_MAIN;
using openwsp::channel;
using openwsp::memcpyLimited;
using openwsp::encodeString;
using openwsp::WSList;

namespace gui {

/*******************************************************************************
*   GUI Layouts                                                                *
*******************************************************************************/

static const GUI_WIDGET_CREATE_INFO _aCategoriesDlg[] = {
    { FRAMEWIN_CreateIndirect, "Channel list",  ID_CHANNELSDLG,           30,  30,  280,  174,  0,0 },
    { TEXT_CreateIndirect,     "Select a channel from the following list:",
                                                ID_TEXT_CHANNELS,         5,   5,   202,  20,   0,50 },
    { LISTBOX_CreateIndirect,  0,               ID_LISTBOX_CHANNELS,     5,   29,  262,  122,  0,0 }
};

/*******************************************************************************
*   GUI Resources                                                              *
*******************************************************************************/

static const GUI_BITMAP* res_bmListitemUnselected = &bmListitemUnselected;
static const GUI_BITMAP* res_bmListitemSelected = &bmListitemSelected;

////////////////////////////////////////////////////////////////////////////////


#include "DIALOG.h"


ChannelsDlg::ChannelsDlg() :
    hFrameMain(0),
    hListBoxChannels(0),
    hSliderV(0),
    hSliderH(0),
    m_listClickTime(0),
    m_listClickIndex(0),
    m_listsel(0),
    event(this)
{
}

////////////////////////////////////////////////////////////////////////////////

void ChannelsDlg::onButtonOpen() {

}

void ChannelsDlg::onClose() {
    destroy();
    delete this;
}

void ChannelsDlg::onListChannelsSelChanged() {
    m_listsel = LISTBOX_GetSel(hListBoxChannels);
}

void ChannelsDlg::onListChannelsDbClick() {
    int num = LISTBOX_GetNumItems(hListBoxChannels);
    if (num && m_listsel < num) {
        channel *ch = mapIndex[m_listsel];
        const channel::mediainfo *media = 0;
        for (ch->m_mediainfo.begin(); ch->m_mediainfo.end(); ch->m_mediainfo++) {
            if (WS_SUCCESS(ch->m_mediainfo.get(media))) {
                break;
            }
        }
        if (media) {
            char title[255];
            LISTBOX_GetItemText(hListBoxChannels, m_listsel, title, sizeof(title));
            gui().loadChannel(media->m_mid, std::string(title));
        } else {
            gui().processRc(WERR_FAILED);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////


/*********************************************************************
*
*       _cbFrameWin
*/
static void _cbFrameWin(WM_MESSAGE* pMsg, void *opaque)
{
    WS_ASSERT(opaque);
    static_cast<ChannelsDlg *>(opaque)->handle_cbFrameWin(pMsg);
}

/*********************************************************************
*
*       _OwnerDraw
*
* Purpose:
*   This is the owner draw function.
*   It allows complete customization of how the items in the listbox are
*   drawn. A command specifies what the function should do;
*   The minimum is to react to the draw command (WIDGET_ITEM_DRAW);
*   If the item x-size differs from the default, then this information
*   needs to be returned in reaction to WIDGET_ITEM_GET_XSIZE.
*   To insure compatibility with future version, all unhandled commands
*   must call the default routine LISTBOX_OwnerDraw.
*/
static int _cbOwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    WS_ASSERT(pDrawItemInfo->opaque);
    return static_cast<ChannelsDlg *>(pDrawItemInfo->opaque)->handle_cbOwnerDraw(pDrawItemInfo);
}

////////////////////////////////////////////////////////////////////////////////


void ChannelsDlg::handle_cbFrameWin(WM_MESSAGE* pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            /* Get handles */
            hListBoxChannels = WM_GetDialogItem(hWin, ID_LISTBOX_CHANNELS);
            hSliderV = WM_GetDialogItem(hWin, GUI_ID_SLIDER0);
            hSliderH = WM_GetDialogItem(hWin, GUI_ID_SLIDER1);

            /*
             * Do initialization here
             */
            LISTBOX_SetFont(hListBoxChannels, &GUI_FontTahoma12);
            LISTBOX_SetScrollStepH(hListBoxChannels, 6);
            LISTBOX_SetAutoScrollH(hListBoxChannels, 1);
            LISTBOX_SetAutoScrollV(hListBoxChannels, 1);
            LISTBOX_SetOwnerDraw(hListBoxChannels, _cbOwnerDraw, this);
            break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch(NCode) {
                case WM_NOTIFICATION_SEL_CHANGED:
                  LISTBOX_InvalidateItem(hListBoxChannels, LISTBOX_ALL_ITEMS);
                  onListChannelsSelChanged();
                  break;

                case WM_NOTIFICATION_RELEASED:
                switch(Id) {
                case ID_LISTBOX_CHANNELS:
                    /*
                     * whether it is a double clicking ?
                     */
                    if ((m_listClickTime)
                          && (GUI_GetTime() - m_listClickTime < GUI_DOUBLE_CLICK_INTERVAL)
                          && (m_listClickIndex == LISTBOX_GetSel(hListBoxChannels))) {
                        onListChannelsDbClick();
                    }

                    m_listClickTime = GUI_GetTime();
                    m_listClickIndex = LISTBOX_GetSel(hListBoxChannels);
                    break;
                }
                break;
            }
            break;
    }

    WM_DefaultProc(pMsg);
}

////////////////////////////////////////////////////////////////////////////////

// owner-draw ListBox

/*********************************************************************
*
*       _GetItemSizeX
*/
static int _GetItemSizeX(WM_HWIN hWin, int ItemIndex) {
  char acBuffer[100];
  int  DistX;
  LISTBOX_GetItemText(hWin, ItemIndex, acBuffer, sizeof(acBuffer));
  DistX = GUI_GetStringDistX(acBuffer);
  return DistX + res_bmListitemUnselected->XSize + 16;
}

/*********************************************************************
*
*       _GetItemSizeY
*/
static int _GetItemSizeY(WM_HWIN hWin, int ItemIndex) {
  int DistY;
  DistY = GUI_GetFontDistY() + 1;
  if (LISTBOX_GetMulti(hWin)) {
    if (LISTBOX_GetItemSel(hWin, ItemIndex)) {
      DistY += 8;
    }
  } else if (LISTBOX_GetSel(hWin) == ItemIndex) {
    DistY += 8;
  }
  return DistY;
}

int ChannelsDlg::handle_cbOwnerDraw(const void * p) {
    WM_HWIN hWin;
    int Index;
    const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo = static_cast<const WIDGET_ITEM_DRAW_INFO*>(p);
    hWin     = pDrawItemInfo->hWin;
    Index    = pDrawItemInfo->ItemIndex;
    switch (pDrawItemInfo->Cmd) {
    case WIDGET_ITEM_GET_XSIZE:
        return _GetItemSizeX(hWin, Index);
    case WIDGET_ITEM_GET_YSIZE:
        return _GetItemSizeY(hWin, Index);
    case WIDGET_ITEM_DRAW:
    {
      int MultiSel, Sel, YSize, FontDistY;
      int IsDisabled, IsSelected;
      int ColorIndex = 0;
      char acBuffer[100];
      const GUI_BITMAP * pBm;
      const GUI_FONT* pOldFont = 0;
      GUI_COLOR aColor[4] = {GUI_BLACK, GUI_WHITE, GUI_WHITE, GUI_GRAY};
      GUI_COLOR aBkColor[4] = {GUI_WHITE, GUI_GRAY, GUI_BLUE, 0xC0C0C0};
      IsDisabled = LISTBOX_GetItemDisabled(pDrawItemInfo->hWin, pDrawItemInfo->ItemIndex);
      IsSelected = LISTBOX_GetItemSel(hWin, Index);
      MultiSel   = LISTBOX_GetMulti(hWin);
      Sel        = LISTBOX_GetSel(hWin);
      YSize      = _GetItemSizeY(hWin, Index);
      /* Calculate color index */
      if (MultiSel) {
        if (IsDisabled) {
          ColorIndex = 3;
        } else {
          ColorIndex = (IsSelected) ? 2 : 0;
        }
      } else {
        if (IsDisabled) {
          ColorIndex = 3;
        } else {
          if (pDrawItemInfo->ItemIndex == Sel) {
            ColorIndex = WM_HasFocus(pDrawItemInfo->hWin) ? 2 : 1;
          } else {
            ColorIndex = 0;
          }
        }
      }
      /* Draw item */
      GUI_SetBkColor(aBkColor[ColorIndex]);
      GUI_SetColor  (aColor[ColorIndex]);
      LISTBOX_GetItemText(pDrawItemInfo->hWin, pDrawItemInfo->ItemIndex, acBuffer, sizeof(acBuffer));
      GUI_Clear();
#if 1
      if ((ColorIndex == 1) || (ColorIndex == 2)) {
        pOldFont = GUI_SetFont(&GUI_FontTahoma12B);
      }
#endif
      FontDistY  = GUI_GetFontDistY();
      GUI_DispStringAt(acBuffer, pDrawItemInfo->x0 + res_bmListitemUnselected->XSize + 16, pDrawItemInfo->y0 + (YSize - FontDistY) / 2);
      if (pOldFont) {
          GUI_SetFont(pOldFont);
      }
      GUI_DispCEOL();

      /* Draw bitmap */
      pBm = MultiSel ?
              IsSelected ? res_bmListitemSelected : res_bmListitemUnselected
                      :
              (pDrawItemInfo->ItemIndex == Sel) ? res_bmListitemSelected : res_bmListitemUnselected;

      GUI_DrawBitmap(pBm, pDrawItemInfo->x0 + 7, pDrawItemInfo->y0 + (YSize - pBm->YSize) / 2);
      /* Draw focus rectangle */
      if (MultiSel && (pDrawItemInfo->ItemIndex == Sel)) {
        GUI_RECT rFocus;
        GUI_RECT rInside;
        WM_GetInsideRectEx(pDrawItemInfo->hWin, &rInside);
        rFocus.x0 = pDrawItemInfo->x0;
        rFocus.y0 = pDrawItemInfo->y0;
        rFocus.x1 = rInside.x1;
        rFocus.y1 = pDrawItemInfo->y0 + YSize - 1;
        GUI_SetColor(GUI_WHITE - aBkColor[ColorIndex]);
        GUI_DrawFocusRect(&rFocus, 0);
      }
    }
    break;
    default:
    return LISTBOX_OwnerDraw(pDrawItemInfo);
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////


int ChannelsDlg::create(void)
{
    /* Create frame window */
    hFrameMain = GUI_CreateDialogBox( _aCategoriesDlg,GUI_COUNTOF(_aCategoriesDlg),
                                    &_cbFrameWin, this,
                                    0, 0, 0);

    if (!hFrameMain) {
        return WERR_FAILED;
    }

    FRAMEWIN_AddCloseButton(hFrameMain, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMinButton  (hFrameMain, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_SetActive(hFrameMain, 1);
    FRAMEWIN_SetMoveable(hFrameMain, 1);

    return WINF_SUCCEEDED;
}

int ChannelsDlg::destroy(void)
{
    if(hFrameMain)
    GUI_EndDialog(hFrameMain, 0);

    clearList();
    return WINF_SUCCEEDED;
}

void ChannelsDlg::minimize() {
    FRAMEWIN_Minimize(hFrameMain);
}

void ChannelsDlg::restore() {
    FRAMEWIN_Restore(hFrameMain);
}

void ChannelsDlg::clearList() {
    /* remove all the rows */
    int num = LISTBOX_GetNumItems(hListBoxChannels);
    for (int i=0; i < num; i++) {
        LISTBOX_DeleteItem(hListBoxChannels, i);
    }
    m_listsel = 0;

    /*
     * reset the index mapping and release memory
     */
    std::map<int, channel *>::iterator iter;
    for (iter = mapIndex.begin(); iter != mapIndex.end(); ++iter) {
        delete iter->second;
    }

    mapIndex.clear();
    std::map<int, channel *> map = mapIndex;
    mapIndex.swap(map);
}

void ChannelsDlg::insertListItem(channel *ch)
{
    std::size_t srclen;
    char src[255];
    char s_name[255], s_desc[255];

    memcpyLimited(src, ch->m_name.c_str(), ch->m_name.length()+1, sizeof(src));
    encodeString(src, "utf-8", "gb2312", s_name, sizeof(s_name));

    memcpyLimited(src, ch->m_desc.c_str(), ch->m_desc.length()+1, sizeof(src));
    encodeString(src, "utf-8", "gb2312", s_desc, sizeof(s_desc));

    LISTBOX_AddString(hListBoxChannels, s_name);

    int row = LISTBOX_GetNumItems(hListBoxChannels) - 1;

    mapIndex[row] = ch;
}


} // namespace gui
