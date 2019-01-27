#ifndef FLOATSWITCH_H
#define FLOATSWITCH_H

#include "leaksensor.h"
#include "state.h"

class FloatSwitch {
  public:
    FloatSwitch();
    virtual bool isTriggered();
    virtual void setup();
};

#endif // FLOATSWITCH_H
