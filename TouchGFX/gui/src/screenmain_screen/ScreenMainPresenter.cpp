#include <gui/screenmain_screen/ScreenMainView.hpp>
#include <gui/screenmain_screen/ScreenMainPresenter.hpp>

ScreenMainPresenter::ScreenMainPresenter(ScreenMainView& v)
    : view(v)
{

}

void ScreenMainPresenter::activate()
{

}

void ScreenMainPresenter::deactivate()
{

}

void ScreenMainPresenter::moveMouse(int16_t dx, int16_t dy)
{
    model->moveMouse(dx, dy);
}
