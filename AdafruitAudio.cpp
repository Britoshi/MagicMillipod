#include "AdafruitAudio.hpp"
#define TX_PIN 1
#define RX_PIN 0

AdafruitAudio::AdafruitAudio()
{
}

AdafruitAudio::~AdafruitAudio()
{
}

bool AdafruitAudio::Begin(uint8_t resetPin)
{
    return false;
}

void AdafruitAudio::Reset()
{
}

void AdafruitAudio::Play(uint8_t trackNumber)
{
}

void AdafruitAudio::Stop()
{
}

void AdafruitAudio::Pause()
{
}

void AdafruitAudio::Resume()
{
}

void AdafruitAudio::SetVolume(uint8_t volume)
{
}

uint8_t AdafruitAudio::GetVolume() const
{
    return 0;
}

bool AdafruitAudio::IsPlaying() const
{
    return false;
}

uint8_t AdafruitAudio::GetCurrentTrack() const
{
    return 0;
}
