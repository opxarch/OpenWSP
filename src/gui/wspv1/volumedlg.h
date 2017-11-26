#ifndef GUI_VOLUMEDLG_H_
#define GUI_VOLUMEDLG_H_

namespace gui {

class VolumeDlg {
public:
    int create(void);
    int destroy(void);

    void show(void);
    void hide(void);
    void updateVolume(int volL, int volR);

    void handle_cbFrameWin(WM_MESSAGE* pMsg);

private:
    FRAMEWIN_Handle hFrameMain;
};

} // namespace gui

#endif //!defined(GUI_VOLUMEDLG_H_)
