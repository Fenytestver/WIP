#include "pump.h"

Pump::Pump(SystemTime* _systemTime, RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor)
{
  //ctor
  systemTime = _systemTime;
  rpmSensor = _rpmSensor;
  voltageSensor = _voltageSensor;
}

Pump::~Pump()
{
  //dtor
}
void Pump::turnOn()
{
  turnedOnAt = systemTime->nowMillis();
}

void Pump::turnOff()
{
  turnedOnAt = 0;
}

bool Pump::isTurnedOn()
{
  return turnedOnAt != 0;
}

int Pump::getRpm()
{
  return rpmSensor->getRpm();
}
bool Pump::isVoltageDetected()
{
  return voltageSensor->getVoltage() > SPS_PUMP_LOW_VOLTAGE_THREASHOLD;
}

long Pump::getPumpUptime()
{
  if (!isTurnedOn()) {
    return 0;
  } else {
    return systemTime->nowMillis() - turnedOnAt;
  }
}
