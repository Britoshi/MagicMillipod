#pragma once
#include "SwitchController.hpp"

class CalibrationData {
public:
    static CalibrationData &Instance()
    {
        static CalibrationData instance;
        return instance;
    }

    float doorDistance = 0.0f;
    float errorRange   = 0.0f;
    bool  isCalibrated = false;

    bool IsPersonPresent(float distance) const
    {
        if(Switches.IsOn(0) || !isCalibrated) return distance < FALLBACK_DISTANCE_CM;
        return distance < (doorDistance - 1.5f * errorRange);
    }

    bool HasPersonLeft(float distance) const
    {
        return !IsPersonPresent(distance);
    }

private:
    CalibrationData() = default;
    static constexpr float FALLBACK_DISTANCE_CM = 60.0f;
};
