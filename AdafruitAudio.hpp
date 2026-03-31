#ifndef ADAFRUIT_AUDIO
#define ADAFRUIT_AUDIO

#include <stdint.h>
#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>

class AdafruitAudio {
public:
    AdafruitAudio();
    ~AdafruitAudio();

    bool Begin(uint8_t rxPin, uint8_t txPin, uint8_t resetPin, int8_t busyPin = -1);
    void Reset();

    void Play(uint8_t trackNumber);
    void Stop();
    void Pause();
    void Resume();

    void    SetVolume(uint8_t volume); // 0–10
    uint8_t GetVolume() const;

    bool    IsPlaying() const;
    uint8_t GetCurrentTrack() const;

private:
    SoftwareSerial*      _ss;
    Adafruit_Soundboard* _sfx;

    int8_t  _busyPin;
    uint8_t _currentTrack;
    uint8_t _volume;
    bool    _isPlaying;
    bool    _isInitialized;
};

#endif