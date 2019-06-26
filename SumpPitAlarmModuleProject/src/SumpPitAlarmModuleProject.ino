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
#define SHUTOFF_ANOMALY_TOPIC "shutoffAnomaly"
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

typedef struct DeviceStatus {
  int id;
  bool technical;
  bool critical;
  int mode;
  long alarmReason;
};

#define MAX_DEVICE_COUNT 5
DeviceStatus statusArray[MAX_DEVICE_COUNT];
int numDevices = 0;

char* dataCpy = new char[1024];
unsigned long snoozeAt = 0L;
unsigned long lastCritical = 0L;
bool sirenOn = false;
bool shutoffAnomaly = false;
bool sendKeepAlive = false;
char message[256];
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

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
Timer keepAliveTimer(120000, sendKeepAlivePacket);
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

  Particle.subscribe(SHUTOFF_VALVE_TOPIC, shutoffValveHandler, MY_DEVICES);
  Particle.function("snooze", snoozeExtra);
  Particle.function("reboot", reboot);
  Particle.subscribe(SYSTEM_STATUS_TOPIC, statusHandler, MY_DEVICES);
  Particle.subscribe(SHUTOFF_ANOMALY_TOPIC, shutoffAnomalyHandler, MY_DEVICES);
  // request immediate update.
  Particle.publish("spnPing", "anyonethere", PRIVATE);
  keepAliveTimer.start();
}

void loop() {
  unsigned long now = systemTime->nowMillis();
  long nowbit = now / 200;
  bool technical = isTechnical();
  bool critical = isCritical();

  long alarmReason = getAlarmReason();
  bool snoozed = isSnoozed(now);

  int armed = 0;
  int disarmed = 0;
  int maintenance = 0;
  for (int i = 0; i < numDevices; ++i) {
    switch (statusArray[i].mode) {
      case SPN_ARMED:
        armed++;
        break;
      case SPN_DISARMED:
        disarmed++;
        break;
      case SPN_MAINTENANCE:
      default:
        maintenance++;
        break;
    }
  }
  if (shutoffAnomaly) {
    critical = true;
  }

  if (numDevices > 0) {
    ledGreen->setState(!technical && !critical && armed > 0 && maintenance == 0);
    ledYellow->setState(maintenance > 0
        || (armed == 0)
        || (technical && ((nowbit % 2) == 0)));

    if (!snoozed && critical && lastCritical == 0L) {
      lastCritical = now;
    }
    if (!critical || snoozed) {
      lastCritical = 0L;
    }
    unsigned long criticalDuration = (critical && lastCritical > 0L) ?
        now - lastCritical
        : 0L;
    if (maintenance == 0 && critical && isCriticalShutoff(alarmReason) && !snoozed) {
      renderCriticalLeds(criticalDuration, true /* shutoff */, nowbit);
    } else if (maintenance == 0 && critical && !snoozed) {
      renderCriticalLeds(criticalDuration, false /* shutoff */, nowbit);
    } else {
      ledRed->setState(false);
      ledRed2->setState(false);
      ledRed3->setState(false);
      sirenOn = false;
    }
  } else {
    ledRed->setState(false);
    ledRed2->setState(false);
    ledRed3->setState(false);
    ledGreen->setState((nowbit % 2) == 0);
    ledYellow->setState((nowbit % 2) == 1);
    sirenOn = false;
    lastCritical = 0L;
  }

  if (sirenOn && !snoozed) {
    siren->on();
  } else {
    siren->off();
  }
  siren->update();
  snoozeButton->update();
  if (sendKeepAlive) {
    sendKeepAlive = false;
    int seconds = systemTime->nowMillis() / 1000;
    sprintf(message, "{\"uptime\": \"%d\"}", seconds);
    Particle.publish("spnAlarm/status", message, PRIVATE);
  }
}

bool isSnoozed(long now) {
  return (snoozeAt != 0 && snoozeAt + SPN_SNOOZE_TIME > now);
}

