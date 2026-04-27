#include "IdleState.hpp"
#include "StateMachine.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "Time.hpp"

void IdleState::OnStateEnter()
{
    Serial.println("[Idle] Entered");
    NeoPixels.SetAll(0, 0, 255);
    UVLight.SetBrightness(0.0f);

    bool alreadyTriggered = CalibrationData::Instance().IsPersonPresent(
        DistanceSensorManager::Instance().GetAverageDistance());

    if (alreadyTriggered)
    {
        _primed         = false;
        _clearStartTime = -1.0;
        Serial.println("[Idle] Sensor suppressed on enter: waiting for clear");
    }
    else
    {
        _primed = true;
    }
}

void IdleState::OnStateExit()
{
    Serial.println("[Idle] Exited");
    NeoPixels.Ring2Off();
}

void IdleState::OnStateUpdate()
{
    NeoPixels.Ring2Rainbow();

    if (!_primed)
    {
        double now  = TimeManager::Instance().GetTime();
        float  dist = DistanceSensorManager::Instance().GetAverageDistance();
        bool   clear = !CalibrationData::Instance().IsPersonPresent(dist);

        if (clear)
        {
            if (_clearStartTime < 0.0) _clearStartTime = now;
            if (now - _clearStartTime >= CLEAR_DURATION)
            {
                _primed = true;
                Serial.println("[Idle] Sensor primed");
            }
        }
        else
        {
            _clearStartTime = -1.0;
        }
    }
}

bool IdleState::CheckSwitchState()
{
    if (!_primed) return false;
    float dist = DistanceSensorManager::Instance().GetAverageDistance();
    if (CalibrationData::Instance().IsPersonPresent(dist))
        return SwitchState(&context->factory.introductionState);
    return false;
}

void IdleState::InitializeSubState() {}
