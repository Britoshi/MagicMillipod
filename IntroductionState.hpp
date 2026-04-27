#pragma once
#include "State.hpp"

class IntroductionState : public State
{
public:
    explicit IntroductionState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~IntroductionState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;

    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr double DURATION               = 51.0;
    static constexpr double FADE_DURATION          = 10.0;
    static constexpr double FADE_OFFSET            = 5.0;
    static constexpr float  SIMPLE_EXIT_DISTANCE_CM = 50.0f;
    double _enterTime = 0.0;
};
