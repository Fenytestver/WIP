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

int WaterLevelSensor::checkState()
{
  return measureLevel() == SPN_WATER_LEVEL_UNKNOWN ? SPN_ALARM_SYSTEM_ERROR : SPN_ALARM_NO_ALARM;
}
