#include "Time.hpp"
#define Time TimeManager::Instance()
#include "pico/multicore.h"
#include "Adafruit_Soundboard.h"

#define SFX_RX_PIN 0
#define SFX_TX_PIN 1
#define SFX_RST 2
#define SFX_BUSY 3

// HardwareSerial Serial1(1);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

void SetupAudio()
{
  Serial1.begin(9600);

  if (!sfx.reset())
  {
    Serial.println("Sound board not found");
    while (1)
      ;
  }

  Serial.println("Soundboard ready");
}

void PlayTrack(uint8_t trackNumber)
{ 
  sfx.playTrack(trackNumber);
}

void setup()
{
  Serial.begin(115200);
  SetupAudio();
}

void loop()
{
  Time.Update(millis());
  
}
