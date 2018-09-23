#ifndef SUMPPITSENSOR_H
#define SUMPPITSENSOR_H
#include "waterlevelsensor.h"
#include "multipump.h"
#include "leaksensor.h"
#include "spn_config.h"

using namespace spn;

class SumpPitSensor
{
  public:
    /** Default constructor */
    SumpPitSensor(WaterLevelSensor* _waterLevelSensors, int _numWaterSensors,
                  LeakSensor* _leakSensors, int _numLeakSensors,
                  MultiPump* _pump);
    /** Default destructor */
    virtual ~SumpPitSensor();

    /** Call at setup phase, once */
    virtual void setup();
    virtual bool isWaterLevelHigh();
    virtual bool isLeaking();
    virtual int checkState();
    virtual void updatePump();
  protected:
    /** Will return SPS_PUMP state bit-mask, where 0 is no error */
    virtual int checkPumpState();
    virtual int checkWaterLevelState();
    virtual int checkLeakState();

  protected:
    WaterLevelSensor* waterLevelSensors;
    short numWaterSensors;
    LeakSensor* leakSensors;
    int numLeakSensors;
    MultiPump* pump;

  private:
    // FIXME: 70? Hard coded!
    const int SPS_WATER_THREASHOLD = 70;
};

#endif // SUMPPITSENSOR_H
