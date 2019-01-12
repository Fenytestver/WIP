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
  lastWaterLevel = 0;
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
  int leakState = checkLeakState();
  state |= checkPumpState();
  state |= checkWaterLevelState(outstate);
  state |= leakState;
  outstate->leak = (leakState != SPN_ALARM_NO_ALARM);

  int waterLevel = waterLevelSensors->measureLevel();
  if (lastWaterLevel == 0 && (pump->isTurnedOn(0) || pump->isTurnedOn(1))) {
    lastWaterLevel = waterLevel;
  } else if (lastWaterLevel != 0 && (!pump->isTurnedOn(0) && !pump->isTurnedOn(1))) {
    lastWaterLevel = 0;
  }

  if (lastWaterLevel != 0
      && waterLevel > lastWaterLevel + SPN_WATER_LEVEL_TOLERANCE_PERCENT) {
    if ((pump->isTurnedOn(0) && pump->getPumpUptime(0) > SPN_PUMP_SPINUP_TIME)
         && (pump->isTurnedOn(1) && pump->getPumpUptime(1) > SPN_PUMP_SPINUP_TIME)) {
      state |= SPN_ALARM_PUMP_OVERWHELMED_CRITICAL;
    } else if ((pump->isTurnedOn(0) && pump->getPumpUptime(0) > SPN_PUMP_SPINUP_TIME)
         || (pump->isTurnedOn(1) && pump->getPumpUptime(1) > SPN_PUMP_SPINUP_TIME)) {
      state |= SPN_ALARM_PUMP_OVERWHELMED_TECHNICAL;
    }
  }

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

  /*if (waterLevel >= SPN_WATER_CRITICAL) {
    expectedPumpState = SPN_MULTIPUMP_TURBO;
  } else if (waterLevel >= SPN_WATER_HIGH) {
    expectedPumpState = SPN_MULTIPUMP_ON;
  } else if (waterLevel <= SPN_WATER_LOW) {
    expectedPumpState = SPN_MULTIPUMP_OFF;
  }*/
}
