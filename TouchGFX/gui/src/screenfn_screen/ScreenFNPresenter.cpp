#include <gui/screenfn_screen/ScreenFNView.hpp>
#include <gui/screenfn_screen/ScreenFNPresenter.hpp>

ScreenFNPresenter::ScreenFNPresenter(ScreenFNView& v)
    : view(v)
{

}

void ScreenFNPresenter::activate()
{

}

void ScreenFNPresenter::deactivate()
{

}

void ScreenFNPresenter::sendKey(uint8_t code)
{
    model->sendKey(code);
}

void ScreenFNPresenter::sendConsumer(uint8_t usage)
{
    model->sendConsumer(usage);
}
