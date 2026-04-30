#include "Time.hpp"
#define Time TimeManager::Instance()
#include "VisitCounter.hpp"
#include "CalibrationData.hpp"
#include "UVLightController.hpp"
#include "NeoPixelController.hpp"
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "DistanceSensorManager.hpp"
#include "SwitchController.hpp"
#include "StateMachine.hpp"

#define SFX_TRIGGER_0       4
#define SFX_TRIGGER_1       2
#define SFX_TRIGGER_2       3
#define SFX_RESET           5
#define NEOPIXEL3_PIN      16
#define RESET_BUTTON       15

StateMachine *stateMachine    = nullptr;
static bool     _btnWasPressed = false;
static uint32_t _btnPressTime  = 0;

void setup()
{
    pinMode(SFX_RESET, OUTPUT);     digitalWrite(SFX_RESET, HIGH);
    pinMode(SFX_TRIGGER_0, OUTPUT); digitalWrite(SFX_TRIGGER_0, HIGH);
    pinMode(SFX_TRIGGER_1, OUTPUT); digitalWrite(SFX_TRIGGER_1, HIGH);
    pinMode(SFX_TRIGGER_2, OUTPUT); digitalWrite(SFX_TRIGGER_2, HIGH);

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    Switches.Begin();
    UVLight.Begin();
    AdafruitAudio::Instance().Begin(SFX_TRIGGER_0, SFX_TRIGGER_1, SFX_TRIGGER_2, SFX_RESET);
    DistanceSensorManager::Instance().Start();
    NeoPixels.Begin();
    VisitCount.Begin();
    CalibrationData::Instance().Load();
    stateMachine = new StateMachine();
    delay(3000);
}

void loop()
{
    Time.Update(millis());
    DistanceSensorManager::Instance().Update();

    bool btnDown = digitalRead(RESET_BUTTON) == LOW;
    if (btnDown && !_btnWasPressed)
    {
        _btnPressTime  = millis();
        _btnWasPressed = true;
    }
    else if (!btnDown && _btnWasPressed)
    {
        uint32_t held = millis() - _btnPressTime;
        if (held >= 1000)
            stateMachine->ResetToIdle();
        else if (stateMachine->currentState == &stateMachine->factory.idleState)
            stateMachine->EnterCalibration();
        else
            stateMachine->AdvanceToNextState();
        _btnWasPressed = false;
    }

    stateMachine->Update();
}
