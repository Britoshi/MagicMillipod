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
    _enterTime       = TimeManager::Instance().GetTime();
    _lastRepeatTime  = _enterTime;
    _personLeftSince = -1.0;
    NeoPixels.Ring1Fade(255, 0, 0, 0.0f);
    NeoPixels.Ring2Fade(255, 255, 255, 0.0f);
    NeoPixels.Ring3Fade(255, 0, 0, 0.0f);
    AdafruitAudio::Instance().PlayTrack(2);
}

void ExitRoomState::OnStateExit()
{
    Serial.println("[ExitRoom] Exited");
    NeoPixels.Off();
    NeoPixels.Ring2Off();
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

    float t = (float)((now - _enterTime) / FADE_DURATION);
    if (t > 1.0f) t = 1.0f;

    NeoPixels.Ring1Fade(255, 0, 0, t);
    NeoPixels.Ring2Fade(255, 255, 255, t);
    NeoPixels.Ring3Fade(255, 0, 0, t);
}

bool ExitRoomState::CheckSwitchState()
{
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;

    float dist       = DistanceSensorManager::Instance().GetAverageDistance();
    bool  personLeft = CalibrationData::Instance().HasPersonLeft(dist);
    double now       = TimeManager::Instance().GetTime();

    if (elapsed >= DURATION)
    {
        if (personLeft)
        {
            if (_personLeftSince < 0.0) _personLeftSince = now;
            if (now - _personLeftSince >= LEFT_DEBOUNCE)
                return SwitchState(&context->factory.idleState);
        }
        else
        {
            _personLeftSince = -1.0;
        }
    }

    return false;
}

void ExitRoomState::InitializeSubState() {}
