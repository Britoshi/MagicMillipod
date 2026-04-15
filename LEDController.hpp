#pragma once
#include <Arduino.h>
#define LED LEDController::Instance()

enum Color { RED, BLUE, YELLOW };

class LEDController {
public:
    static LEDController &Instance()
    {
        static LEDController instance;
        return instance;
    }

    LEDController(const LEDController &) = delete;
    LEDController &operator=(const LEDController &) = delete;

    void Begin()
    {
        for (int i = 0; i < 3; i++)
        {
            pinMode(_redPins[i],    OUTPUT); digitalWrite(_redPins[i],    LOW);
            pinMode(_bluePins[i],   OUTPUT); digitalWrite(_bluePins[i],   LOW);
            pinMode(_yellowPins[i], OUTPUT); digitalWrite(_yellowPins[i], LOW);
        }
    }

    void Toggle(Color color, bool on)
    {
        uint8_t *pins = _pinsFor(color);
        for (int i = 0; i < 3; i++)
            digitalWrite(pins[i], on ? HIGH : LOW);
    }

private:
    LEDController() = default;

    uint8_t _redPins[3]    = { 6,  7,  8  };
    uint8_t _bluePins[3]   = { 9,  10, 11 };
    uint8_t _yellowPins[3] = { 14, 15, 16 };

    uint8_t *_pinsFor(Color color)
    {
        switch (color)
        {
            case RED:    return _redPins;
            case BLUE:   return _bluePins;
            case YELLOW: return _yellowPins;
        }
        return _redPins;
    }
};
