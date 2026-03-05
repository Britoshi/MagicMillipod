#include "State.hpp"

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
    // No need to do exit stuff if it never even initialized.
    Exit();

    if (subState != nullptr)
        subState->Exit();
}

void State::Update()
{
    if (CheckSwitchState())
        return;
    OnStateUpdate();

    if (subState != nullptr)
        subState->Update();
}
