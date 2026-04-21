#include "PlayAudioState.hpp"
#include "StateMachine.hpp"
#include "AdafruitAudio.hpp"
#include "LEDController.hpp"
#include "UVLightController.hpp"
#include "DistanceSensorManager.hpp"
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

void PlayAudioState::OnStateUpdate()
{
    float distance = DistanceSensorManager::Instance().distance;
    if (distance == _lastDistance) return;
    _lastDistance = distance;

    float ratio = distance / 200.0f;
    Serial.print("[PlayAudio] Distance: ");
    Serial.print(distance);
    Serial.print(" cm | Ratio: ");
    Serial.println(ratio);
    UVLight.SetBrightness(ratio);
}

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
