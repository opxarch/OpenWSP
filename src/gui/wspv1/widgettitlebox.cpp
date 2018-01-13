#include <string>

#include "widgettitlebox.h"

extern "C" {
# include "GUI.h"
# include "WM.h"
}

#include "bitmaps/bitmaps.h"

namespace gui {

WidgetTitlebox::WidgetTitlebox(int x, int y, int width, int height) :
    WidgetBase(x, y, width, height),
    m_icon(&bmTitleboxIcon) {
}

WidgetTitlebox::WidgetTitlebox(int x, int y, int width, int height, const char *text)
    : WidgetBase(x, y, width, height),
      m_text(text),
      m_icon(&bmTitleboxIcon) {
}

void WidgetTitlebox::setText(const char *text) {
    m_text = text;
}

void WidgetTitlebox::onPaint() {
    GUI_RECT rClient;
    WM_GetClientRect(&rClient);

    int widgetX0 = getX();
    int widgetY0 = getY();
    int widgetX1 = getWidth(); // X size
    int widgetY1 = getHeight(); // Y size

    widgetX0 += rClient.x0;
    widgetY0 += rClient.y0;
    widgetX1 += widgetX0;
    widgetY1 += widgetY0;

    int iconWidth = 16;

    int tm = GUI_SetTextMode(GUI_TM_TRANS);
    const GUI_FONT *font = GUI_SetFont(&GUI_FontTahoma12);

    WM_SetUserClipArea(&rClient);

    GUI_Clear();
    GUI_SetColor(0x999999);//0xAAAAAA);
    GUI_FillRect(widgetX0, widgetY0, widgetX1, widgetY1);
    GUI_SetColor(0xFFFFFF); //0x000000);
    GUI_GotoXY(widgetX0 + iconWidth + 12, widgetY0 + 5);
    GUI_DispString(m_text.c_str());
    GUI_DrawBitmap(static_cast<GUI_CONST_STORAGE GUI_BITMAP *>(m_icon),widgetX0 + 6, widgetY0 + 3);

    WM_SetUserClipArea(0);

    GUI_SetTextMode(tm);
    if (font)
    GUI_SetFont(font);
}

} // namespace gui
