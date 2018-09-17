#include "sumppitnode.h"

SumpPitNode::SumpPitNode(Siren* _siren,
    Display* _display,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs)
{
  //ctor
  siren = _siren;
  display = _display;
  sensor = _sensor;
  inputs = _inputs;

  mode = SPN_INITIALIZING;
}

SumpPitNode::~SumpPitNode()
{
  //dtor
}
int SumpPitNode::getMode()
{
  return mode;
}

void SumpPitNode::arm()
{
  mode = SPN_ARMED;
}

void SumpPitNode::disarm()
{
  mode = SPN_DISARMED;
  alarmOff();
}

void SumpPitNode::maintenance()
{
  mode = SPN_MAINTENANCE;
}

void SumpPitNode::alarm()
{
  if (mode == SPN_ARMED) {
    siren->on();
  }
}

void SumpPitNode::setup()
{
  sensor->setup();
  alarmOff();
  arm();
}

void SumpPitNode::update()
{
  switch (getMode()) {
  case SPN_ARMED:
    updateArmed();
    break;
  }
}

void SumpPitNode::updateArmed() {
  int _alarmReason = 0;
  if (sensor->isWaterLevelHigh()) {
    _alarmReason |= SPN_ALARM_HIGH_WATER;
  } else {
    _alarmReason &= ~SPN_ALARM_HIGH_WATER;
  }

  if (sensor->isLeaking()) {
    _alarmReason |= SPN_ALARM_LEAK;
  } else {
    _alarmReason &= ~SPN_ALARM_LEAK;
  }
  alarmReason = _alarmReason;
  if (alarmReason != 0) {
    alarm();
  } else {
    alarmOff();
  }
}

void SumpPitNode::alarmOff() {
  siren->off();
}

int SumpPitNode::getAlarmReason()
{
  return alarmReason;
}
