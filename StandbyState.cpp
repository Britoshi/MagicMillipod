#include "StandbyState.hpp"
#include "StateMachine.hpp"

void StandbyState::OnStateEnter() {}
void StandbyState::OnStateExit() {}
void StandbyState::OnStateUpdate() {}

bool StandbyState::CheckSwitchState() { return false; }
void StandbyState::InitializeSubState() {}