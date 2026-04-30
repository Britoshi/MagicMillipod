#include "DistanceSensorManager.hpp"
#include <Arduino.h>

#define TRIG_PIN 12
#define ECHO_PIN 13

void DistanceSensorManager::Start()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    for (int i = 0; i < BUFFER_SIZE; i++)      _buffer[i]     = 200.0f;
    for (int i = 0; i < LONG_BUFFER_SIZE; i++) _longBuffer[i] = 200.0f;
    _bufferCount     = BUFFER_SIZE;
    _longBufferCount = LONG_BUFFER_SIZE;
}

void DistanceSensorManager::Update()
{
    timer += TimeManager::Instance().GetDeltaTime();
    if (timer >= (1.0 / TICK_RATE))
    {
        timer = 0.0;
        Tick();
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

    if (distance < 2.0f) return;  // no echo / timeout, keep buffer as-is

    float sample = (distance > 200.0f) ? 100.0f : distance;

    _buffer[_bufferIndex] = sample;
    _bufferIndex = (_bufferIndex + 1) % BUFFER_SIZE;
    if (_bufferCount < BUFFER_SIZE) _bufferCount++;

    _longBuffer[_longBufferIndex] = sample;
    _longBufferIndex = (_longBufferIndex + 1) % LONG_BUFFER_SIZE;
    if (_longBufferCount < LONG_BUFFER_SIZE) _longBufferCount++;

    Serial.print("[Distance] ");
    Serial.print(GetAverageDistance());
    Serial.println(" cm");
}

float DistanceSensorManager::GetAverageDistance() const
{
    if (_bufferCount == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < _bufferCount; i++) sum += _buffer[i];
    return sum / _bufferCount;
}

float DistanceSensorManager::GetLongAverageDistance() const
{
    if (_longBufferCount == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < _longBufferCount; i++) sum += _longBuffer[i];
    return sum / _longBufferCount;
}
