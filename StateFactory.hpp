#pragma once
#include "IdleState.hpp"
#include "IntroductionState.hpp"
#include "PresentationState.hpp"

class StateFactory
{
public:
    explicit StateFactory(StateMachine *context)
        : idleState(context),
          introductionState(context),
          presentationState(context) {}

    IdleState         idleState;
    IntroductionState introductionState;
    PresentationState presentationState;
};
