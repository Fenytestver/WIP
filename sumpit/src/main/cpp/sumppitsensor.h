#ifndef SUMPPITSENSOR_H
#define SUMPPITSENSOR_H
#include "waterlevelsensor.h"
#include "pump.h"
#include "leaksensor.h"

class SumpPitSensor
{
  public:
    /** Default constructor */
    SumpPitSensor(WaterLevelSensor* _waterLevelSensors, int _numWaterSensors,
                  LeakSensor* _leakSensors, int _numLeakSensors,
                  Pump* _pump);
    /** Default destructor */
    virtual ~SumpPitSensor();

    /** Call at setup phase, once */
    virtual void setup();
  protected:
    WaterLevelSensor* waterLevelSensors;
    short numWaterSensors;
    LeakSensor* leakSensors;
    int numLeakSensors;
    Pump* pump;

  private:
};

#endif // SUMPPITSENSOR_H
