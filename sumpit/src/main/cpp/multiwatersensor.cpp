#include "multiwatersensor.h"

MultiWaterSensor::MultiWaterSensor()
  :WaterLevelSensor()
{
  //ctor
}

MultiWaterSensor::~MultiWaterSensor()
{
  //dtor
}

void MultiWaterSensor::addSensor(WaterLevelSensor* _sensor)
{
  if (numSensors == SPN_MULTWATER_MAX_SENSORS) {
    return;
  }
  sensors[numSensors++] = _sensor;
}

short MultiWaterSensor::measureLevel()
{
  int sum = 0;
  int numValid = 0;

  for (int i = 0; i<numSensors; ++i) {
    short level = sensors[i]->measureLevel();
    if (level != SPN_WATER_LEVEL_UNKNOWN) {
      sum += level;
      numValid++;
    }
  }
  if (numValid != 0) {
    return sum / numValid;
  } else {
    return SPN_WATER_LEVEL_UNKNOWN;
  }
}
