#pragma once
#include "StateFactory.hpp"

class State;

class StateMachine
{
    friend class State;
public:
    State *currentState = nullptr;

    StateMachine();
    ~StateMachine();

    void Update();

public:
    StateFactory factory;
};