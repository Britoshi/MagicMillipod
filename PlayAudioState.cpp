#include "PlayAudioState.hpp"
#include "StateMachine.hpp"
#include "AdafruitAudio.hpp"
#include "LEDController.hpp"
#include "Time.hpp"

void PlayAudioState::OnStateEnter()
{
    Serial.println("[PlayAudio] Entered");
    _enterTime = TimeManager::Instance().GetTime();
    LED.Toggle(RED, true);
    LED.Toggle(BLUE, false);
    AdafruitAudio::Instance().Play();
}

void PlayAudioState::OnStateExit()
{
    Serial.println("[PlayAudio] Exited");
}

void PlayAudioState::OnStateUpdate() {}

bool PlayAudioState::CheckSwitchState()
{
    if (TimeManager::Instance().GetTime() - _enterTime >= PLAY_DURATION)
    {
        AdafruitAudio::Instance().Reset();
        return SwitchState(&context->factory.standbyState);
    }
    return false;
}

void PlayAudioState::InitializeSubState() {}
