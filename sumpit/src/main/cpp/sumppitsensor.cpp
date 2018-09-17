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

