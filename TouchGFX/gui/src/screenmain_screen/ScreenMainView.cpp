#include <gui/screenmain_screen/ScreenMainView.hpp>

ScreenMainView::ScreenMainView()
{

}

void ScreenMainView::setupScreen()
{
    ScreenMainViewBase::setupScreen();
}

void ScreenMainView::tearDownScreen()
{
    ScreenMainViewBase::tearDownScreen();
}

void ScreenMainView::handleDragEvent(const touchgfx::DragEvent& evt)
{
    ScreenMainViewBase::handleDragEvent(evt);
    presenter->moveMouse(evt.getDeltaX(), evt.getDeltaY());
}
