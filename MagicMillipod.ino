#include "Time.hpp"
#define Time TimeManager::Instance()
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "StateMachine.hpp"

#define SFX_RX_PIN  0
#define SFX_TX_PIN  1
#define SFX_RST     2
#define SFX_BUSY    3

AdafruitAudio audio;
StateMachine stateMachine;

void SetupAudio()
{
    if (!audio.Begin(SFX_RX_PIN, SFX_TX_PIN, SFX_RST, SFX_BUSY))
    {
        Serial.println("Soundboard not found — check wiring");
        while (1);
    }

    audio.SetVolume(7);
    Serial.println("Soundboard ready");
}

void setup()
{
    Serial.begin(115200);
    SetupAudio();
}

void loop()
{
    Time.Update(millis());
}