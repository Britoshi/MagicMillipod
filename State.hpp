#pragma once

#include "StateMachine.hpp"

class State
{
protected:
    StateMachine *context = nullptr; // non-owning
    State *superState = nullptr;     // non-owning
    State *subState = nullptr;       // non-owning

protected:
    bool SwitchState(State *state);
    void SetSuperState(State *superState);
    void SetSubState(State *subState);

    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}
    virtual void OnStateUpdate() {}

    virtual bool CheckSwitchState() = 0;
    virtual void InitializeSubState() = 0;

public:
    explicit State(StateMachine *context, State *superState = nullptr)
        : context(context)
    {
        SetSuperState(superState);
    }

    virtual ~State() = default;

    State(const State &) = delete;
    State &operator=(const State &) = delete;

    void Enter();
    void Exit();
    void Update();
};