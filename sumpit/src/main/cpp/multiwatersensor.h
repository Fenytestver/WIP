#ifndef MULTIWATERSENSOR_H
#define MULTIWATERSENSOR_H

#include "waterlevelsensor.h"
#include "spn_config.h"

#define SPN_MULTWATER_MAX_SENSORS 2

class MultiWaterSensor : public WaterLevelSensor
{
  public:
    MultiWaterSensor();
    virtual ~MultiWaterSensor();
    virtual void addSensor(WaterLevelSensor* _sensor);
    virtual short measureLevel();

  protected:
    WaterLevelSensor* sensors[SPN_MULTWATER_MAX_SENSORS];
    int numSensors;

  private:
};

#endif // MULTIWATERSENSOR_H
