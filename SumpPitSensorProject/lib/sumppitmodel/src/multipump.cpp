#include "multipump.h"

MultiPump::MultiPump()
{
  //ctor
  pumps = new Pump*[SPN_PUMP_COUNT];
  pumpCount = 0;
}

MultiPump::~MultiPump()
{
  //dtor
  delete pumps;
}

void MultiPump::addPump(Pump* pump)
{
  if (pumpCount < SPN_MULTIPUMP_MAX_PUMPS) {
    pumps[pumpCount++] = pump;
  }
}

int MultiPump::update()
{
  for (int i=0; i<pumpCount; ++i) {
    pumps[i]->update();
  }
  return 0;
}

int MultiPump::checkState()
{
  int state = SPN_ALARM_NO_ALARM;
  for (int i=0; i<pumpCount; ++i) {
    state |= pumps[i]->checkState();
  }
  return state;
}
