#pragma once
#include "State.hpp"

class ExitRoomState : public State {
public:
    explicit ExitRoomState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~ExitRoomState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;
    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr double DURATION = 10.0;
    double _enterTime = 0.0;
};
