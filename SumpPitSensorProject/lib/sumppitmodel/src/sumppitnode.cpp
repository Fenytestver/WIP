#include "sumppitnode.h"

SumpPitNode::SumpPitNode(Siren* _siren,
    Buzzer* _buzzer,
    LocalView* _localView,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs,
    ShutoffValve* _shutoffValve)
{
  //ctor
  siren = _siren;
  buzzer = _buzzer;
  localView = _localView;
  sensor = _sensor;
  inputs = _inputs;
  shutoffValve = _shutoffValve;

  state.mode = SPN_INITIALIZING;
  mainrenancePressListener = new OnMaintenancePress(this);
  disarmPressListener = new OnDisarmPress(this);
  armPressListener = new OnArmPress(this);
  beepPressListener = new OnAnyPress(buzzer);
  // set up button callbacks
  inputs->armResetButton->setOnButtonLongPressListener(armPressListener);
  inputs->disarmButton->setOnButtonLongPressListener(disarmPressListener);
  inputs->maintenanceButton->setOnButtonLongPressListener(mainrenancePressListener);
  inputs->armResetButton->setOnButtonPressListener(beepPressListener);
  inputs->disarmButton->setOnButtonPressListener(beepPressListener);
  inputs->maintenanceButton->setOnButtonPressListener(beepPressListener);
}

SumpPitNode::~SumpPitNode()
{
  //dtor
}
int SumpPitNode::getMode()
{
  return state.mode;
}

void SumpPitNode::arm()
{
  state.mode = SPN_ARMED;
}

void SumpPitNode::disarm()
{
  state.mode = SPN_DISARMED;
  alarmOff();
}

void SumpPitNode::maintenance()
{
  state.mode = SPN_MAINTENANCE;
}

void SumpPitNode::alarm()
{
  if (state.mode == SPN_ARMED) {
    siren->on();
  }
}

void SumpPitNode::setup()
{
  sensor->setup();
  localView->setup();
  alarmOff();
  arm();
}

State* SumpPitNode::update()
{
  switch (getMode()) {
  case SPN_ARMED:
    updateArmed();
    break;
  }
  return &state;
}

void SumpPitNode::updateArmed() {
  sensor->updatePump();
  state.alarmReason = sensor->checkState();

  // check system status
  if (isCritical(state.alarmReason)) {
    siren->on();
    // take care of the critical water level
    if ((state.alarmReason & SPN_WATER_CRITICAL) != 0) {
      shutoffValve->activate();
    } else {
      shutoffValve->deactivate();
    }
  } else if (isTechnical(state.alarmReason)) {
    // TODO: do we need this?
    siren->on();
  } else {
    siren->off();
  }
  showState(state);
}

void SumpPitNode::showState(State stateCopy)
{
  localView->render(stateCopy);
}

void SumpPitNode::alarmOff() {
  siren->off();
}

int SumpPitNode::getAlarmReason()
{
  return state.alarmReason;
}
