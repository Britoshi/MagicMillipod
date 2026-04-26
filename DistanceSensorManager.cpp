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
    if (now - lastTickTime >= (1.0 / TICK_RATE))
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

    _buffer[_bufferIndex] = distance;
    _bufferIndex = (_bufferIndex + 1) % BUFFER_SIZE;
    if (_bufferCount < BUFFER_SIZE) _bufferCount++;

    Serial.print("[Distance] ");
    Serial.print(distance);
    Serial.println(" cm");
}

float DistanceSensorManager::GetAverageDistance() const
{
    if (_bufferCount == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < _bufferCount; i++) sum += _buffer[i];
    return sum / _bufferCount;
}
