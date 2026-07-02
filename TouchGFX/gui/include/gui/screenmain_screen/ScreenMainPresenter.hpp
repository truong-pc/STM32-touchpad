#ifndef SCREENMAINPRESENTER_HPP
#define SCREENMAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenMainView;

class ScreenMainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenMainPresenter(ScreenMainView& v);

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

    void moveMouse(int16_t dx, int16_t dy);

    virtual ~ScreenMainPresenter() {}

private:
    ScreenMainPresenter();

    ScreenMainView& view;
};

#endif // SCREENMAINPRESENTER_HPP
