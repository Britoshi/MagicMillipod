#pragma once
#include "StandbyState.hpp"

class StateFactory
{
public:
    explicit StateFactory(StateMachine *context)
        : standbyState(context) {}

    StandbyState *GetStandbyState() { return &standbyState; }

private:
    StandbyState standbyState; // owned — lives and dies with the factory
};