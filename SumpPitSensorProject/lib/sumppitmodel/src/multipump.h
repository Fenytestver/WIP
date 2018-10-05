#ifndef MULTIPUMP_H
#define MULTIPUMP_H

#include "pump.h"

#define SPN_MULTIPUMP_MAX_PUMPS 2
#define SPN_MULTIPUMP_OFF 0
#define SPN_MULTIPUMP_ON 1
#define SPN_MULTIPUMP_TURBO 2

class MultiPump
{
  public:
    MultiPump();
    virtual ~MultiPump();
    virtual void addPump(Pump* pump);
    int update();
    int checkState();
  protected:

  private:
    Pump** pumps;
    short pumpCount;
};

#endif // MULTIPUMP_H
