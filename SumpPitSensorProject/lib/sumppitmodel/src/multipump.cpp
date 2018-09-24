#include "multipump.h"

MultiPump::MultiPump()
  : Pump(0, 0, 0)
{
  //ctor
  mode = SPN_MULTIPUMP_OFF;
  pumps = new Pump*[SPN_PUMP_COUNT];
}

MultiPump::~MultiPump()
{
  //dtor
  delete pumps;
}

void MultiPump::turnOn()
{
  if (mode != SPN_MULTIPUMP_ON) {
    setMode(SPN_MULTIPUMP_ON);
  }
}

void MultiPump::turnOff()
{
  if (mode != SPN_MULTIPUMP_OFF) {
    setMode(SPN_MULTIPUMP_OFF);
  }
}

bool MultiPump::isTurnedOn()
{
  return mode != SPN_MULTIPUMP_OFF;
}

bool MultiPump::isVoltageDetected()
{
  if (mode != SPN_MULTIPUMP_TURBO) {
    return pumps[currentPump]->isVoltageDetected();
  } else {
    for (short i=0; i<pumpCount; ++i) {
      if (!pumps[currentPump]->isVoltageDetected()) {
        // fail fast if a pump does not work
        return false;
      }
    }
    // all pump had voltage
    return true;
  }
}

int MultiPump::getRpm()
{
  return pumps[currentPump]->getRpm();
}

long MultiPump::getUptime()
{
  return pumps[currentPump]->getUptime();
}

void MultiPump::turbo()
{
  if (mode != SPN_MULTIPUMP_TURBO) {
    setMode(SPN_MULTIPUMP_TURBO);
  }
}

void MultiPump::setMode(short _mode)
{
  mode = _mode;
  if (pumpCount == 0) {
    // should not happen.
    return;
  }
  switch (mode) {
  case SPN_MULTIPUMP_OFF:
    for (short i=0; i<pumpCount; ++i) {
      pumps[i]->turnOff();
    }
    // select next pump, loop at pumpCount
    currentPump = (currentPump + 1) % pumpCount;
    break;
  case SPN_MULTIPUMP_ON:
    // turn off all, but the current
    for (short i=0; i<pumpCount; ++i) {
      if (i != currentPump) {
        pumps[i]->turnOff();
      } else {
        // make sure current is on
        pumps[i]->turnOn();
      }
    }
  break;
  case SPN_MULTIPUMP_TURBO:
    // turn off all
    for (short i=0; i<pumpCount; ++i) {
      pumps[i]->turnOn();
    }
  break;
  }
}

void MultiPump::addPump(Pump* pump)
{
  if (pumpCount < SPN_MULTIPUMP_MAX_PUMPS) {
    pumps[pumpCount++] = pump;
  }
}
