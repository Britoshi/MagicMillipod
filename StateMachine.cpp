#include "StateMachine.hpp"
#include "State.hpp" 

StateMachine::StateMachine()
    : factory(this),
      currentState(factory.GetStandbyState())
{
    currentState->Enter();
}

StateMachine::~StateMachine() {}

void StateMachine::Update()
{
    currentState->Update();
}