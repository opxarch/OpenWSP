#ifndef __RESOURCE_H__
#define __RESOURCE_H__

enum {
    // USER START (Optionally insert additional includes)

// maindlg.cpp
    ID_MAINDLG = GUI_ID_USER + 0x00,
    ID_MENU_SEPARATOR,
    ID_MENU_MAIN,
    ID_MENU_FILE,
    ID_MENU_OPEN_CATALOGS,
    ID_MENU_PLAYLIST,
    ID_MENU_FAVORITES,

    ID_MENU_AUDIO,
    ID_MENU_AUDIO_VOLCTL,

    ID_MENU_NETWORK,

    /** Play pos */
    ID_TEXT_POS,
    /** Playback progress */
    ID_SLIDER_PLAYER,
    /** Play/Pause button */
    ID_BUTTON_PLAY,

//InitDlg.c
    ID_INITDLG,
    ID_TEXT_LOADING,


//catalogdlg.cpp
    ID_CATELOGDLG,

    ID_LISTVIEW_CATELOG,
    ID_TEXT_CATALOG,
    ID_BUTTON_OPEN_SEARCH,
    ID_BUTTON_CANCEL_SEARCH,

//channelsdlg.cpp
    ID_CHANNELSDLG,
    ID_TEXT_CHANNELS,
    ID_LISTBOX_CHANNELS,

//volumedlg.cpp
    ID_VOLUMEDLG,
    ID_TEXT_VOLUME_TITLE,
    ID_SLIDER_VOLUME,
    ID_SLIDER_VOLUMEL,
    ID_SLIDER_VOLUMER,
    ID_BUTTON_CLOSE_VOLUME,

// USER END

};



#endif
