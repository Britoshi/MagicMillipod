#include "IntroductionState.hpp"
#include "StateMachine.hpp"
#include "AdafruitAudio.hpp"
#include "NeoPixelController.hpp"
#include "Time.hpp"
#include "UVLightController.hpp"
#include "DistanceSensorManager.hpp"

void IntroductionState::OnStateEnter()
{
    Serial.println("[Introduction] Entered");
    _enterTime = TimeManager::Instance().GetTime();
    NeoPixels.SetAll(255, 0, 0);
    NeoPixels.Ring2Rainbow();
    UVLight.SetBrightness(0.0f);
    AdafruitAudio::Instance().PlayTrack(0);
}

void IntroductionState::OnStateExit()
{
    Serial.println("[Introduction] Exited");
    AdafruitAudio::Instance().Reset();
}

void IntroductionState::OnStateUpdate()
{
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;
    double remaining = DURATION - elapsed;

    float fadeProgress = (float)((remaining - FADE_OFFSET) / FADE_DURATION);
    if (fadeProgress > 1.0f) fadeProgress = 1.0f;
    if (fadeProgress < 0.0f) fadeProgress = 0.0f;

    NeoPixels.Ring2Rainbow(fadeProgress);
}

bool IntroductionState::CheckSwitchState()
{
    if (TimeManager::Instance().GetTime() - _enterTime >= DURATION)
        return SwitchState(&context->factory.presentationState);
    return false;
}

void IntroductionState::InitializeSubState() {}
