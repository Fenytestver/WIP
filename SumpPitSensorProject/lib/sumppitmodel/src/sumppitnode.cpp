#include "sumppitnode.h"

SumpPitNode::SumpPitNode(
    SystemTime* _systemTime,
    Siren* _siren,
    Buzzer* _buzzer,
    LocalView* _localView,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs,
    ShutoffValve* _shutoffValve,
    FloatSwitch* _floatSwitch)
{
  //ctor
  systemTime = _systemTime;
  siren = _siren;
  buzzer = _buzzer;
  localView = _localView;
  sensor = _sensor;
  inputs = _inputs;
  shutoffValve = _shutoffValve;
  floatSwitch = _floatSwitch;
  snoozeAt = 0L;
  snoozeDuration = 0L;
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
  siren->setup();
  buzzer->setup();
  inputs->setup();
  floatSwitch->setup();
  alarmOff();
  arm();
}

State* SumpPitNode::update()
{
  state.alarmReason = SPN_ALARM_NO_ALARM;
  inputs->update();
  siren->update();
  switch (getMode()) {
  case SPN_MAINTENANCE:
  case SPN_ARMED:
    updateArmed();
    break;
  default:
    showState(state);
    break;
  }
  return &state;
}

void SumpPitNode::updateArmed() {
  sensor->updatePump(&state);
  state.floatSwitch = floatSwitch->isTriggered();
  state.alarmReason |= sensor->checkState(&state);
  if (state.floatSwitch) {
    state.alarmReason |= SPN_ALARM_FLOAT_SWITCH_CRITICAL;
  }

  // check system status
  if (isCritical(state.alarmReason)) {
    if (state.mode != SPN_MAINTENANCE) {
      if (!isSnoozed()) {
        siren->on();
      } else {
        siren->off();
      }
      // take care of the critical water level
      if ((state.alarmReason & SPN_ALARM_WATER_CRITICAL) != 0) {
        shutoffValve->activate();
      } else {
        shutoffValve->deactivate();
      }
    }
  } else if (isTechnical(state.alarmReason)) {
    // TODO: do we need this?
    if (state.mode != SPN_MAINTENANCE) {
      if (!isSnoozed()) {
        siren->on();
      } else {
        siren->off();
      }
    }
  } else {
    siren->off();
  }
  state.shutoffValve = shutoffValve->isActive();
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

int SumpPitNode::snooze(long duration) {
  snoozeAt = systemTime->nowMillis();
  snoozeDuration = duration;
  return snoozeRemaining();
}

long SumpPitNode::snoozeRemaining() {
  return snoozeAt + snoozeDuration - systemTime->nowMillis();
}

bool SumpPitNode::isSnoozed() {
  return snoozeRemaining() > 0L;
}
