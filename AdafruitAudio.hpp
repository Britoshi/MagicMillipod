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

    void Begin(uint8_t triggerPin, uint8_t resetPin)
    {
        _triggerPin = triggerPin;
        pinMode(_triggerPin, OUTPUT);
        digitalWrite(_triggerPin, HIGH);

        _resetPin = resetPin;
        pinMode(_resetPin, OUTPUT);
        digitalWrite(_resetPin, LOW);
        delay(10);
        digitalWrite(_resetPin, HIGH);
        delay(1000); // wait for board to finish booting

        Serial.print("[Audio] Trigger pin: "); Serial.println(_triggerPin);
        Serial.print("[Audio] Reset pin: ");   Serial.println(_resetPin);
    }

    void Reset()
    {
        Serial.println("[Audio] Resetting...");
        digitalWrite(_resetPin, LOW);
        delay(10);
        digitalWrite(_resetPin, HIGH);
        delay(1000);  // wait for board to reinitialize
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
    uint8_t _resetPin   = 0;
};

#endif
