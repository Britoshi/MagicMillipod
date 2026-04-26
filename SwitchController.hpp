#pragma once
#include <Arduino.h>

#define SWITCH_PIN_0  6
#define SWITCH_PIN_1  7
#define SWITCH_PIN_2  8
#define SWITCH_PIN_3  9

#define Switches SwitchController::Instance()

class SwitchController {
public:
    static SwitchController &Instance()
    {
        static SwitchController instance;
        return instance;
    }

    SwitchController(const SwitchController &) = delete;
    SwitchController &operator=(const SwitchController &) = delete;

    void Begin()
    {
        for (int i = 0; i < 4; i++)
            pinMode(_pins[i], INPUT_PULLUP);
    }

    bool IsOn(uint8_t index) const
    {
        if (index > 3) return false;
        return digitalRead(_pins[index]) == LOW;
    }

private:
    SwitchController() = default;
    uint8_t _pins[4] = { SWITCH_PIN_0, SWITCH_PIN_1, SWITCH_PIN_2, SWITCH_PIN_3 };
};
