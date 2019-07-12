#ifdef DEBUG
//#include <bitset>
//#include <iostream>
#endif // DEBUG

#ifndef PUMP_H
#define PUMP_H
#include "rpmsensor.h"
#include "voltagesensor.h"
#include "systemtime.h"
#include "spn_config.h"

using namespace spn;

class Pump
{
  public:
    /** Default constructor */
    Pump(SystemTime* _systemTime, RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor);
    /** Default destructor */
    virtual ~Pump();
    virtual void setup();
    /** gives the on/off state of the pump based on the voltage. */
    virtual bool isTurnedOn();
    /** @return returns the RPM of the pump. */
    virtual int getRpm();
    virtual long getUptime();
    virtual int checkState();

    /** Updates the internal state. */
    virtual void update();
  protected:
    SystemTime* systemTime;
    RpmSensor* rpmSensor; //!< Member variable "rpmSensor"
    VoltageSensor* voltageSensor; //!< Member variable "voltageSensor"
    virtual void setTurnedOn();
    virtual void setTurnedOff();
  private:
    bool turnedOn;
    unsigned long turnedOnAt;
    int lastState;
};

#endif // PUMP_H
