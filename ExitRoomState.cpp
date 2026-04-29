#include "ExitRoomState.hpp"
#include "StateMachine.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "AdafruitAudio.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "SwitchController.hpp"
#include "Time.hpp"

void ExitRoomState::OnStateEnter()
{
    Serial.println("[ExitRoom] Entered");
    _enterTime      = TimeManager::Instance().GetTime();
    _lastRepeatTime = _enterTime;
    UVLight.SetBrightness(0.0f);
    NeoPixels.SetAll(0, 255, 0);
    NeoPixels.Ring3SetAll(255, 0, 0);
    AdafruitAudio::Instance().PlayTrack(2);
}

void ExitRoomState::OnStateExit()
{
    Serial.println("[ExitRoom] Exited");
    AdafruitAudio::Instance().Reset();
    NeoPixels.Off();
    NeoPixels.Ring3Off();
}

void ExitRoomState::OnStateUpdate()
{
    double now = TimeManager::Instance().GetTime();
    if (now - _lastRepeatTime >= REPEAT_INTERVAL)
    {
        _lastRepeatTime = now;
        AdafruitAudio::Instance().PlayTrack(2);
    }
}

bool ExitRoomState::CheckSwitchState()
{
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;

    float dist = DistanceSensorManager::Instance().GetAverageDistance();
    bool  personLeft = CalibrationData::Instance().HasPersonLeft(dist);

    if (elapsed >= DURATION && personLeft)
        return SwitchState(&context->factory.idleState);

    return false;
}

void ExitRoomState::InitializeSubState() {}
