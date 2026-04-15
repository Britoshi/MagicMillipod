#ifndef DISTANCE_SENSOR_MANAGER_HPP
#define DISTANCE_SENSOR_MANAGER_HPP

#include "Time.hpp"

class DistanceSensorManager {
    const double TICK_RATE = 4;
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

private:
    DistanceSensorManager() = default;
};

#endif
