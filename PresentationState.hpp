#pragma once
#include "State.hpp"

class PresentationState : public State
{
public:
    explicit PresentationState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~PresentationState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;

    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr double DURATION               = 60.0 * 2.0 + 1.0;
    static constexpr float  UV_RAMP_SPEED          = 0.1f;
    static constexpr float  UV_FADE_SPEED          = 0.1f;
    static constexpr double UV_FADE_START          = 80.0; 
    double _enterTime      = 0.0;
    float  _uvBrightness   = 0.0f;
    double _personLeftTime = -1.0;
    bool   _personSeen     = false;
};
