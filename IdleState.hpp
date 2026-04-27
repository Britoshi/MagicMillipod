#pragma once
#include "State.hpp"

class IdleState : public State
{
public:
    explicit IdleState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~IdleState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;

    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr float TRIGGER_DISTANCE_CM = 30.0f;
};
