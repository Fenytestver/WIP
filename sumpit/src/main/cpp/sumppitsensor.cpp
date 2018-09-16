#include "sumppitsensor.h"

SumpPitSensor::SumpPitSensor(WaterLevelSensor* _waterLevelSensors, int _numWaterSensors,
                  LeakSensor* _leakSensors, int _numLeakSensors,
                  Pump* _pump)
{
  //ctor
  waterLevelSensors = _waterLevelSensors;
  numWaterSensors = _numWaterSensors;
  pump = _pump;
}

SumpPitSensor::~SumpPitSensor()
{
  //dtor
}

void SumpPitSensor::setup() {
}
