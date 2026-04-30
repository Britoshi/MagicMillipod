#pragma once
#include "SwitchController.hpp"
#include <LittleFS.h>

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

    void Save()
    {
        File f = LittleFS.open("/calibration.txt", "w");
        if (!f) return;
        f.println(doorDistance);
        f.println(errorRange);
        f.println(isCalibrated ? 1 : 0);
        f.close();
        Serial.println("[Calibration] Saved to flash");
    }

    void Load()
    {
        File f = LittleFS.open("/calibration.txt", "r");
        if (!f) return;
        doorDistance = f.parseFloat();
        errorRange   = f.parseFloat();
        isCalibrated = f.parseInt() == 1;
        f.close();
        Serial.print("[Calibration] Loaded — Door: "); Serial.print(doorDistance);
        Serial.print(" cm  Error: +-");                Serial.println(errorRange);
    }

private:
    CalibrationData() = default;
    static constexpr float FALLBACK_DISTANCE_CM = 60.0f;
};
