#include "pump.h"

Pump::Pump(RpmSensor* _rpmSensor, VoltageSensor* _voltageSensor)
{
  //ctor
  rpmSensor = _rpmSensor;
  voltageSensor = _voltageSensor;
}

Pump::~Pump()
{
  //dtor
}
void Pump::turnOn()
{

}

void Pump::turnOff()
{

}

bool Pump::isTurnedOn()
{
  return false;
}

int Pump::getRpm()
{
  return rpmSensor->getRpm();
}
bool Pump::isVoltageDetected()
{
  return voltageSensor->getVoltage();
}
