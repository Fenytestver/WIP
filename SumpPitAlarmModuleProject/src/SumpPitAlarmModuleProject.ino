#include "spn_config.h"
#include "button.h"
#include "systemtime.h"
#include "buzzer.h"
#include "impl.ino"
//#include <ArduinoJson.h>

#include "RdJson.h"

/*
 * Project SumpPitAlarmModuleProject
 * Description: The Alarm Module.
 * Author:
 * Date:
 */
#define SPN_SNOOZE_TIME (5 * 60000)
#define BUTTON_LONG_PRESS_TIME 3000
#define BEEP_TIME 50
#define SYSTEM_STATUS_TOPIC "spnStatus"
#define SHUTOFF_VALVE_TOPIC "shutoffValve"
#define SPN_MODE_UNKNOWN -1

#define PIN_BUTTON_SNOOZE A4
#define PIN_BUZZER D3
#define PIN_LED_GREEN D5
#define PIN_LED_YELLOW D4
#define PIN_LED_RED D6
#define PIN_LED_RED2 A1
#define PIN_LED_RED3 A2
#define PIN_SIREN D2

SystemTime* systemTime;
RealButton* snoozeButton;
FunctionPressListener* snoozePressListener;
RealBuzzer* buzzer;
HardwarePinLed* ledGreen;
HardwarePinLed* ledYellow;
HardwarePinLed* ledRed;
HardwarePinLed* ledRed2;
HardwarePinLed* ledRed3;
RealSiren* siren;

bool technical = false;
bool critical = false;
int mode = SPN_MODE_UNKNOWN;
long alarmReason = SPN_ALARM_NO_ALARM;
char* dataCpy = new char[1024];
bool stateUnknown = true;
unsigned long snoozeAt = 0L;
unsigned long lastCritical = 0L;
bool sirenOn = false;

class OnAnyPress : public OnButtonPressListener {
      public:
        OnAnyPress(Buzzer* _buzzer) {buzzer=_buzzer;}
        void onPress() {
          buzzer->beep();
        }
      private:
        Buzzer* buzzer;
    } *beepPressListener;

bool shutoffEnabled = false;
 //SerialDebugOutput debugOutput(115200);

void setup() {
  Serial.begin(115200);
  systemTime = new RealSystemTime();
  ledGreen = new HardwarePinLed(PIN_LED_GREEN);
  ledYellow = new HardwarePinLed(PIN_LED_YELLOW);
  ledRed = new HardwarePinLed(PIN_LED_RED);
  ledRed2 = new HardwarePinLed(PIN_LED_RED2);
  ledRed3 = new HardwarePinLed(PIN_LED_RED3);
  buzzer = new RealBuzzer(PIN_BUZZER, BEEP_TIME);
  siren = new RealSiren(systemTime, PIN_SIREN);
  snoozeButton = new RealButton(systemTime, BUTTON_LONG_PRESS_TIME, PIN_BUTTON_SNOOZE);

  snoozePressListener = new FunctionPressListener(snoozeNow);
  beepPressListener = new OnAnyPress(buzzer);
  snoozeButton->setOnButtonPressListener(beepPressListener);
  snoozeButton->setOnButtonLongPressListener(snoozePressListener);
  ledGreen->setup();
  ledRed->setup();
  ledYellow->setup();
  buzzer->setup();
  ledRed2->setup();
  ledRed3->setup();
  snoozeButton->setup();
  siren->setup();

  Particle.subscribe(SHUTOFF_VALVE_TOPIC, shutoffValveHandler);
  Particle.function("snooze", snoozeExtra);
  Particle.subscribe(SYSTEM_STATUS_TOPIC, statusHandler);
}

void loop() {
  unsigned long now = systemTime->nowMillis();
  long nowbit = now / 200;
  switch (mode) {
    case SPN_MODE_UNKNOWN:
      ledRed->setState((nowbit % 5) == 2);
      ledRed2->setState((nowbit % 5) == 3);
      ledRed3->setState((nowbit % 5) == 4);
      ledGreen->setState((nowbit % 5) == 0);
      ledYellow->setState((nowbit % 5) == 1);
      sirenOn = false;
      break;
    case SPN_ARMED: {
        if (critical && lastCritical == 0L) {
          lastCritical = now;
        } else if (critical && lastCritical == 0L) {
          lastCritical = 0L;
        }
        long criticalDuration = lastCritical > 0 ?
            now - lastCritical
            : 0L;
        if (isCriticalShutoff(alarmReason)) {
          renderCriticalLeds(criticalDuration, true /* shutoff */, nowbit);
        } else if (critical) {
          renderCriticalLeds(criticalDuration, false /* shutoff */, nowbit);
        } else {
          ledRed->setState(false);
          ledRed2->setState(false);
          ledRed3->setState(false);
          sirenOn = false;
        }
        ledGreen->setState(true);
        ledYellow->setState(technical && ((nowbit % 2) == 0));
      }
      break;
    case SPN_DISARMED: {
        ledRed->setState(true);
        ledRed2->setState(true);
        ledRed3->setState(true);
        ledGreen->setState(false);
        ledYellow->setState(true);
        sirenOn = false;
      }
      break;
    case SPN_MAINTENANCE:
      ledRed->setState(false);
      ledRed2->setState(false);
      ledRed3->setState(false);
      ledGreen->setState(false);
      ledYellow->setState(true);
      sirenOn = false;
      break;
  }

  if (sirenOn && (snoozeAt == 0 || snoozeAt + SPN_SNOOZE_TIME < now)) {
    siren->on();
  } else {
    siren->off();
  }
  siren->update();
  snoozeButton->update();
}

