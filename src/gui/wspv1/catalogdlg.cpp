#include <cstdlib>
#include <iostream>

extern "C" {
# include "GUI.h"
# include "TEXT.h"
# include "LISTBOX.h"
# include "LISTVIEW.h"
# include "SLIDER.h"
}

#include "openwsp/types.h"
#include "openwsp/err.h"
#include "openwsp/assert.h"
#include "openwsp.h"

#include "logics.h"
#include "resources.h"
#include "catalogdlg.h"

using openwsp::app;
using openwsp::THREAD_MAIN;

namespace gui {

/*******************************************************************************
*   GUI Layouts                                                                *
*******************************************************************************/

static const GUI_WIDGET_CREATE_INFO _aCategoriesDlg[] = {
    { FRAMEWIN_CreateIndirect, "Channel categories",  ID_CATELOGDLG,           30,  30,  280,  200,  0,0 },
    { TEXT_CreateIndirect,     "Select a catalog from the following list:",
                                                ID_TEXT_CATALOG,         5,   5,   202,  20,   0,50 },
    { LISTVIEW_CreateIndirect, NULL,            ID_LISTVIEW_CATELOG,     5,   29,  262,  122,  0,0 },
    { BUTTON_CreateIndirect,   "Open",          ID_BUTTON_OPEN_SEARCH,   217, 160, 50,   20,   0,0 },
    { BUTTON_CreateIndirect,   "Cancel",        ID_BUTTON_CANCEL_SEARCH, 160, 160, 50,   20,   0,0 },
};

////////////////////////////////////////////////////////////////////////////////


#include "DIALOG.h"


CatalogDlg::CatalogDlg() :
    hFrameMain(0),
    hListViewCatalog(0),
    hSliderV(0),
    hSliderH(0),
    m_listsel(0),
    event(this)
{
}

////////////////////////////////////////////////////////////////////////////////

void CatalogDlg::onListCatalogSelChanged() {
    m_listsel = LISTVIEW_GetSel(hListViewCatalog);
}

void CatalogDlg::onButtonOpen() {
    int num = LISTVIEW_GetNumRows(hListViewCatalog);
    if (num && m_listsel < num) {
        gui().openChannelsDlg(mapIndex[m_listsel]);
        //minimize();
    }
}

void CatalogDlg::onClose() {
    destroy();
    delete this;
}


////////////////////////////////////////////////////////////////////////////////


/*********************************************************************
*
*       _cbFrameWin
*/
static void _cbFrameWin(WM_MESSAGE* pMsg, void *opaque)
{
    WS_ASSERT(opaque);
    static_cast<CatalogDlg *>(opaque)->handle_cbFrameWin(pMsg);
}

void CatalogDlg::handle_cbFrameWin(WM_MESSAGE* pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            /* Get handles */
            hListViewCatalog = WM_GetDialogItem(hWin, ID_LISTVIEW_CATELOG);
            hSliderV = WM_GetDialogItem(hWin, GUI_ID_SLIDER0);
            hSliderH = WM_GetDialogItem(hWin, GUI_ID_SLIDER1);

            /*
             * Do initialization here
             */
            LISTVIEW_SetGridVis(hListViewCatalog, 1);

            LISTVIEW_AddColumn(hListViewCatalog, 80, "Name", GUI_TA_CENTER);
            LISTVIEW_AddColumn(hListViewCatalog, 100, "Description", GUI_TA_CENTER);
            LISTVIEW_AddColumn(hListViewCatalog, 100, "Catalog", GUI_TA_CENTER);

            SCROLLBAR_CreateAttached(hListViewCatalog, SCROLLBAR_CF_VERTICAL);
            //SCROLLBAR_CreateAttached(hListViewCatalog, 0);

            LISTVIEW_SetFont(hListViewCatalog, &GUI_FontTahoma12);

            break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch(NCode) {
            case WM_NOTIFICATION_SEL_CHANGED:
                onListCatalogSelChanged();
                break;

            case WM_NOTIFICATION_RELEASED:
                switch(Id) {
                case ID_BUTTON_OPEN_SEARCH:
                    onButtonOpen();
                    break;
                case ID_BUTTON_CANCEL_SEARCH:
                    onClose();

                    /*
                     * now we have destroyed all the member data.
                     */
                    return;
                }
                break;
            }
            break;
    }

    WM_DefaultProc(pMsg);
}

int CatalogDlg::create(void)
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

int CatalogDlg::destroy(void)
{
    if(hFrameMain)
    GUI_EndDialog(hFrameMain, 0);

    return WINF_SUCCEEDED;
}

void CatalogDlg::minimize() {
    FRAMEWIN_Minimize(hFrameMain);
}

void CatalogDlg::restore() {
    FRAMEWIN_Restore(hFrameMain);
}

void CatalogDlg::clearList() {
    /* remove all the rows */
    int num = LISTVIEW_GetNumRows(hListViewCatalog);
    for (int i=0; i < num; i++) {
        LISTVIEW_DeleteRow(hListViewCatalog, i);
    }
    m_listsel = 0;

    /* reset the index mapping */
    mapIndex.clear();
    std::map<int, int> map = mapIndex;
    mapIndex.swap(map);
}

void CatalogDlg::insertListItem(const char *name, const char *desc, const char *catalog, int id)
{
    const char *cst[3] = {name, desc, catalog};
    LISTVIEW_AddRow(hListViewCatalog, cst);

    int row = LISTVIEW_GetNumRows(hListViewCatalog) - 1;
    mapIndex[row] = id;
}


} // namespace gui
