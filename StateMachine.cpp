#include "StateMachine.hpp"
#include "State.hpp"
#include "AdafruitAudio.hpp"

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

void StateMachine::EnterCalibration()
{
    currentState->Exit();
    currentState = &factory.calibrationState;
    currentState->Enter();
}

void StateMachine::AdvanceToNextState()
{
    AdafruitAudio::Instance().Reset();

    State *next = nullptr;
    if      (currentState == &factory.introductionState)  next = &factory.presentationState;
    else if (currentState == &factory.presentationState)  next = &factory.exitRoomState;
    else if (currentState == &factory.exitRoomState)      next = &factory.idleState;

    if (next)
    {
        currentState->Exit();
        currentState = next;
        currentState->Enter();
    }
}