int snoozeExtra(String extra) {
  return snooze();
}

void snoozeNow() {
  snooze();
}

int snooze() {
  unsigned long now = systemTime->nowMillis();
  snoozeAt = now;
  return SPN_SNOOZE_TIME;
}

void shutoffValveHandler(const char *event, const char *data)
{
  Serial.print("shutoff changed to: ");
  Serial.println(data);
  if (strcmp("true", data) == 0) {
    shutoffEnabled = true;
  } else if (strcmp("false", data) == 0) {
    shutoffEnabled = false;
  }
}

void statusHandler(const char* event, const char* data) {

  Serial.print("event: ");
  Serial.println(event);
  Serial.print("update: ");
  Serial.print(data);
  Serial.println();
  strcpy(dataCpy, data);

  Serial.print("Mode ");
  bool isValid = false;
  jsmnrtype_t objType = JSMNR_UNDEFINED;
  int objSize = 0;
  String mStr = RdJson::getString("mode", "", isValid, objType, objSize, dataCpy);
  Serial.println(mStr);
  mode = stringToInt(mStr, SPN_MODE_UNKNOWN);

  String alarmStr =
      RdJson::getString("alarm", "", isValid, objType, objSize, dataCpy);
  alarmReason = binaryStringToInt(alarmStr, SPN_ALARM_NO_ALARM);
  Serial.print("alarmStr:");
  Serial.print(alarmStr);
  Serial.print("-");
  Serial.println(alarmReason);

  technical = RdJson::getString("technical", "", isValid, objType, objSize, dataCpy) == "1";
  critical = RdJson::getString("critical", "", isValid, objType, objSize, dataCpy) == "1";
  Serial.print("technical");
  Serial.println(technical);
  Serial.print("critical");
  Serial.println(critical);
}

long binaryStringToInt(String extra, long defaultInt) {
  // little endian
  long result = defaultInt;
  if (extra != nullptr) {
    char c = extra[0];
    int extraIndex = 0;
    int bitpos = 0;
    int extralen = extra.length();
    while (c != '\n' && extraIndex < extralen) {
      if (c == '0') {
        bitpos++;
      } else if (c == '1') {
        result |= (1<<bitpos);
        bitpos++;
      }
      c = extra[++extraIndex];
    }
  }
  return result;
}

int stringToInt(String extra, int defaultInt) {
  int result = defaultInt;
  if (extra.length() > 0) {
    int tmp = extra.toInt();
    if (tmp > 0) {
      result = tmp;
    }
  }
  return result;
}

bool isCriticalShutoff(long alarm) {
  return ((SPN_ALERT_ALL_CRITICAL_SHUTOFF & alarm) != 0);
}

void renderCriticalLeds(long duration, bool shutoff, long nowBits) {
  long sec = duration / 100;
  if (shutoff) {
    if (sec > 240) {
      ledRed->setState((nowBits & 2) == 0);
      ledRed2->setState((nowBits & 2) == 0);
      ledRed3->setState((nowBits & 2) == 0);
      sirenOn = true;
    } else if (sec > 120) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState((nowBits & 2) == 0);
      sirenOn = false;
    } else if (sec > 60) {
      ledRed->setState(true);
      ledRed2->setState((nowBits & 2) == 0);
      ledRed3->setState(false);
      sirenOn = false;
    } else {
      ledRed->setState((nowBits & 2) == 0);
      ledRed2->setState(false);
      ledRed3->setState(false);
      sirenOn = false;
    }
  } else {
    // alert A
    sec = sec % 360;
    sirenOn = false;
    if (sec > 300) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState(true);
    } else if (sec > 120) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState(false);
    } else {
      ledRed->setState(true);
      ledRed2->setState(false);
      ledRed3->setState(false);
    }
  }
}
