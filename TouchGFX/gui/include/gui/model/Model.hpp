#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
class ModelListener;

class Model
{
public:
    Model();
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void moveMouse(int16_t dx, int16_t dy);
    void sendKey(uint8_t code);
    void sendConsumer(uint8_t usage);

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
