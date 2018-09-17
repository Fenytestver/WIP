#ifndef PUMP_H
#define PUMP_H
#include "rpmsensor.h"
#include "voltagesensor.h"

class Pump
{
  public:
    /** Default constructor */
    Pump(RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor);
    /** Default destructor */
    virtual ~Pump();
    virtual void turnOn();
    virtual void turnOff();
    /** gives the on/off state of the pump, as we requested. */
    virtual bool isTurnedOn();
    /** gives the on/off state of the pump based on it's voltage. */
    virtual bool isVoltageDetected();
    /** @return returns the RPM of the pump. */
    virtual int getRpm();

  protected:
    RpmSensor* rpmSensor; //!< Member variable "rpmSensor"
    VoltageSensor* voltageSensor; //!< Member variable "voltageSensor"

  private:
};

#endif // PUMP_H
