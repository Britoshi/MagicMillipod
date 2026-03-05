#ifndef ADAFRUIT_AUDIO
#define ADAFRUIT_AUDIO
#include <stdint.h>

class AdafruitAudio {
public:
    AdafruitAudio();
    ~AdafruitAudio();

    // Initialization
    bool Begin(uint8_t resetPin);
    void Reset();

    // Playback control
    void Play(uint8_t trackNumber);
    void Stop();
    void Pause();
    void Resume();

    // Volume control
    void SetVolume(uint8_t volume);
    uint8_t GetVolume() const;

    // Status queries
    bool IsPlaying() const;
    uint8_t GetCurrentTrack() const;

private:
    uint8_t resetPin_;
    uint8_t currentTrack_;
    bool isInitialized_;
};

#endif 