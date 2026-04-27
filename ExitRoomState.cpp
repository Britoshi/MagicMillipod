#include "ExitRoomState.hpp"
#include "StateMachine.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "SwitchController.hpp"
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
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;

    float dist = DistanceSensorManager::Instance().GetAverageDistance();
    bool  personLeft = Switches.IsOn(0)
        ? dist >= SIMPLE_EXIT_DISTANCE_CM
        : CalibrationData::Instance().HasPersonLeft(dist);

    if (elapsed >= DURATION && personLeft)
        return SwitchState(&context->factory.idleState);

    return false;
}

void ExitRoomState::InitializeSubState() {}
