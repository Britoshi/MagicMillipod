#pragma once
#include <Arduino.h>

#define UV_LIGHT_PIN 17
#define UVLight UVLightController::Instance()

class UVLightController {
public:
    static UVLightController &Instance()
    {
        static UVLightController instance;
        return instance;
    }

    UVLightController(const UVLightController &) = delete;
    UVLightController &operator=(const UVLightController &) = delete;

    void Begin()
    {
        pinMode(UV_LIGHT_PIN, OUTPUT);
        analogWriteFreq(1000);
        analogWriteRange(255);
        SetBrightness(0.0f);
    }

    // brightness: 0.0 = off, 1.0 = full power
    void SetBrightness(float brightness)
    {
        brightness = constrain(brightness, 0.0f, 1.0f);
        // N-channel MOSFET: HIGH = on, LOW = off
        int duty = (int)(brightness * 255);
        Serial.print("[UV] Brightness: ");
        Serial.print(brightness);
        Serial.print(" -> duty: ");
        Serial.println(duty);
        analogWrite(UV_LIGHT_PIN, duty);
    }

private:
    UVLightController() = default;
};
