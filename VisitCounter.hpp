#pragma once
#include <LittleFS.h>
#include <Arduino.h>

#define VisitCount VisitCounterManager::Instance()

class VisitCounterManager {
public:
    static VisitCounterManager &Instance()
    {
        static VisitCounterManager instance;
        return instance;
    }

    VisitCounterManager(const VisitCounterManager &) = delete;
    VisitCounterManager &operator=(const VisitCounterManager &) = delete;

    void Begin()
    {
        if (!LittleFS.begin())
        {
            LittleFS.format();
            LittleFS.begin();
        }

        File f = LittleFS.open("/visits.txt", "r");
        if (f)
        {
            _count = (uint32_t)f.parseInt();
            f.close();
        }

        Serial.print("[VisitCounter] Loaded count: ");
        Serial.println(_count);
    }

    void Increment()
    {
        _count++;
        File f = LittleFS.open("/visits.txt", "w");
        if (f)
        {
            f.println(_count);
            f.close();
        }
        Serial.print("[VisitCounter] Visit #");
        Serial.println(_count);
    }

    uint32_t GetCount() const { return _count; }

private:
    VisitCounterManager() = default;
    uint32_t _count = 0;
};
