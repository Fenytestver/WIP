#include "waterlevelsensor.h"

WaterLevelSensor::WaterLevelSensor(int _waterLowIn, int _waterHighIn)
{
  //ctor
  lastAlarm = SPN_ALARM_NO_ALARM;
  waterLowIn = _waterLowIn;
  waterHighIn = _waterHighIn;
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
  state->levelIn = spn::mapp((long)level, SPN_WATER_LOW, SPN_WATER_HIGH, waterLowIn, waterHighIn);
  state->levelPercent = level;

  if (state->mode != SPN_ARMED) {
    lastAlarm = SPN_ALARM_NO_ALARM;
    return SPN_ALARM_NO_ALARM;
  }

  int currentAlarm = level >= SPN_WATER_CRITICAL ?
  SPN_ALARM_WATER_CRITICAL :
  level <= SPN_WATER_LOW ? SPN_ALARM_WATER_LOW : SPN_ALARM_NO_ALARM;

  // easy to turn on, hard to turn off
  if (lastAlarm == SPN_ALARM_NO_ALARM) {
    lastAlarm = currentAlarm;
  } else if (lastAlarm == SPN_ALARM_WATER_CRITICAL
             // alarm wants to turn off, but it needs to reach a threshold
             && level < SPN_WATER_CRITICAL - SPN_WATER_VARIANCE) {
    lastAlarm = currentAlarm;
  } else if (lastAlarm == SPN_ALARM_WATER_LOW
             && level > SPN_WATER_LOW + SPN_WATER_VARIANCE) {
    lastAlarm = currentAlarm;
  }
  return lastAlarm;
}
