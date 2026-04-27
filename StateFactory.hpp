#pragma once
#include "IdleState.hpp"
#include "IntroductionState.hpp"
#include "PresentationState.hpp"
#include "CalibrationState.hpp"
#include "ExitRoomState.hpp"

class StateFactory
{
public:
    explicit StateFactory(StateMachine *context)
        : idleState(context),
          introductionState(context),
          presentationState(context),
          calibrationState(context),
          exitRoomState(context) {}

    IdleState         idleState;
    IntroductionState introductionState;
    PresentationState presentationState;
    CalibrationState  calibrationState;
    ExitRoomState     exitRoomState;
};
