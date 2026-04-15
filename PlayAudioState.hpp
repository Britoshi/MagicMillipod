#pragma once
#include "State.hpp"

class PlayAudioState : public State
{
public:
    explicit PlayAudioState(StateMachine *context, State *superState = nullptr)
        : State(context, superState) {}

    ~PlayAudioState() = default;

protected:
    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate() override;

    bool CheckSwitchState() override;
    void InitializeSubState() override;

private:
    static constexpr double PLAY_DURATION = 10.0;
    double _enterTime = 0.0;
};
