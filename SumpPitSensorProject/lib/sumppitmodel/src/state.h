#ifndef STATE_H
#define STATE_H
struct State {
  int alarmReason;
  int mode;
  int pump1Rpm;
  int pump2Rpm;
  int pump1Alarm;
  int pump2Alarm;
  int levelIn;
  int levelPercent;
};
#endif
