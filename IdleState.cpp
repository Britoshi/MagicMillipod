#include "IdleState.hpp"
#include "StateMachine.hpp"
#include "DistanceSensorManager.hpp"
#include "NeoPixelController.hpp"
#include "UVLightController.hpp"
#include "Time.hpp"

void IdleState::OnStateEnter()
{
    Serial.println("[Idle] Entered");
    NeoPixels.SetAll(0, 0, 255);
    UVLight.SetBrightness(0.0f);
}

void IdleState::OnStateExit()
{
    Serial.println("[Idle] Exited");
    NeoPixels.Ring2Off();
}

void IdleState::OnStateUpdate()
{
    NeoPixels.Ring2Rainbow();
}

bool IdleState::CheckSwitchState()
{
    if (DistanceSensorManager::Instance().GetAverageDistance() < TRIGGER_DISTANCE_CM)
        return SwitchState(&context->factory.introductionState);
    return false;
}

void IdleState::InitializeSubState() {}
