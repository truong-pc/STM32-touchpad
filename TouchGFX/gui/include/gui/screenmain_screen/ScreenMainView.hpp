#ifndef SCREENMAINVIEW_HPP
#define SCREENMAINVIEW_HPP

#include <gui_generated/screenmain_screen/ScreenMainViewBase.hpp>
#include <gui/screenmain_screen/ScreenMainPresenter.hpp>
#include <touchgfx/events/DragEvent.hpp>

class ScreenMainView : public ScreenMainViewBase
{
public:
    ScreenMainView();
    virtual ~ScreenMainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
protected:
};

#endif // SCREENMAINVIEW_HPP
