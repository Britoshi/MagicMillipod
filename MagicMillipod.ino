#include "Time.hpp"
#define Time TimeManager::Instance() 

void setup()
{
  
}

void loop()
{
  Time.Update(millis());
}
