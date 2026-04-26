#include "Time.hpp"
#define Time TimeManager::Instance()
#include "UVLightController.hpp"
#include "NeoPixelController.hpp"
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "DistanceSensorManager.hpp"
#include "SwitchController.hpp"
#include "StateMachine.hpp"

#define SFX_TRIGGER_0       4
#define SFX_TRIGGER_1       2
#define SFX_RESET           5
#define NEOPIXEL3_PIN      16
#define RESET_BUTTON       15

StateMachine *stateMachine = nullptr;

void setup()
{
    pinMode(SFX_RESET, OUTPUT);    digitalWrite(SFX_RESET, HIGH);
    pinMode(SFX_TRIGGER_0, OUTPUT); digitalWrite(SFX_TRIGGER_0, HIGH);
    pinMode(SFX_TRIGGER_1, OUTPUT); digitalWrite(SFX_TRIGGER_1, HIGH);

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    Switches.Begin();
    UVLight.Begin();
    AdafruitAudio::Instance().Begin(SFX_TRIGGER_0, SFX_TRIGGER_1, SFX_RESET);
    DistanceSensorManager::Instance().Start();
    NeoPixels.Begin();
    stateMachine = new StateMachine();
    delay(3000);
}

void loop()
{
    Time.Update(millis());
    DistanceSensorManager::Instance().Update();

    if (digitalRead(RESET_BUTTON) == LOW)
    {
        stateMachine->ResetToIdle();
        delay(300);
    }

    stateMachine->Update();
}
