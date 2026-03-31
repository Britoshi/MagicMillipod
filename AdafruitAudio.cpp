#include "AdafruitAudio.hpp"
#include <Arduino.h>

static const uint8_t VOLUME_MAX = 10;

AdafruitAudio::AdafruitAudio()
    : _ss(nullptr), _sfx(nullptr),
      _busyPin(-1), _currentTrack(0),
      _volume(5), _isPlaying(false), _isInitialized(false)
{
}

AdafruitAudio::~AdafruitAudio()
{
    delete _sfx;
    delete _ss;
}

bool AdafruitAudio::Begin(uint8_t rxPin, uint8_t txPin, uint8_t resetPin, int8_t busyPin)
{
    _busyPin = busyPin;

    if (busyPin >= 0)
        pinMode(busyPin, INPUT);

    _ss  = new SoftwareSerial(rxPin, txPin);
    _sfx = new Adafruit_Soundboard(_ss, nullptr, resetPin);

    _ss->begin(9600);
    _isInitialized = _sfx->reset();
    return _isInitialized;
}

void AdafruitAudio::Reset()
{
    if (!_isInitialized) return;
    _sfx->reset();
    _currentTrack = 0;
    _isPlaying    = false;
}

void AdafruitAudio::Play(uint8_t trackNumber)
{
    if (!_isInitialized) return;
    if (_sfx->playTrack(trackNumber))
    {
        _currentTrack = trackNumber;
        _isPlaying    = true;
    }
}

void AdafruitAudio::Stop()
{
    if (!_isInitialized) return;
    _sfx->stop();
    _isPlaying = false;
}

void AdafruitAudio::Pause()
{
    if (!_isInitialized) return;
    _sfx->pause();
    _isPlaying = false;
}

void AdafruitAudio::Resume()
{
    if (!_isInitialized) return;
    _sfx->unpause();
    _isPlaying = true;
}

void AdafruitAudio::SetVolume(uint8_t volume)
{
    if (!_isInitialized) return;
    uint8_t target = min(volume, VOLUME_MAX);
    while (_volume < target) { _sfx->volUp();   _volume++; }
    while (_volume > target) { _sfx->volDown(); _volume--; }
}

uint8_t AdafruitAudio::GetVolume() const  { return _volume; }
uint8_t AdafruitAudio::GetCurrentTrack() const { return _currentTrack; }

bool AdafruitAudio::IsPlaying() const
{
    // BUSY pin is LOW while audio is actively playing — more reliable than state tracking
    if (_busyPin >= 0)
        return digitalRead(_busyPin) == LOW;

    return _isPlaying;
}