int snoozeExtra(String extra) {
  return snooze();
}

void snoozeNow() {
  buzzer->beep();
  snooze();
}

int snooze() {
  unsigned long now = systemTime->nowMillis();
  snoozeAt = now;
  char snoozeText[10];
  sprintf(snoozeText, "%d", SPN_SNOOZE_TIME);
  Particle.publish("snoozeAlarm", snoozeText, PRIVATE);
  return SPN_SNOOZE_TIME;
}

void shutoffAnomalyHandler(const char *event, const char *data)
{
  Serial.print("shutoff anomaly changed to: ");
  Serial.println(data);
  if (strcmp("true", data) == 0) {
    shutoffAnomaly = true;
  } else if (strcmp("false", data) == 0) {
    shutoffAnomaly = false;
  }
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
  long sec = duration / 1000;
  if (shutoff) {
    if (sec > 240) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState(true);
      sirenOn = true;
    } else if (sec > 120) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState(true);
      sirenOn = false;
    } else if (sec > 60) {
      ledRed->setState(true);
      ledRed2->setState(true);
      ledRed3->setState(false);
      sirenOn = false;
    } else {
      ledRed->setState(true);
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

int reboot(String extra) {
  System.reset();
  return 1;
}

DeviceStatus* getStatusById(int id) {
  for (int i=0; i<numDevices; ++i) {
    if (statusArray[i].id = id) {
      return &statusArray[i];
    }
  }
  if (numDevices < MAX_DEVICE_COUNT) {
    DeviceStatus status;
    status.id = id;
    status.technical = false;
    status.critical = false;
    status.mode = SPN_MODE_UNKNOWN;
    status.alarmReason = SPN_ALARM_NO_ALARM;
    statusArray[numDevices] = status;
    numDevices++;
    return &statusArray[numDevices];
  }
  // FIXME: what if there's another device?
  return nullptr;
}
bool isTechnical() {
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].technical) {
      return true;
    }
  }
  return false;
}
bool isCritical() {
  for (int i = 0; i < numDevices; ++i) {
    if (statusArray[i].critical && statusArray[i].mode != SPN_MAINTENANCE) {
      return true;
    }
  }
  return false;
}

long getAlarmReason() {
  long alarmReason = SPN_ALARM_NO_ALARM;
  for (int i = 0; i < numDevices; ++i) {
    alarmReason |= statusArray[i].alarmReason;
  }
  return alarmReason;
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
  String alarmStr =
      RdJson::getString("alarm", "", isValid, objType, objSize, dataCpy);

  String idStr = RdJson::getString("id", "", isValid, objType, objSize, dataCpy);
  int id = stringToInt(idStr, -1);
  DeviceStatus* thisStatus = getStatusById(id);
  if (thisStatus == nullptr) {
    Serial.print("unaccepted id ");
    Serial.println(id);
    return;
  }
  thisStatus->mode = stringToInt(mStr, SPN_MODE_UNKNOWN);
  thisStatus->alarmReason = binaryStringToInt(alarmStr, SPN_ALARM_NO_ALARM);
  thisStatus->technical = RdJson::getString("technical", "", isValid, objType, objSize, dataCpy) == "1";
  thisStatus->critical = RdJson::getString("critical", "", isValid, objType, objSize, dataCpy) == "1";

  Serial.print("alarmStr:");
  Serial.print(alarmStr);
  Serial.print("-");
  Serial.println(thisStatus->alarmReason);
  Serial.print("technical");
  Serial.println(thisStatus->technical);
  Serial.print("critical");
  Serial.println(thisStatus->critical);
  Serial.println("SumStat:");
  Serial.print("technical:");
  Serial.println(isTechnical());
  Serial.print("critical:");
  Serial.println(isCritical());
  Serial.print("alarmReason:");
  Serial.println(getAlarmReason());
}

void sendKeepAlivePacket() {
  sendKeepAlive = true;
}
