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
  pump->setup();
}

bool SumpPitSensor::isLeaking()
{
  return leakSensors->isLeaking();
}
int SumpPitSensor::checkState(State* outstate)
{
  int state = SPN_ALARM_NO_ALARM;
  bool leakState = checkLeakState();
  state |= checkPumpState();
  state |= checkWaterLevelState(outstate);
  state |= leakState;
  outstate->leak = (leakState != SPN_ALARM_NO_ALARM);
  return state;
}

int SumpPitSensor::checkPumpState()
{
  return pump->checkState();
}

int SumpPitSensor::checkWaterLevelState(State* outstate)
{
  int state = waterLevelSensors->checkState(outstate);
  // TODO: rename waterlevelsensors to single
  return state;
}

int SumpPitSensor::checkLeakState()
{
  return isLeaking() ? SPN_ALARM_LEAK : SPN_ALARM_NO_ALARM;
}

void SumpPitSensor::updatePump(State* state)
{
  pump->update(state);
  int waterLevel = waterLevelSensors->measureLevel();
  if (waterLevel >= SPN_WATER_CRITICAL) {
    expectedPumpState = SPN_MULTIPUMP_TURBO;
  } else if (waterLevel >= SPN_WATER_HIGH) {
    expectedPumpState = SPN_MULTIPUMP_ON;
  } else if (waterLevel <= SPN_WATER_LOW) {
    expectedPumpState = SPN_MULTIPUMP_OFF;
  }
}
