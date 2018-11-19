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

int MultiPump::update(State* state)
{
  state->pump1Rpm = getRpm(0);
  state->pump2Rpm = getRpm(1);
  state->pump1Alarm = checkState(0);
  state->pump2Alarm = checkState(1);

  for (int i=0; i<pumpCount; ++i) {
    pumps[i]->update();
  }
  return 0;
}

int MultiPump::getRpm(int index)
{
  if (index >= pumpCount) {
    return -1;
  }
  return pumps[index]->getRpm();
}


int MultiPump::checkState()
{
  int state = SPN_ALARM_NO_ALARM;
  for (int i=0; i<pumpCount; ++i) {
    state |= checkState(i);
  }
  return state;
}

int MultiPump::checkState(int index)
{
  if (index >= pumpCount) {
    return SPN_ALARM_NO_ALARM;
  }
  return pumps[index]->checkState();
}
