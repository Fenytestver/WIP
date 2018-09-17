#ifndef PUMP_H
#define PUMP_H
#include "rpmsensor.h"
#include "voltagesensor.h"
#include "systemtime.h"

#define SPS_PUMP_LOW_VOLTAGE_THREASHOLD 10.0

class Pump
{
  public:
    /** Default constructor */
    Pump(SystemTime* _systemTime, RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor);
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

    virtual long getPumpUptime();
  protected:
    SystemTime* systemTime;
    RpmSensor* rpmSensor; //!< Member variable "rpmSensor"
    VoltageSensor* voltageSensor; //!< Member variable "voltageSensor"

  private:
    unsigned long turnedOnAt;
};

#endif // PUMP_H
