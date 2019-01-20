#include "localview.h"
#include "spn_config.h"

LocalView::LocalView(Display* _display, SystemTime* _systemTime, Led* _ledRed, Led* _ledGreen, Led* _ledYellow)
{
  //ctor
  display = _display;
  systemTime = _systemTime;
  ledRed = _ledRed;
  ledGreen = _ledGreen;
  ledYellow = _ledYellow;
  pump1Rpm = new char[10];
  pump2Rpm = new char[10];
  statusString = new char[SPN_STATUS_BITS + 1];
  for (int i = 0; i < SPN_LOCALVIEW_LINES_COUNT; ++i) {
    lines[i] = new char[SPN_LOCALVIEW_LINES_LENGTH];
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
  ledRed->setup();
  ledYellow->setup();
  ledGreen->setup();
}

void LocalView::render(State state)
{
  ledRed->setState(state.mode == Mode::SPN_DISARMED);
  ledGreen->setState(state.mode == Mode::SPN_ARMED);
  ledYellow->setState(state.mode == Mode::SPN_MAINTENANCE);
  int len;
  switch (state.mode) {
    case Mode::SPN_MAINTENANCE:
    case Mode::SPN_ARMED:
      renderArmed(state);
      break;
    case Mode::SPN_DISARMED:
      len = sprintf(message, SPN_DISPLAY_OFF);
      message[len] = '\0';
      display->displayMessage(message);
      break;
    case Mode::SPN_INITIALIZING:
      len = sprintf(message, SPN_DISPLAY_WELCOME);
      message[len] = '\0';
      display->displayMessage(message);
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
  // pump overtime
  if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
    sprintf(lines[lineIndex++], "Pump1 cycle is %d%%+",
    toPercentExtra(state.pump1Uptime, SPN_PUMP_CYCLE_LENGTH_NORMAL));
  } else if ((state.pump1Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0) {
    sprintf(lines[lineIndex++], "Pump1 CYCLE is %d%%+",
    toPercentExtra(state.pump1Uptime, SPN_PUMP_CYCLE_LENGTH_NORMAL));
  }
  if ((state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_CRITICAL) != 0) {
    sprintf(lines[lineIndex++], "Pump2 cycle is %d%%+",
    toPercentExtra(state.pump2Uptime, SPN_PUMP_CYCLE_LENGTH_NORMAL));
  } else if ((state.pump2Alarm & SPN_ALARM_PUMP_CYCLE_TECHNICAL) != 0) {
    sprintf(lines[lineIndex++], "Pump2 cycle is %d%%+",
    toPercentExtra(state.pump2Uptime, SPN_PUMP_CYCLE_LENGTH_NORMAL));
  }

  // overwhelmed
  if ((state.alarmReason & SPN_ALARM_PUMP_OVERWHELMED_CRITICAL) != 0) {
    sprintf(lines[lineIndex++], "P1&2 overwhelmed");
  } else if ((state.alarmReason & SPN_ALARM_PUMP_OVERWHELMED_TECHNICAL) != 0) {
    sprintf(lines[lineIndex++], "%s is overwhelmed", state.pump1On ? "P1" : "P2");
  }

  if (isCritical(state.alarmReason)) {
    // pump rpm
    if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "P1,2 rpm: %d%%,%d%%",
          toPercent(state.pump1Rpm, SPN_PUMP_STD_RPM),
          toPercent(state.pump2Rpm, SPN_PUMP_STD_RPM));
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1 rpm: %d%%", toPercent(state.pump1Rpm, SPN_PUMP_STD_RPM));
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_RPM_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump2 rpm: %d%%", toPercent(state.pump2Rpm, SPN_PUMP_STD_RPM));
    }

    if ((state.alarmReason & SPN_ALARM_LEAK) != 0) {
      sprintf(lines[lineIndex++], "Leak detected!");
    }
    if ((state.alarmReason & SPN_ALARM_WATER_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "Water alert!%d\"%d%%", state.levelIn, state.levelPercent);
    }

    if ((state.alarmReason & SPN_ALARM_FLOAT_SWITCH_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], SPN_DISPLAY_FLOAT_SWITH);
    }

    if ((state.alarmReason & SPN_ALARM_PUMP_FAILED_CRITICAL) != 0) {
      sprintf(lines[lineIndex++], "P1&2 failed turn on");
    }
    sprintf(lines[lineIndex++], "!!CALL MANAGER NOW!!");
  }
  if (isTechnical(state.alarmReason)) {
    // pump rpm
    if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0
        && (state.pump2Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "P1,2 rpm: %d%%,%d%%",
          toPercent(state.pump1Rpm, SPN_PUMP_STD_RPM),
          toPercent(state.pump2Rpm, SPN_PUMP_STD_RPM));
    } else if ((state.pump1Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump1 rpm: %d%%", toPercent(state.pump1Rpm, SPN_PUMP_STD_RPM));
    } else if ((state.pump2Alarm & SPN_ALARM_PUMP_RPM_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump2 rpm: %d%%", toPercent(state.pump2Rpm, SPN_PUMP_STD_RPM));
    }

    // too low water
    if ((state.alarmReason & SPN_ALARM_WATER_LOW) != 0) {
      if (state.pump1On && state.pump2On) {
        sprintf(lines[lineIndex++], "P1&2 fail turn off");
      } else {
        sprintf(lines[lineIndex++], "%s fail turn off", state.pump1On ? "P1" : "P2");
      }
    }

    if ((state.alarmReason & SPN_ALARM_PUMP_FAILED_TECHNICAL) != 0) {
      sprintf(lines[lineIndex++], "Pump failed turn on");
    }
    if (!isCritical(state.alarmReason)) {
      sprintf(lines[lineIndex++], "NOTIFY MAINTENANCE!");
    }
  }
  if (isTechnical(state.alarmReason) || isCritical(state.alarmReason)) {
    int startLine;
    if (lineIndex > 3) {
      startLine = (systemTime->nowMillis() / 3000) % lineIndex;
    } else {
      startLine = 0;
      for (int i=lineIndex; i<SPN_LOCALVIEW_LINES_COUNT; ++i) {
        lines[i][0] = '\0';
      }
    }
    if (lineIndex < 3) {
      lineIndex = 3;
    }
    len = sprintf(message, SPN_DISPLAY_WARNING,
        isCritical(state.alarmReason) ? "ALRT" : "WARN", statusString,
        lines[startLine], lines[(startLine + 1) % lineIndex], lines[(startLine + 2) % lineIndex]);
  } else {
    len = sprintf(message, SPN_DISPLAY_NORMAL,
        (state.mode == SPN_ARMED) ?
            SPN_DISPLAY_NORMAL_MODE_TEXT : SPN_DISPLAY_MAINTENANCE_MODE_TEXT,
        levelInches, levelPercent,
                pump1Status, pump1Rpm, pump2Status, pump2Rpm);
  }
  message[len] = '\0';
  display->displayMessage(message);
}
