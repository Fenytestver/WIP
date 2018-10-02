#include "sumppitnode.h"

SumpPitNode::SumpPitNode(Siren* _siren,
    Display* _display,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs,
    ShutoffValve* _shutoffValve)
{
  //ctor
  siren = _siren;
  display = _display;
  sensor = _sensor;
  inputs = _inputs;
  shutoffValve = _shutoffValve;

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
  alarmReason = sensor->checkState();

  sensor->updatePump();

  // check system status
  if (isCritical(alarmReason)) {
    siren->on();
    // take care of the critical water level
    if ((alarmReason & SPN_WATER_CRITICAL) != 0) {
      shutoffValve->activate();
    } else {
      shutoffValve->deactivate();
    }
  } else if (isTechnical(alarmReason)) {
    // TODO: do we need this?
    siren->on();
  } else {
    siren->off();
  }

}

void SumpPitNode::alarmOff() {
  siren->off();
}

int SumpPitNode::getAlarmReason()
{
  return alarmReason;
}

bool SumpPitNode::isCritical(int reason)
{
  return (reason & SPN_ALERT_ALL_CRITICAL) != SPN_ALERT_NO_ALERT;
}

bool SumpPitNode::isTechnical(int reason)
{
  return (reason & SPN_ALERT_ALL_TECHNICAL) != SPN_ALERT_NO_ALERT;
}
