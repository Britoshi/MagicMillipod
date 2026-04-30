#pragma once
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN    18
#define NEOPIXEL2_PIN   19
#define NEOPIXEL3_PIN   16
#define NEOPIXEL_COUNT  7
#define NeoPixels NeoPixelController::Instance()

class NeoPixelController {
public:
    static NeoPixelController &Instance()
    {
        static NeoPixelController instance;
        return instance;
    }

    NeoPixelController(const NeoPixelController &) = delete;
    NeoPixelController &operator=(const NeoPixelController &) = delete;

    void Begin()
    {
        _ring1.begin(); _ring1.setBrightness(80);
        _ring2.begin(); _ring2.setBrightness(80);
        _ring3.begin(); _ring3.setBrightness(80);
        delay(200);
        _ring1.clear(); _ring1.show();
        _ring2.clear(); _ring2.show();
        _ring3.clear(); _ring3.show();
        delay(10);
        _ring1.show(); _ring2.show(); _ring3.show();
        delay(200);
        Serial.println("[NeoPixel] Ready");
    }

    // Ring 1 : solid color
    void SetAll(uint8_t r, uint8_t g, uint8_t b)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
            _ring1.setPixelColor(i, _ring1.Color(r, g, b));
        _ring1.show();
        delay(10);
        _ring1.show();
    }

    void Ring1Fade(uint8_t r, uint8_t g, uint8_t b, float t)
    {
        t = constrain(t, 0.0f, 1.0f);
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
            _ring1.setPixelColor(i, _ring1.Color((uint8_t)(r*t), (uint8_t)(g*t), (uint8_t)(b*t)));
        _ring1.show();
    }

    void Off()
    {
        _ring1.clear();
        _ring1.show();
    }

    // Ring 2 : rainbow, call from OnStateUpdate(). brightness 0.0-1.0
    void Ring2Rainbow(float brightness = 1.0f, uint32_t intervalMs = 20)
    {
        uint32_t now = millis();
        if (now - _rainbowTimer >= intervalMs)
        {
            _rainbowTimer = now;
            for (int i = 0; i < NEOPIXEL_COUNT; i++)
            {
                uint32_t c = _hue(_rainbowIndex + (i * 256 / NEOPIXEL_COUNT));
                uint8_t r = ((c >> 16) & 0xFF) * brightness;
                uint8_t g = ((c >> 8)  & 0xFF) * brightness;
                uint8_t b = ((c)       & 0xFF) * brightness;
                _ring2.setPixelColor(i, _ring2.Color(r, g, b));
            }
            _ring2.show();
            _rainbowIndex = (_rainbowIndex + 1) % 256;
        }
    }

    void Ring2Fade(uint8_t r, uint8_t g, uint8_t b, float t)
    {
        t = constrain(t, 0.0f, 1.0f);
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
            _ring2.setPixelColor(i, _ring2.Color((uint8_t)(r*t), (uint8_t)(g*t), (uint8_t)(b*t)));
        _ring2.show();
    }

    void Ring2Off()
    {
        _ring2.clear();
        _ring2.show();
    }

    void Ring3SetAll(uint8_t r, uint8_t g, uint8_t b)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
            _ring3.setPixelColor(i, _ring3.Color(r, g, b));
        _ring3.show();
    }

    void Ring3Fade(uint8_t r, uint8_t g, uint8_t b, float t)
    {
        t = constrain(t, 0.0f, 1.0f);
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
            _ring3.setPixelColor(i, _ring3.Color((uint8_t)(r*t), (uint8_t)(g*t), (uint8_t)(b*t)));
        _ring3.show();
    }

    void Ring3Off()
    {
        _ring3.clear();
        _ring3.show();
    }

private:
    NeoPixelController()
        : _ring1(NEOPIXEL_COUNT, NEOPIXEL_PIN,  NEO_GRB + NEO_KHZ800)
        , _ring2(NEOPIXEL_COUNT, NEOPIXEL2_PIN, NEO_GRB + NEO_KHZ800)
        , _ring3(NEOPIXEL_COUNT, NEOPIXEL3_PIN, NEO_GRB + NEO_KHZ800)
    {}

    Adafruit_NeoPixel _ring1;
    Adafruit_NeoPixel _ring2;
    Adafruit_NeoPixel _ring3;

    uint32_t _rainbowTimer = 0;
    uint8_t  _rainbowIndex = 0;

    uint32_t _hue(uint8_t pos)
    {
        pos = 255 - pos;
        if (pos < 85)  return _ring2.Color(255 - pos * 3, 0, pos * 3);
        if (pos < 170) { pos -= 85; return _ring2.Color(0, pos * 3, 255 - pos * 3); }
        pos -= 170;    return _ring2.Color(pos * 3, 255 - pos * 3, 0);
    }
};
