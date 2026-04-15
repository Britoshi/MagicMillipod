#include "StandbyState.hpp"
#include "StateMachine.hpp"
#include "pico/multicore.h"
#include "AdafruitAudio.hpp"

void StandbyState::OnStateEnter()
{
    Serial.println("[Standby] Entered");
    AdafruitAudio::Instance().Play();
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
} 

bool StandbyState::CheckSwitchState() { return false; }
void StandbyState::InitializeSubState() {}