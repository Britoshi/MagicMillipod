#pragma once
#include <Arduino.h>

class StateMachine;

class State
{
protected:
    StateMachine *context = nullptr;
    State *superState = nullptr;
    State *subState = nullptr;
    double _lastCheckTime = 0.0;

    static constexpr double CHECK_INTERVAL = 0.25; // 4 Hz

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
    
    State(const State &) = delete;
    State &operator=(const State &) = delete;

    void Enter();
    void Exit();
    void Update(); 
}; 