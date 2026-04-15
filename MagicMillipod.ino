#include "Time.hpp"
#define Time TimeManager::Instance()
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "DistanceSensorManager.hpp"
#include "StateMachine.hpp"

#define SFX_TRIGGER  5

StateMachine *stateMachine = nullptr;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    AdafruitAudio::Instance().Begin(SFX_TRIGGER);
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
