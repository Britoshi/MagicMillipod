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

    void Begin(uint8_t triggerPin0, uint8_t triggerPin1, uint8_t triggerPin2, uint8_t resetPin)
    {
        _triggerPins[0] = triggerPin0;
        _triggerPins[1] = triggerPin1;
        _triggerPins[2] = triggerPin2;
        for (int i = 0; i < 3; i++)
        {
            pinMode(_triggerPins[i], OUTPUT);
            digitalWrite(_triggerPins[i], HIGH);
        }

        _resetPin = resetPin;
        pinMode(_resetPin, OUTPUT);
        digitalWrite(_resetPin, LOW);
        delay(10);
        digitalWrite(_resetPin, HIGH);
        delay(1000);

        Serial.print("[Audio] Trigger pins: ");
        Serial.print(_triggerPins[0]); Serial.print(", "); Serial.println(_triggerPins[1]);
        Serial.print("[Audio] Reset pin: "); Serial.println(_resetPin);
    }

    void PlayTrack(uint8_t index)
    {
        if (index > 2) return;
        Serial.print("[Audio] Playing track "); Serial.println(index);
        digitalWrite(_triggerPins[index], LOW);
        delay(200);
        digitalWrite(_triggerPins[index], HIGH);
    }

    void Reset()
    {
        Serial.println("[Audio] Resetting...");
        digitalWrite(_resetPin, LOW);
        delay(10);
        digitalWrite(_resetPin, HIGH);
        delay(1000);
    }

private:
    AdafruitAudio() = default;
    uint8_t _triggerPins[3] = { 0, 0, 0 };
    uint8_t _resetPin = 0;
};

#endif
