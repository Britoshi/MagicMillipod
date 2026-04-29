#include "State.hpp"
#include "StateMachine.hpp"
#include "Time.hpp"

bool State::SwitchState(State *state)
{
    Exit();
    if (context->currentState == this)
    {
        context->currentState = state;
        state->Enter();
    }
    else
        superState->SetSubState(state);

    return true;
}

void State::SetSuperState(State *superState)
{
    this->superState = superState;
}

void State::SetSubState(State *subState)
{
    this->subState = subState;
    subState->SetSuperState(this);
    subState->Enter();
}

void State::Enter()
{
    InitializeSubState();
    OnStateEnter();
}

void State::Exit()
{ 
    OnStateExit();

    if (subState != nullptr)
        subState->Exit();
}

void State::Update()
{
    double now = TimeManager::Instance().GetTime();
    if (now - _lastCheckTime >= CHECK_INTERVAL)
    {
        _lastCheckTime = now;
        if (CheckSwitchState())
            return;
    }
    OnStateUpdate();

    if (subState != nullptr)
        subState->Update();
} 
