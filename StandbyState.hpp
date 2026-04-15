#pragma once
#include "State.hpp"

class StandbyState : public State
{
public:
    explicit StandbyState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~StandbyState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override; 

    bool CheckSwitchState() override;
    void InitializeSubState() override;
};