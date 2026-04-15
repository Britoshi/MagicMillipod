#include "Time.hpp"
#define Time TimeManager::Instance()
#include "LEDController.hpp"
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "DistanceSensorManager.hpp"
#include "StateMachine.hpp"

#define SFX_TRIGGER  4
#define SFX_RESET    5

StateMachine *stateMachine = nullptr;

void setup()
{
    pinMode(SFX_RESET, OUTPUT);
    digitalWrite(SFX_RESET, HIGH);

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    LED.Begin();
    AdafruitAudio::Instance().Begin(SFX_TRIGGER, SFX_RESET);
    DistanceSensorManager::Instance().Start();
    stateMachine = new StateMachine();
    delay(3000);
}

void loop()
{
    Time.Update(millis());
    DistanceSensorManager::Instance().Update();
    stateMachine->Update();
}
