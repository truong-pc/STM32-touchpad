#include <gui/screenfn_screen/ScreenFNView.hpp>

ScreenFNView::ScreenFNView()
{

}

void ScreenFNView::setupScreen()
{
    ScreenFNViewBase::setupScreen();
}

void ScreenFNView::tearDownScreen()
{
    ScreenFNViewBase::tearDownScreen();
}

/* Keyboard page (0x07): Up=0x52, Down=0x51, Left=0x50, Right=0x4F */
void ScreenFNView::arrowUpClicked()
{
    presenter->sendKey(0x52);
}

void ScreenFNView::arrowDownClicked()
{
    presenter->sendKey(0x51);
}

void ScreenFNView::arrowLeftClicked()
{
    presenter->sendKey(0x50);
}

void ScreenFNView::arrowRightClicked()
{
    presenter->sendKey(0x4F);
}

/* Consumer page (0x0C): Vol+=0xE9, Vol-=0xEA, Play/Pause=0xCD */
void ScreenFNView::volUpClicked()
{
    presenter->sendConsumer(0xE9);
}

void ScreenFNView::volDownClicked()
{
    presenter->sendConsumer(0xEA);
}

void ScreenFNView::playPauseClicked()
{
    presenter->sendConsumer(0xCD);
}
