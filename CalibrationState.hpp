#pragma once
#include "State.hpp"

class CalibrationState : public State {
public:
    explicit CalibrationState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~CalibrationState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;
    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr double DURATION = 10.0;
    double _enterTime = 0.0;
    float  _sum       = 0.0f;
    float  _min       = 9999.0f;
    float  _max       = 0.0f;
    int    _count     = 0;
};
