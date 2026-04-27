#ifndef DISTANCE_SENSOR_MANAGER_HPP
#define DISTANCE_SENSOR_MANAGER_HPP

#include "Time.hpp"

class DistanceSensorManager {
    const double TICK_RATE = 20;
    void Tick();

public:
    static DistanceSensorManager &Instance()
    {
        static DistanceSensorManager instance;
        return instance;
    }

    DistanceSensorManager(const DistanceSensorManager &) = delete;
    DistanceSensorManager &operator=(const DistanceSensorManager &) = delete;

    float duration = 0.0;
    float distance = 0.0;
    double lastTickTime = 0.0;

    void Start();
    void Update();
    float GetAverageDistance() const;
    float GetLongAverageDistance() const;

private:
    DistanceSensorManager() = default;

    static const int BUFFER_SIZE      = 4;
    static const int LONG_BUFFER_SIZE = 60; // 3s at 20Hz

    float _buffer[BUFFER_SIZE]           = {};
    int   _bufferIndex                   = 0;
    int   _bufferCount                   = 0;

    float _longBuffer[LONG_BUFFER_SIZE]  = {};
    int   _longBufferIndex               = 0;
    int   _longBufferCount               = 0;
};

#endif
