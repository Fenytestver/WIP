#include "waterlevelsensor.h"

WaterLevelSensor::WaterLevelSensor()
{
  //ctor
  lastAlarm = SPN_ALARM_NO_ALARM;
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
  int currentAlarm = level >= SPN_WATER_CRITICAL ?
    SPN_ALARM_WATER_CRITICAL :
      level <= SPN_WATER_LOW ? SPN_ALARM_WATER_LOW : SPN_ALARM_NO_ALARM;
  state->levelIn = spn::mapp((long)level, (long)SPN_WATER_LOW, (long)SPN_WATER_CRITICAL, 15, 75);;
  state->levelPercent = spn::mapp((long)level, (long)SPN_WATER_LOW, (long)SPN_WATER_CRITICAL, 0L, 100L);

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
