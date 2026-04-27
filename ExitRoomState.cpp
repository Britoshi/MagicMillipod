#include "ExitRoomState.hpp"
#include "StateMachine.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "Time.hpp"

void ExitRoomState::OnStateEnter()
{
    Serial.println("[ExitRoom] Entered");
    _enterTime = TimeManager::Instance().GetTime();
    UVLight.SetBrightness(0.0f);
    NeoPixels.SetAll(0, 255, 0);
}

void ExitRoomState::OnStateExit()
{
    Serial.println("[ExitRoom] Exited");
    NeoPixels.Off();
}

void ExitRoomState::OnStateUpdate() {}

bool ExitRoomState::CheckSwitchState()
{
    double elapsed   = TimeManager::Instance().GetTime() - _enterTime;
    float  dist      = DistanceSensorManager::Instance().GetAverageDistance();
    bool   personLeft = CalibrationData::Instance().HasPersonLeft(dist);

    if (personLeft || elapsed >= DURATION)
        return SwitchState(&context->factory.idleState);
    return false;
}

void ExitRoomState::InitializeSubState() {}
