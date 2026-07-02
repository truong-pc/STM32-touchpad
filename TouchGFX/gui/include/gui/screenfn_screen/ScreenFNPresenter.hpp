#ifndef SCREENFNPRESENTER_HPP
#define SCREENFNPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenFNView;

class ScreenFNPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenFNPresenter(ScreenFNView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void sendKey(uint8_t code);
    void sendConsumer(uint8_t usage);

    virtual ~ScreenFNPresenter() {}

private:
    ScreenFNPresenter();

    ScreenFNView& view;
};

#endif // SCREENFNPRESENTER_HPP
