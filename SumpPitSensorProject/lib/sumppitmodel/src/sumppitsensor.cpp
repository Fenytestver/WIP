#include "sumppitsensor.h"

SumpPitSensor::SumpPitSensor(WaterLevelSensor* _waterLevelSensors, int _numWaterSensors,
                  LeakSensor* _leakSensors, int _numLeakSensors,
                  MultiPump* _pump)
{
  //ctor
  waterLevelSensors = _waterLevelSensors;
  leakSensors = _leakSensors;
  numWaterSensors = _numWaterSensors;
  pump = _pump;
}

SumpPitSensor::~SumpPitSensor()
{
  //dtor
}

void SumpPitSensor::setup() {

}

bool SumpPitSensor::isLeaking()
{
  // FIXME: multiple leak sensors.
  return leakSensors->isLeaking();
}
int SumpPitSensor::checkState()
{
  int state = SPN_ALARM_NO_ALARM;
  state |= checkPumpState();
  state |= checkWaterLevelState();
  state |= checkLeakState();
  return state;
}

int SumpPitSensor::checkPumpState()
{
  return pump->checkState();
}

int SumpPitSensor::checkWaterLevelState()
{
  return waterLevelSensors->measureLevel() >= SPN_ALARM_WATER_CRITICAL ?
    SPN_ALARM_WATER_CRITICAL : SPN_ALARM_NO_ALARM;
}

int SumpPitSensor::checkLeakState()
{
  return isLeaking() ? SPN_ALARM_LEAK : SPN_ALARM_NO_ALARM;
}

void SumpPitSensor::updatePump()
{
  pump->update();
  int waterLevel = waterLevelSensors->measureLevel();
  if (waterLevel >= SPN_WATER_CRITICAL) {
    expectedPumpState = SPN_MULTIPUMP_TURBO;
  } else if (waterLevel >= SPN_WATER_HIGH) {
    expectedPumpState = SPN_MULTIPUMP_ON;
  } else if (waterLevel <= SPN_WATER_LOW) {
    expectedPumpState = SPN_MULTIPUMP_OFF;
  }
}
