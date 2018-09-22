#include "waterlevelsensor.h"

WaterLevelSensor::WaterLevelSensor()
{
  //ctor
}

WaterLevelSensor::~WaterLevelSensor()
{
  //dtor
}
short WaterLevelSensor::measureLevel()
{
  return SPN_WATER_LEVEL_UNKNOWN;
}
