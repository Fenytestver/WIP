#ifndef SUMPPITSENSOR_H
#define SUMPPITSENSOR_H
#include "waterlevelsensor.h"
#include "pump.h"
#include "leaksensor.h"

#define SPS_PUMP_NO_ERROR (0)
#define SPS_PUMP_LOW_VOLTAGE (1<<0)
#define SPS_PUMP_LOW_RPM (1<<1)

// give a little time after a turn on to spin up the pump
#define SPS_PUMP_SPINUP_TIME 10000
// below this, rpm considered low
#define SPS_PUMP_LOW_RPM_THRESHOLD 400

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
    virtual bool isWaterLevelHigh();
    virtual bool isLeaking();
    /** Will return SPS_PUMP state bit-mask, where 0 is no error */
    virtual int getPumpState();

  protected:
    WaterLevelSensor* waterLevelSensors;
    short numWaterSensors;
    LeakSensor* leakSensors;
    int numLeakSensors;
    Pump* pump;

  private:
    // FIXME: 70? Hard coded!
    const int SPS_WATER_THREASHOLD = 70;
};

#endif // SUMPPITSENSOR_H
