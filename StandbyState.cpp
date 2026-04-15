#include "StandbyState.hpp"
#include "StateMachine.hpp"
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"
#include "DistanceSensorManager.hpp"
#include "LEDController.hpp"

static const float TRIGGER_DISTANCE_CM = 30.0f;

void StandbyState::OnStateEnter()
{
    Serial.println("[Standby] Entered");
    LED.Toggle(BLUE, true);
    LED.Toggle(RED, false);
    // AdafruitAudio::Instance().Play();
}

void StandbyState::OnStateExit()
{
    Serial.println("[Standby] Exited");
}

void StandbyState::OnStateUpdate() {
  digitalWrite(LED_BUILTIN , HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN , LOW);
  delay(100);

  Serial.print("[Standby] Distance: ");
  Serial.print(DistanceSensorManager::Instance().distance);
  Serial.println(" cm");
} 

bool StandbyState::CheckSwitchState()
{
    if (DistanceSensorManager::Instance().GetAverageDistance() < TRIGGER_DISTANCE_CM)
        return SwitchState(&context->factory.playAudioState);
    return false;
}
void StandbyState::InitializeSubState() {}