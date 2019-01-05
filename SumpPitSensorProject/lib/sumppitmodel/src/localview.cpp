#include "localview.h"
#include "spn_config.h"

LocalView::LocalView(Display* _display)
{
  //ctor
  display = _display;
  pump1Rpm = new char[10];
  pump2Rpm = new char[10];
  statusString = new char[10];
  for (int i = 0; i < 4; ++i) {
    lines[i] = new char[100];
  }
  message = new char[1024];
}

LocalView::~LocalView()
{
  //dtor
  delete pump1Rpm;
  delete pump2Rpm;
  delete statusString;
  for (int i = 0; i < 4; ++i) {
    delete lines[i];
  }
  delete message;
}

void LocalView::setup()
{
  display->setup();
}

void LocalView::render(State state)
{
  switch (state.mode) {
    case Mode::SPN_ARMED:
      renderArmed(state);
      break;
    case Mode::SPN_DISARMED:
      //display->displayMessage(SPN_DISPLAY_OFF);
      break;
    case Mode::SPN_INITIALIZING:
      //display->displayMessage(SPN_DISPLAY_WELCOME);
      break;
    case Mode::SPN_MAINTENANCE:
      //display->displayMessage(SPN_DISPLAY_MAINTENANCE);
      break;
  }
}

void LocalView::renderArmed(State state)
{

  int levelInches = state.levelIn;
  int levelPercent = state.levelPercent;
  const char* pump1Status = state.pump1On == 0 ? "OFF" : " ON";
  const char* pump2Status = state.pump2On == 0 ? "OFF" : " ON";

  sprintf(pump1Rpm, "%d", state.pump1Rpm);
  sprintf(pump2Rpm, "%d", state.pump2Rpm);
  statusToString(state.alarmReason, statusString);
  lines[0][0] = '\0';
  lines[1][0] = '\0';
  lines[2][0] = '\0';
  // FIXME: magic number
  // must be a c-style string.

  int len=0;

  int lineIndex = 0;
  if (isCritical(state.alarmReason)) {
    // pump overtime
    if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1,2 OVT! %ds,%ds", millisToSec(state.pump1Uptime), millisToSec(state.pump2Uptime));
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1 OVT! %ds", millisToSec(state.pump1Uptime));
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump2 OVT! %ds", millisToSec(state.pump2Uptime));
    }
    // pump rpm
    if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1,2 rpm!! %d,%d", state.pump1Rpm, state.pump2Rpm);
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1 rpm!! %d", state.pump1Rpm);
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump2 rpm!! %d", state.pump2Rpm);
    }

    if ((state.alarmReason & SPN_ALARM_LEAK) != 0) {
      sprintf(lines[lineIndex++], "Leak detected!\n");
    }
    if ((state.alarmReason & SPN_ALARM_WATER_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Water alert!%d\"%d%%", state.levelIn, state.levelPercent);
    }
    if ((state.alarmReason & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump overuse! P1:%s,P2:%s",
              isCritical(state.pump1Alarm) ? "CR" : "OK",
              isCritical(state.pump2Alarm) ? "CR" : "OK");
    }
    if ((state.alarmReason & SPN_ALARM_PUMP_VOLTAGE_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump voltage!\nP1:%s,P2:%s",
              isCritical(state.pump1Alarm) ? "CR" : "OK",
              isCritical(state.pump2Alarm) ? "CR" : "OK");
    }

    len = sprintf(message, SPN_DISPLAY_WARNING, "ALERT!", statusString, lines[0], lines[1], lines[2]);

  } else if (isTechnical(state.alarmReason)) {
    // pump overtime
    if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "P1,2 ovt! %ds,%ds", millisToSec(state.pump1Uptime), millisToSec(state.pump2Uptime));
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "P1 ovt! %ds", millisToSec(state.pump1Uptime));
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "P2 ovt! %ds", millisToSec(state.pump2Uptime));
    }
    // pump rpm
    if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1,2 rpm warn!! %d,%d", state.pump1Rpm, state.pump2Rpm);
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1 rpm!! %d", state.pump1Rpm);
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump2 rpm!! %d", state.pump2Rpm);
    }

    // too low water
    if ((state.alarmReason & SPN_ALARM_WATER_LOW) != 0) {
      sprintf(lines[lineIndex++], "Water too low: %d\"", state.levelIn);
    }

    len = sprintf(message, SPN_DISPLAY_WARNING, "warn!", statusString, lines[0], lines[1], lines[2]);
  } else {
    len = sprintf(message, SPN_DISPLAY_NORMAL, SPN_DISPLAY_NORMAL_MODE_TEXT, levelInches, levelPercent,
                pump1Status, pump1Rpm, pump2Status, pump2Rpm);
  }
  message[len] = '\0';
  display->displayMessage(message);
}
