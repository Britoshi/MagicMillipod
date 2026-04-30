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
    _enterTime      = TimeManager::Instance().GetTime();
    _uvBrightness   = 0.0f;
    _personLeftTime = -1.0;
    _personSeen     = false;
    NeoPixels.SetAll(255, 0, 0);
    NeoPixels.Ring2Off();
    NeoPixels.Ring3Off();
    UVLight.SetBrightness(0.0f);
    AdafruitAudio::Instance().PlayTrack(1);
}

void PresentationState::OnStateExit()
{
    Serial.println("[Presentation] Exited"); 
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
    double now = TimeManager::Instance().GetTime();

    if (!Switches.IsOn(1))
    {
        float dist = DistanceSensorManager::Instance().GetAverageDistance();
        bool personLeft = CalibrationData::Instance().HasPersonLeft(dist);

        if (!personLeft) _personSeen = true;

        if (_personSeen)
        {
            if (personLeft)
            {
                if (_personLeftTime < 0.0) _personLeftTime = now;
                if (now - _personLeftTime >= 10.0)
                    return SwitchState(&context->factory.idleState);
            }
            else
            {
                _personLeftTime = -1.0;
            }
        }
    }

    if (now - _enterTime >= DURATION)
        return SwitchState(&context->factory.exitRoomState);

    return false;
}

void PresentationState::InitializeSubState() {}
