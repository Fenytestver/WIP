#ifdef DEBUG
//#include <iostream>
#endif // DEBUG

#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#define SPN_WATER_LEVEL_UNKNOWN 0
#include "state.h"
#include "spn_config.h"
class WaterLevelSensor
{
  public:
    /** Default constructor */
    WaterLevelSensor(int _waterLowIn, int _waterHighIn, int _waterPercentHigh);
    /** Default destructor */
    virtual ~WaterLevelSensor();
    /** @return the water level in percentage. */
    virtual short measureLevel();
    virtual int checkState(State* state);
    virtual int getWaterPercentHigh();
  protected:

  private:
    int lastAlarm;
    int waterLowIn;
    int waterHighIn;
    int waterPercentHigh;
};

#endif // WATERLEVELSENSOR_H
