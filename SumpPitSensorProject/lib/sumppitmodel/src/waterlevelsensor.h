#ifdef DEBUG
//#include <iostream>
#endif // DEBUG

#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#define SPN_WATER_LEVEL_UNKNOWN 0

#include "spn_config.h"
class WaterLevelSensor
{
  public:
    /** Default constructor */
    WaterLevelSensor();
    /** Default destructor */
    virtual ~WaterLevelSensor();
    /** @return the water level in percentage. */
    virtual short measureLevel();
    virtual int checkState();

  protected:

  private:
    int lastAlarm;
};

#endif // WATERLEVELSENSOR_H
