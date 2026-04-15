#pragma once
#include "StandbyState.hpp"
#include "PlayAudioState.hpp"

class StateFactory
{
public:
    explicit StateFactory(StateMachine *context)
        : standbyState(context),
          playAudioState(context) {}

    StandbyState *GetStandbyState() { return &standbyState; }

    StandbyState  standbyState;
    PlayAudioState playAudioState;
};