#include "DistanceSensorManager.hpp"
#include <Arduino.h>

#define TRIG_PIN 12
#define ECHO_PIN 13

void DistanceSensorManager::Start()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void DistanceSensorManager::Update()
{
    double now = TimeManager::Instance().GetTime();
    double difference = now - lastTickTime;

    if (difference >= (1.0 / TICK_RATE))
    {
        Tick();
        lastTickTime = now;
    }
}

void DistanceSensorManager::Tick()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * 0.0343f) / 2.0f;
}
