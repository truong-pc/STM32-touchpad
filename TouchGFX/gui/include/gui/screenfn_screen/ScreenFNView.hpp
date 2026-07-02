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
protected:
};

#endif // SCREENFNVIEW_HPP
