#include "CalibrationState.hpp"
#include "StateMachine.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "Time.hpp"

void CalibrationState::OnStateEnter()
{
    Serial.println("[Calibration] Entered");
    _enterTime = TimeManager::Instance().GetTime();
    _sum   = 0.0f;
    _min   = 9999.0f;
    _max   = 0.0f;
    _count = 0;
    UVLight.SetBrightness(0.0f);
    NeoPixels.SetAll(255, 255, 0);
}

void CalibrationState::OnStateExit()
{
    Serial.println("[Calibration] Exited");
    NeoPixels.Off();
}

void CalibrationState::OnStateUpdate()
{
    float d = DistanceSensorManager::Instance().GetAverageDistance();
    if (d > 0.0f)
    {
        _sum += d;
        if (d < _min) _min = d;
        if (d > _max) _max = d;
        _count++;
    }
}

bool CalibrationState::CheckSwitchState()
{
    if (TimeManager::Instance().GetTime() - _enterTime >= DURATION)
    {
        if (_count > 0)
        {
            CalibrationData &cal = CalibrationData::Instance();
            cal.doorDistance = _sum / _count;
            cal.errorRange   = (_max - _min) / 2.0f;
            cal.isCalibrated = true;
            Serial.print("[Calibration] Door: "); Serial.print(cal.doorDistance);
            Serial.print(" cm  Error: +-");      Serial.println(cal.errorRange);
            cal.Save();
        }
        return SwitchState(&context->factory.idleState);
    }
    return false;
}

void CalibrationState::InitializeSubState() {}
