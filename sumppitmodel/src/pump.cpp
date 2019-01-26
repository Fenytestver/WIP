#include "pump.h"

Pump::Pump(SystemTime* _systemTime, RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor)
{
  //ctor
  systemTime = _systemTime;
  rpmSensor = _rpmSensor;
  voltageSensor = _voltageSensor;
  turnedOnAt = 0L;
  turnedOn = false;
  lastState = SPN_ALARM_NO_ALARM;
}

Pump::~Pump()
{
  //dtor
}

void Pump::setup() {
  rpmSensor->setup();
}

bool Pump::isTurnedOn()
{
  return turnedOn;
}
void Pump::setTurnedOn()
{
  if (!turnedOn) {
    turnedOn = true;
    turnedOnAt = systemTime->nowMillis();
  }

}

void Pump::setTurnedOff()
{
  if (turnedOn) {
    turnedOn = false;
    turnedOnAt = 0L;
  }
}

void Pump::update()
{
  // TODO: constant;
  if (voltageSensor->getVoltage() > 0.1f) {
    setTurnedOn();
  } else {
    setTurnedOff();
  }
}
int Pump::checkState()
{
  if (!isTurnedOn()) {
    lastState = SPN_ALARM_NO_ALARM;
    return SPN_ALARM_NO_ALARM;
  }

  int flags = SPN_ALARM_NO_ALARM;
  int uptime = getUptime();
  if (uptime >= SPN_PUMP_SPINUP_TIME) {
    int rpm = getRpm();
    int deviation = abso(rpm - SPN_PUMP_STD_RPM);
    // rpm deviation check
    sb(flags, SPN_ALARM_PUMP_RPM_TECHNICAL,
       deviation >= SPN_PUMP_RPM_DEVI_TECHNICAL);
    sb(flags, SPN_ALARM_PUMP_RPM_CRITICAL,
       deviation >= SPN_PUMP_RPM_DEVI_CRITICAL);
    // critical is the most important, turns on immediately
    if ((flags & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sb(lastState, SPN_ALARM_PUMP_RPM_CRITICAL, true);
    } else if ((flags & SPN_ALARM_PUMP_RPM_CRITICAL) == 0 &&
               // going down from critical
               (lastState & SPN_ALARM_PUMP_RPM_CRITICAL) != 0 &&
               // was critical, but now isn't. check if it's below the variance limit.
               deviation < SPN_PUMP_RPM_DEVI_CRITICAL - SPN_PUMP_RPM_DEVI_VARIANCE) {
      sb(lastState, SPN_ALARM_PUMP_RPM_CRITICAL, false);
    } else if ((flags & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0 &&
               // was no error before, let it happen
               (lastState & SPN_ALARM_PUMP_RPM_TECHNICAL) == 0) {
      sb(lastState, SPN_ALARM_PUMP_RPM_TECHNICAL, true);
    } else if ((flags & SPN_ALARM_PUMP_RPM_TECHNICAL) == 0 &&
               (lastState & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0 &&
               deviation < SPN_PUMP_RPM_DEVI_TECHNICAL - SPN_PUMP_RPM_DEVI_VARIANCE) {
      sb(lastState, SPN_ALARM_PUMP_RPM_TECHNICAL, false);
    }

    // uptime check, no need for variance
    sb(lastState, SPN_ALARM_PUMP_CYCLE_TECHNICAL,
       uptime > SPN_PUMP_CYCLE_MAX_LENGTH_TECHNICAL);
    sb(lastState, SPN_ALARM_PUMP_CYCLE_CRITICAL,
       uptime > SPN_PUMP_CYCLE_MAX_LENGTH_CRITICAL);
  } else {
    lastState = SPN_ALARM_NO_ALARM;
  }
  return lastState;
}

int Pump::getRpm()
{
  return rpmSensor->getRpm();
}

long Pump::getUptime()
{
  if (!isTurnedOn()) {
    return 0L;
  } else {
    return systemTime->nowMillis() - turnedOnAt;
  }
}
