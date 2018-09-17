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

int SumpPitSensor::getPumpState()
{
  int result = SPS_PUMP_NO_ERROR;
  // TODO: configuration.
  if (pump->getPumpUptime() > SPS_PUMP_SPINUP_TIME) {
    // check rpm of the motor.
    if (pump->getRpm() < SPS_PUMP_LOW_RPM_THRESHOLD) {
      // rpm low
      result |= SPS_PUMP_LOW_RPM;
    } else {
      result &= ~SPS_PUMP_LOW_RPM;
    }

    if (!pump->isVoltageDetected()) {
      // low voltage
      result |= SPS_PUMP_LOW_VOLTAGE;
    } else {
      result &= ~SPS_PUMP_LOW_VOLTAGE;
    }
  }
  return result;
}
