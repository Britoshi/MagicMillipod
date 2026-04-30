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
    static constexpr double DURATION        = 10.0;
    static constexpr double REPEAT_INTERVAL = 10.0;
    static constexpr float  FADE_DURATION   = 5.0f;
    static constexpr double LEFT_DEBOUNCE   = 1.0;
    double _enterTime       = 0.0;
    double _lastRepeatTime  = 0.0;
    double _personLeftSince = -1.0;
};
