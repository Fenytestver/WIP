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

void Pump::turnOn()
{
  if (!turnedOn) {
    turnedOn = true;
    turnedOnAt = systemTime->nowMillis();
  }
}

void Pump::turnOff()
{
  if (turnedOn) {
    turnedOn = false;
    turnedOnAt = 0L;
  }
}

bool Pump::isTurnedOn()
{
  return turnedOn;
}

int Pump::getRpm()
{
  return rpmSensor->getRpm();
}
bool Pump::isVoltageDetected()
{
  return voltageSensor->getVoltage() > SPN_PUMP_LOW_VOLTAGE_THRESHOLD;
}

long Pump::getUptime()
{
  if (!isTurnedOn()) {
    return 0L;
  } else {
    return systemTime->nowMillis() - turnedOnAt;
  }
}
