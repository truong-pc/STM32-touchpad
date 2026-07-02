#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
extern "C"
{
#include "cmsis_os2.h"
#include "hid_events.h"
extern osMessageQueueId_t hidQueueHandle;
}

static void pushHidEvent(uint8_t type, uint8_t a, int8_t b, int8_t c)
{
    HID_Event_t ev = { type, a, b, c };
    if (hidQueueHandle != 0)
    {
        (void)osMessageQueuePut(hidQueueHandle, &ev, 0U, 0U);
    }
}
#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}

void Model::moveMouse(int16_t dx, int16_t dy)
{
#ifndef SIMULATOR
    if (dx == 0 && dy == 0)
    {
        return;
    }
    if (dx > 127)
    {
        dx = 127;
    }
    else if (dx < -127)
    {
        dx = -127;
    }
    if (dy > 127)
    {
        dy = 127;
    }
    else if (dy < -127)
    {
        dy = -127;
    }
    pushHidEvent(EV_MOUSE, 0, (int8_t)dx, (int8_t)dy);
#else
    (void)dx;
    (void)dy;
#endif
}

void Model::sendKey(uint8_t code)
{
#ifndef SIMULATOR
    pushHidEvent(EV_KEYBOARD, code, 1, 0);
#else
    (void)code;
#endif
}

void Model::sendConsumer(uint8_t usage)
{
#ifndef SIMULATOR
    pushHidEvent(EV_CONSUMER, usage, 1, 0);
#else
    (void)usage;
#endif
}
