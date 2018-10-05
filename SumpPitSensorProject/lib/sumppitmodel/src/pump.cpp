#include "pump.h"

Pump::Pump(SystemTime* _systemTime, RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor)
{
  //ctor
  systemTime = _systemTime;
  rpmSensor = _rpmSensor;
  voltageSensor = _voltageSensor;
  turnedOnAt = 0L;
  turnedOn = false;
}

Pump::~Pump()
{
  //dtor
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
    return SPN_ALARM_NO_ALARM;
  }

  int flags = SPN_ALARM_NO_ALARM;
  int uptime = getUptime();
  if (uptime > SPS_PUMP_SPINUP_TIME) {
    int rpm = getRpm();
    int deviation = abso(rpm - SPN_PUMP_STD_RPM);
    // rpm deviation check
    sb(flags, SPN_ALARM_PUMP_RPM_TECHNICAL,
       deviation > SPN_PUMP_RPM_DEVI_TECHNICAL);
    sb(flags, SPN_ALARM_PUMP_RPM_CRITICAL,
       deviation > SPN_PUMP_RPM_DEVI_CRITICAL);
    // uptime check
    sb(flags, SPN_PUMP_CYCLE_MAX_LENGTH_TECHNICAL,
       uptime > SPN_ALARM_PUMP_CYCLE_TECHNICAL);
    sb(flags, SPN_PUMP_CYCLE_MAX_LENGTH_CRITICAL,
       uptime > SPN_ALARM_PUMP_CYCLE_CRITICAL);
  }
  return flags;
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
