#include "IntroductionState.hpp"
#include "StateMachine.hpp"
#include "AdafruitAudio.hpp"
#include "NeoPixelController.hpp"
#include "CalibrationData.hpp"
#include "DistanceSensorManager.hpp"
#include "SwitchController.hpp"
#include "Time.hpp"
#include "UVLightController.hpp"
#include "VisitCounter.hpp"

void IntroductionState::OnStateEnter()
{
    Serial.println("[Introduction] Entered");
    VisitCount.Increment();
    _enterTime      = TimeManager::Instance().GetTime();
    _personLeftTime = -1.0;
    _personSeen     = false;
    NeoPixels.SetAll(255, 0, 0);
    NeoPixels.Ring2Rainbow();
    NeoPixels.Ring3Off();
    UVLight.SetBrightness(0.0f);
    AdafruitAudio::Instance().PlayTrack(0);
}

void IntroductionState::OnStateExit()
{
    Serial.println("[Introduction] Exited");
}

void IntroductionState::OnStateUpdate()
{
    double elapsed = TimeManager::Instance().GetTime() - _enterTime;
    double remaining = DURATION - elapsed;

    float fadeProgress = (float)((remaining - FADE_OFFSET) / FADE_DURATION);
    if (fadeProgress > 1.0f)
        fadeProgress = 1.0f;
    if (fadeProgress < 0.0f)
        fadeProgress = 0.0f;

    NeoPixels.Ring2Rainbow(fadeProgress);
}

bool IntroductionState::CheckSwitchState()
{
    double now = TimeManager::Instance().GetTime();

    if (!Switches.IsOn(1))
    {
        float dist = DistanceSensorManager::Instance().GetAverageDistance();
        bool personLeft = CalibrationData::Instance().HasPersonLeft(dist);

        if (!personLeft) _personSeen = true;

        if (_personSeen)
        {
            if (personLeft)
            {
                if (_personLeftTime < 0.0) _personLeftTime = now;
                if (now - _personLeftTime >= 10.0)
                    return SwitchState(&context->factory.idleState);
            }
            else
            {
                _personLeftTime = -1.0;
            }
        }
    }

    if (now - _enterTime >= DURATION)
        return SwitchState(&context->factory.presentationState);

    return false;
}

void IntroductionState::InitializeSubState() {}
