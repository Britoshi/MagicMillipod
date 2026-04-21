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

private:
    DistanceSensorManager() = default;

    static const int BUFFER_SIZE = 4; // 1 second at 4 Hz
    float _buffer[BUFFER_SIZE] = {};
    int _bufferIndex = 0;
    int _bufferCount = 0;
};

#endif
