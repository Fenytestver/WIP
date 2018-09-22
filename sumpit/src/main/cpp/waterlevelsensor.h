#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#define SPN_WATER_LEVEL_UNKNOWN 0
class WaterLevelSensor
{
  public:
    /** Default constructor */
    WaterLevelSensor();
    /** Default destructor */
    virtual ~WaterLevelSensor();
    /** @return the water level in percentage. */
    virtual short measureLevel();

  protected:

  private:
};

#endif // WATERLEVELSENSOR_H
