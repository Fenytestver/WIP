#include "waterlevelsensor.h"

WaterLevelSensor::WaterLevelSensor(int _waterLowIn, int _waterHighIn, int _waterPercentHigh)
{
  //ctor
  lastAlarm = SPN_ALARM_NO_ALARM;
  waterLowIn = _waterLowIn;
  waterHighIn = _waterHighIn;
  waterPercentHigh = _waterPercentHigh;
}

int WaterLevelSensor::getWaterPercentHigh() {
  return waterPercentHigh;
}

WaterLevelSensor::~WaterLevelSensor()
{
  //dtor
}
short WaterLevelSensor::measureLevel()
{
  return SPN_WATER_LEVEL_UNKNOWN;
}

int WaterLevelSensor::checkState(State* state)
{
  int level = measureLevel();
  state->levelIn = (int)spn::mapp((double)level, (double)SPN_WATER_LOW, (double)waterPercentHigh,
      (double)waterLowIn, (double)waterHighIn);
  state->levelPercent = level;

  if (state->mode != SPN_ARMED) {
    lastAlarm = SPN_ALARM_NO_ALARM;
    return SPN_ALARM_NO_ALARM;
  }

  int currentAlarm = level >= SPN_WATER_CRITICAL ?
    SPN_ALARM_WATER_CRITICAL :
    level <= SPN_WATER_LOW - SPN_WATER_VARIANCE ? SPN_ALARM_WATER_LOW : SPN_ALARM_NO_ALARM;

  // easy to turn on, hard to turn off
  if (lastAlarm == SPN_ALARM_NO_ALARM) {
    lastAlarm = currentAlarm;
  } else if (lastAlarm == SPN_ALARM_WATER_CRITICAL
             // alarm wants to turn off, but it needs to reach a threshold
             && level < SPN_WATER_CRITICAL - SPN_WATER_VARIANCE) {
    lastAlarm = currentAlarm;
  } else if (lastAlarm == SPN_ALARM_WATER_LOW
             && level > SPN_WATER_LOW) {
    lastAlarm = currentAlarm;
  }
  return lastAlarm;
}
