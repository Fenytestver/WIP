#include "sumppitsensor.h"

SumpPitSensor::SumpPitSensor(WaterLevelSensor* _waterLevelSensors, int _numWaterSensors,
                  LeakSensor* _leakSensors, int _numLeakSensors,
                  Pump* _pump)
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
bool SumpPitSensor::isWaterLevelHigh()
{
  // FIXME: multiple sensors.
  return waterLevelSensors->measureLevel() > SPS_WATER_THREASHOLD;
}
bool SumpPitSensor::isLeaking()
{
  // FIXME: multiple sensors.
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
  int flags = SPN_ALARM_NO_ALARM;
  int uptime = pump->getUptime();
  if (uptime > SPS_PUMP_SPINUP_TIME) {
    int rpm = pump->getRpm();
    int deviation = abs(rpm - SPN_PUMP_STD_RPM);
    // rpm deviation check
    sb(flags, SPN_ALARM_PUMP_RPM_TECHNICAL,
       deviation > SPN_PUMP_RPM_DEVI_TECHNICAL);
    sb(flags, SPN_ALARM_PUMP_RPM_CRITICAL,
       deviation > SPN_PUMP_RPM_DEVI_CRITICAL);
    // uptime check
    sb(flags, SPN_PUMP_CYCLE_MAX_LENGTH_TECHNICAL,
       uptime > SPN_ALARM_PUMP_CYCLE_TECHNICAL);
    sb(flags, SPN_PUMP_CYCLE_MAX_LENGTH_CRITICAL,
       uptime > SPN_ALARM_PUMP_CYCLE_CRITICAL);
    // pump voltage check
    sb(flags, SPN_ALARM_PUMP_VOLTAGE_CRITICAL,
                !pump->isVoltageDetected());
  }
  return flags;
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
  int waterLevel = waterLevelSensors->measureLevel();
  if (waterLevel >= SPN_WATER_CRITICAL) {
    // TODO: simple/turbo mode
    pump->turnOn();
  } else if (waterLevel >= SPN_WATER_HIGH) {
    // TODO: simple/turbo mode
    pump->turnOn();
  } else if (waterLevel <= SPN_WATER_LOW) {
    pump->turnOff();
  }
}
