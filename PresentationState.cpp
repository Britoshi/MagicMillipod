#include "PresentationState.hpp"
#include "StateMachine.hpp"
#include "AdafruitAudio.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "SwitchController.hpp"
#include "Time.hpp"

void PresentationState::OnStateEnter()
{
    Serial.println("[Presentation] Entered");
    _enterTime    = TimeManager::Instance().GetTime();
    _uvBrightness = 0.0f;
    NeoPixels.SetAll(255, 0, 0);
    NeoPixels.Ring2Off();
    UVLight.SetBrightness(0.0f);
    AdafruitAudio::Instance().PlayTrack(1);
}

void PresentationState::OnStateExit()
{
    Serial.println("[Presentation] Exited");
    AdafruitAudio::Instance().Reset();
    UVLight.SetBrightness(0.0f);
}

void PresentationState::OnStateUpdate()
{
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;
    float  delta   = (float)TimeManager::Instance().GetDeltaTime();

    if (elapsed < UV_FADE_START)
    {
        _uvBrightness += delta * UV_RAMP_SPEED;
        if (_uvBrightness > 1.0f) _uvBrightness = 1.0f;
    }
    else
    {
        _uvBrightness -= delta * UV_FADE_SPEED;
        if (_uvBrightness < 0.0f) _uvBrightness = 0.0f;
    }

    UVLight.SetBrightness(_uvBrightness);
}

bool PresentationState::CheckSwitchState()
{
    float dist      = DistanceSensorManager::Instance().GetLongAverageDistance();
    bool  personLeft = Switches.IsOn(0)
        ? dist >= SIMPLE_EXIT_DISTANCE_CM
        : CalibrationData::Instance().HasPersonLeft(dist);

    if (personLeft)
        return SwitchState(&context->factory.idleState);

    if (TimeManager::Instance().GetTime() - _enterTime >= DURATION)
        return SwitchState(&context->factory.exitRoomState);

    return false;
}

void PresentationState::InitializeSubState() {}
