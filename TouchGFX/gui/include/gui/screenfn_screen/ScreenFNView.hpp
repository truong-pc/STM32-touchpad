#ifndef SCREENFNVIEW_HPP
#define SCREENFNVIEW_HPP

#include <gui_generated/screenfn_screen/ScreenFNViewBase.hpp>
#include <gui/screenfn_screen/ScreenFNPresenter.hpp>

class ScreenFNView : public ScreenFNViewBase
{
public:
    ScreenFNView();
    virtual ~ScreenFNView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void arrowUpClicked();
    virtual void arrowDownClicked();
    virtual void arrowLeftClicked();
    virtual void arrowRightClicked();
    virtual void volUpClicked();
    virtual void volDownClicked();
    virtual void playPauseClicked();
protected:
};

#endif // SCREENFNVIEW_HPP
