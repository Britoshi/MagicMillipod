#ifndef ADAFRUIT_AUDIO
#define ADAFRUIT_AUDIO

#include <Arduino.h>

class AdafruitAudio {
public:
    static AdafruitAudio &Instance()
    {
        static AdafruitAudio instance;
        return instance;
    }

    AdafruitAudio(const AdafruitAudio &) = delete;
    AdafruitAudio &operator=(const AdafruitAudio &) = delete;

    void Begin(uint8_t triggerPin)
    {
        _triggerPin = triggerPin;
        pinMode(_triggerPin, OUTPUT);
        digitalWrite(_triggerPin, HIGH);
        Serial.print("[Audio] Trigger pin: "); Serial.println(_triggerPin);
    }

    void Play()
    {
        Serial.println("[Audio] Triggering...");
        digitalWrite(_triggerPin, LOW);
        delay(200);
        digitalWrite(_triggerPin, HIGH);
    }

private:
    AdafruitAudio() = default;
    uint8_t _triggerPin = 0;
};

#endif
