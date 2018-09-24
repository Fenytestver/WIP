#ifndef MULTIPUMP_H
#define MULTIPUMP_H

#include "pump.h"

#define SPN_MULTIPUMP_MAX_PUMPS 2
#define SPN_MULTIPUMP_OFF 0
#define SPN_MULTIPUMP_ON 1
#define SPN_MULTIPUMP_TURBO 2

class MultiPump : public Pump
{
  public:
    MultiPump();
    virtual ~MultiPump();

    virtual void turnOn();
    virtual void turnOff();
    /** gives the on/off state of the pump, as we requested. */
    virtual bool isTurnedOn();
    /** gives the on/off state of the pump based on it's voltage. */
    virtual bool isVoltageDetected();
    /** @return returns the RPM of the pump. */
    virtual int getRpm();
    virtual long getUptime();
    virtual void turbo();
    virtual void addPump(Pump* pump);
  protected:

  private:
    Pump** pumps;
    short currentPump;
    short pumpCount;
    short mode;

    void setMode(short _mode);
};

#endif // MULTIPUMP_H
