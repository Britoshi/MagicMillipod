#include "StateMachine.hpp"
#include "State.hpp" 

StateMachine::StateMachine()
    : factory(this),
      currentState(&factory.idleState)
{
    currentState->Enter();
}

StateMachine::~StateMachine() {}

void StateMachine::Update()
{
    currentState->Update();
}

void StateMachine::ResetToIdle()
{
    currentState->Exit();
    currentState = &factory.idleState;
    currentState->Enter();
}